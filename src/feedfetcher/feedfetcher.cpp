/****************************************************************************
**
** QuiteRSS is a open-source cross-platform news feed reader
** Copyright (C) 2011-2018 QuiteRSS Team <quiterssteam@gmail.com>
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <https://www.gnu.org/licenses/>.
**
****************************************************************************/
#include "feedfetcher.h"

#include <QNetworkRequest>
#include <QTimer>
#include <QUrl>

FeedFetcher::FeedFetcher(QNetworkAccessManager *nam, QObject *parent) :
    QObject(parent),
    m_nam(nam),
    m_maxConcurrent(5),
    m_timeoutMs(30000),
    m_maxRedirects(5)
{
    m_queueTimer = new QTimer(this);
    m_queueTimer->setSingleShot(true);
    connect(m_queueTimer, &QTimer::timeout, this, &FeedFetcher::processQueue);
}

FeedFetcher::~FeedFetcher()
{
    cancelAll();
}

void FeedFetcher::setMaxConcurrent(int max)
{
    m_maxConcurrent = max;
}

void FeedFetcher::setTimeoutMs(int ms)
{
    m_timeoutMs = ms;
}

void FeedFetcher::setMaxRedirects(int max)
{
    m_maxRedirects = max;
}

void FeedFetcher::fetch(int feedId, const QString &url,
                        const QDateTime &lastModified)
{
    // Don't duplicate requests
    for (int i = 0; i < m_queue.size(); ++i) {
        if (m_queue.at(i).feedId == feedId)
            return;
    }
    if (m_active.contains(feedId))
        return;

    PendingRequest req;
    req.feedId = feedId;
    req.url = url;
    req.lastModified = lastModified;
    req.redirectCount = 0;
    m_queue.enqueue(req);

    // Start processing immediately
    m_queueTimer->start(0);
}

bool FeedFetcher::isFetching(int feedId) const
{
    return m_active.contains(feedId);
}

void FeedFetcher::cancelAll()
{
    foreach (QNetworkReply *reply, m_active.values()) {
        reply->abort();
    }
    m_active.clear();
    m_replyToFeed.clear();

    qDeleteAll(m_timeoutTimers);
    m_timeoutTimers.clear();

    m_queue.clear();
    m_queueTimer->stop();
}

void FeedFetcher::processQueue()
{
    while (m_active.size() < m_maxConcurrent && !m_queue.isEmpty()) {
        startRequest(m_queue.dequeue());
    }

    if (m_active.isEmpty() && m_queue.isEmpty()) {
        emit allDone();
    }
}

void FeedFetcher::startRequest(const PendingRequest &req)
{
    QNetworkRequest networkReq(QUrl(req.url));
    networkReq.setAttribute(QNetworkRequest::FollowRedirectsAttribute, false);
    networkReq.setRawHeader("User-Agent",
        "QuiteRSS/0.19.4 (RSS/Atom feed reader; https://quiterss.org)");
    networkReq.setRawHeader("Accept",
        "application/rss+xml, application/atom+xml, application/rdf+xml, application/xml, text/xml, */*");

    // Conditional request
    if (req.lastModified.isValid()) {
        networkReq.setRawHeader("If-Modified-Since",
            req.lastModified.toUTC().toString("ddd, dd MMM yyyy HH:mm:ss 'GMT'").toUtf8());
    }

    QNetworkReply *reply = m_nam->get(networkReq);
    m_active.insert(req.feedId, reply);
    m_replyToFeed.insert(reply, req.feedId);

    connect(reply, &QNetworkReply::finished,
            this, &FeedFetcher::replyFinished);

    // Timeout timer
    QTimer *timer = new QTimer(this);
    timer->setSingleShot(true);
    timer->setProperty("feedId", req.feedId);
    connect(timer, &QTimer::timeout, this, &FeedFetcher::requestTimeout);
    timer->start(m_timeoutMs);
    m_timeoutTimers.insert(req.feedId, timer);
}

void FeedFetcher::replyFinished(QNetworkReply *reply)
{
    int feedId = m_replyToFeed.value(reply, -1);
    if (feedId < 0)
        return;

    // Remove from active maps
    m_active.remove(feedId);
    m_replyToFeed.remove(reply);

    // Stop timeout timer
    QTimer *timer = m_timeoutTimers.take(feedId);
    if (timer) {
        timer->stop();
        timer->deleteLater();
    }

    FetchResult result;
    result.feedId = feedId;

    // Check for HTTP redirect
    QVariant redirectAttr = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (redirectAttr.isValid()) {
        QUrl redirectUrl = redirectAttr.toUrl();
        if (redirectUrl.isRelative())
            redirectUrl = reply->url().resolved(redirectUrl);

        // Find the original pending request to get redirect count
        int redirectCount = 0;
        for (int i = 0; i < m_queue.size(); ++i) {
            // Not in queue - check if we can find the original request
        }
        // We need to track redirect count differently
        // For now, re-enqueue with increment
        PendingRequest req;
        req.feedId = feedId;
        req.url = redirectUrl.toString();
        req.lastModified = QDateTime();
        req.redirectCount = redirectCount + 1;

        if (req.redirectCount <= m_maxRedirects) {
            m_queue.prepend(req);
            m_queueTimer->start(100);
            return;
        } else {
            result.error = "Too many redirects";
            result.feedUrl = reply->url().toString();
            emit fetchDone(result);
            return;
        }
    }

    result.feedUrl = reply->url().toString();
    result.httpStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    // 304 Not Modified
    if (result.httpStatus == 304) {
        result.notModified = true;
        emit fetchDone(result);
        return;
    }

    // Error
    if (reply->error() != QNetworkReply::NoError) {
        result.error = reply->errorString();
        emit fetchDone(result);
        return;
    }

    // Success
    result.data = reply->readAll();

    // Extract charset from Content-Type header
    QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
    int charsetPos = contentType.indexOf("charset=");
    if (charsetPos >= 0) {
        result.charset = contentType.mid(charsetPos + 8).trimmed();
        // Remove quotes if present
        if (result.charset.startsWith('"'))
            result.charset = result.charset.mid(1);
        if (result.charset.endsWith('"'))
            result.charset.chop(1);
    }

    // Last-Modified header
    QString lastMod = reply->header(QNetworkRequest::LastModifiedHeader).toString();
    if (!lastMod.isEmpty())
        result.lastModified = QDateTime::fromString(lastMod, "ddd, dd MMM yyyy HH:mm:ss 'GMT'");

    emit fetchDone(result);

    // Process more items in queue
    m_queueTimer->start(50);
}

void FeedFetcher::requestTimeout()
{
    QTimer *timer = qobject_cast<QTimer *>(sender());
    if (!timer)
        return;

    int feedId = timer->property("feedId").toInt();
    QNetworkReply *reply = m_active.value(feedId);
    if (reply) {
        reply->abort();
    }

    m_timeoutTimers.remove(feedId);
    timer->deleteLater();

    FetchResult result;
    result.feedId = feedId;
    result.error = "Request timed out";
    emit fetchDone(result);

    m_queueTimer->start(50);
}

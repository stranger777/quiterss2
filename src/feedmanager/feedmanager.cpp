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
#include "feedmanager.h"
#include "databasemanager.h"

#include <QNetworkAccessManager>

FeedManager::FeedManager(DatabaseManager *db, QNetworkAccessManager *nam, QObject *parent) :
    QObject(parent),
    m_db(db),
    m_fetchingAll(false)
{
    m_fetcher = new FeedFetcher(nam, this);
    m_fetcher->setMaxConcurrent(5);
    m_fetcher->setTimeoutMs(30000);

    m_parser = new FeedParser(this);

    connect(m_fetcher, &FeedFetcher::fetchDone,
            this, &FeedManager::onFetchDone);
    connect(m_fetcher, &FeedFetcher::allDone,
            this, &FeedManager::onFetchAllDone);

    loadFeeds();
}

FeedManager::~FeedManager()
{
}

void FeedManager::fetchAllFeeds()
{
    if (!m_db->isOpen())
        return;

    m_fetchingAll = true;
    loadFeeds();

    foreach (const FeedState &state, m_feeds) {
        m_fetcher->fetch(state.id, state.url, state.lastUpdate);
    }
}

void FeedManager::fetchFeed(int feedId)
{
    if (!m_db->isOpen())
        return;

    FeedInfo info = m_db->getFeed(feedId);
    if (info.id <= 0)
        return;

    FeedState state;
    state.id = info.id;
    state.url = info.url;
    state.lastUpdate = QDateTime::fromSecsSinceEpoch(info.lastUpdate);
    state.title = info.title;
    m_feeds[feedId] = state;

    m_fetcher->fetch(feedId, info.url, state.lastUpdate);
}

int FeedManager::addFeed(const QString &url, const QString &title, const QString &category)
{
    if (!m_db->isOpen())
        return -1;

    // Fetch the feed first to get metadata
    // For now, create a placeholder - the fetch will update title from the feed
    QString feedTitle = title;
    if (feedTitle.isEmpty())
        feedTitle = url;  // Use URL as temporary title

    int feedId = m_db->addFeed(feedTitle, url, category);
    if (feedId > 0) {
        FeedState state;
        state.id = feedId;
        state.url = url;
        state.title = feedTitle;
        m_feeds[feedId] = state;

        // Fetch immediately to get real metadata
        m_fetcher->fetch(feedId, url);
    }
    return feedId;
}

bool FeedManager::removeFeed(int feedId)
{
    bool ok = m_db->removeFeed(feedId);
    if (ok) {
        m_feeds.remove(feedId);
        emit feedsUpdated();
    }
    return ok;
}

QMap<int, FeedInfo> FeedManager::feeds() const
{
    QMap<int, FeedInfo> result;
    foreach (const FeedInfo &info, m_db->getFeeds()) {
        result[info.id] = info;
    }
    return result;
}

QList<ArticleInfo> FeedManager::articles(int feedId, int limit) const
{
    return m_db->getArticles(feedId, limit);
}

bool FeedManager::markFeedRead(int feedId)
{
    return m_db->markFeedRead(feedId);
}

bool FeedManager::markArticleRead(int articleId, bool read)
{
    return m_db->markArticleRead(articleId, read);
}

int FeedManager::totalUnreadCount() const
{
    return m_db->getUnreadCount();
}

void FeedManager::onFetchDone(const FetchResult &result)
{
    if (result.notModified) {
        // Feed hasn't changed since last fetch
        return;
    }

    if (!result.error.isEmpty() && result.httpStatus != 200) {
        emit error(result.feedId, result.error);
        return;
    }

    if (result.data.isEmpty()) {
        emit error(result.feedId, "Empty response");
        return;
    }

    processFetchResult(result);
}

void FeedManager::onFetchAllDone()
{
    m_fetchingAll = false;
    emit feedsUpdated();
}

void FeedManager::processFetchResult(const FetchResult &result)
{
    ParsedFeed feed;
    QDateTime fetchTime = QDateTime::currentDateTime();

    if (!m_parser->parse(result.data, result.feedUrl, fetchTime, feed)) {
        emit error(result.feedId, "Failed to parse feed");
        return;
    }

    // Update feed metadata in DB if we got a real title
    if (!feed.title.isEmpty()) {
        m_db->updateFeed(result.feedId, feed.title);
    }

    // Update last update timestamp
    m_db->setFeedLastUpdate(result.feedId, fetchTime.toSecsSinceEpoch());

    // Insert articles
    int newCount = 0;
    foreach (const ParsedArticle &pa, feed.articles) {
        ArticleInfo ai;
        ai.feedId = result.feedId;
        ai.guid = pa.guid;
        ai.title = pa.title;
        ai.link = pa.link;
        ai.content = FeedParser::extractContent(pa.summary, pa.content);
        ai.author = pa.author;
        ai.date = pa.published.isValid() ? pa.published.toSecsSinceEpoch()
                                         : pa.received.toSecsSinceEpoch();
        ai.isRead = false;
        ai.isStarred = false;

        int articleId = m_db->addArticle(result.feedId, ai);
        if (articleId > 0)
            newCount++;
    }

    emit feedUpdated(result.feedId, newCount);
    if (newCount > 0)
        emit newArticles(result.feedId, newCount);
}

void FeedManager::loadFeeds()
{
    if (!m_db->isOpen())
        return;

    m_feeds.clear();
    foreach (const FeedInfo &info, m_db->getFeeds()) {
        FeedState state;
        state.id = info.id;
        state.url = info.url;
        state.lastUpdate = QDateTime::fromSecsSinceEpoch(info.lastUpdate);
        state.title = info.title;
        m_feeds[info.id] = state;
    }
}

QString FeedManager::detectCharset(const QByteArray &data)
{
    // Simple heuristic: look for XML declaration
    int pos = data.indexOf("encoding=\"");
    if (pos >= 0) {
        int start = pos + 10;
        int end = data.indexOf('"', start);
        if (end > start)
            return QString::fromLatin1(data.mid(start, end - start));
    }
    pos = data.indexOf("encoding='");
    if (pos >= 0) {
        int start = pos + 10;
        int end = data.indexOf('\'', start);
        if (end > start)
            return QString::fromLatin1(data.mid(start, end - start));
    }
    return "UTF-8";
}

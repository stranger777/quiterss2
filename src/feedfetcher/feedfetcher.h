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
#ifndef FEEDFETCHER_H
#define FEEDFETCHER_H

#include <QByteArray>
#include <QDateTime>
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QQueue>
#include <QTimer>

struct FetchResult {
    int feedId;
    QString feedUrl;
    int httpStatus;
    QByteArray data;
    QString charset;       // Encoding from HTTP header
    QDateTime lastModified; // Last-Modified header
    QString error;         // Error message if failed
    bool notModified;      // 304 Not Modified
};

class FeedFetcher : public QObject
{
    Q_OBJECT
public:
    explicit FeedFetcher(QNetworkAccessManager *nam, QObject *parent = 0);
    ~FeedFetcher();

    // Configuration
    void setMaxConcurrent(int max);     // Default: 5
    void setTimeoutMs(int ms);          // Default: 30000
    void setMaxRedirects(int max);      // Default: 5

    // Queue a feed for fetching
    void fetch(int feedId, const QString &url,
               const QDateTime &lastModified = QDateTime());

    // Check if a feed is in progress
    bool isFetching(int feedId) const;

    // Cancel all pending requests
    void cancelAll();

signals:
    void fetchDone(const FetchResult &result);
    void fetchError(int feedId, const QString &error);
    void allDone();

private slots:
    void processQueue();
    void replyFinished(QNetworkReply *reply);
    void requestTimeout();

private:
    struct PendingRequest {
        int feedId;
        QString url;
        QDateTime lastModified;
        int redirectCount;
    };

    void startRequest(const PendingRequest &req);
    void finishRequest(int feedId, const FetchResult &result);

    QNetworkAccessManager *m_nam;
    QQueue<PendingRequest> m_queue;
    QMap<int, QNetworkReply *> m_active;  // feedId -> reply
    QMultiMap<QNetworkReply *, int> m_replyToFeed;
    QTimer *m_queueTimer;
    QMap<int, QTimer *> m_timeoutTimers;

    int m_maxConcurrent;
    int m_timeoutMs;
    int m_maxRedirects;
};

#endif // FEEDFETCHER_H

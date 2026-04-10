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
#ifndef FEEDMANAGER_H
#define FEEDMANAGER_H

#include "feedparser.h"
#include "feedfetcher.h"

#include <QDateTime>
#include <QMap>
#include <QObject>
#include <QTimer>

class DatabaseManager;
class QNetworkAccessManager;

class FeedManager : public QObject
{
    Q_OBJECT
public:
    explicit FeedManager(DatabaseManager *db,
                         QNetworkAccessManager *nam,
                         QObject *parent = 0);
    ~FeedManager();

    // Fetch all feeds from the database
    void fetchAllFeeds();

    // Fetch a single feed by ID
    void fetchFeed(int feedId);

    // Add a new feed (parse URL, fetch, insert into DB)
    int addFeed(const QString &url, const QString &title = QString(),
                const QString &category = QString());

    // Remove a feed
    bool removeFeed(int feedId);

    // Get list of all feeds (id -> {title, url, lastUpdate})
    QMap<int, FeedInfo> feeds() const;

    // Get articles for a feed
    QList<ArticleInfo> articles(int feedId = -1, int limit = 100) const;

    // Mark all articles in a feed as read
    bool markFeedRead(int feedId);

    // Mark a single article as read
    bool markArticleRead(int articleId, bool read = true);

    // Get total unread count
    int totalUnreadCount() const;

signals:
    void feedUpdated(int feedId, int newArticles);
    void feedsUpdated();
    void error(int feedId, const QString &message);
    void newArticles(int feedId, int count);

private slots:
    void onFetchDone(const FetchResult &result);
    void onFetchAllDone();

private:
    struct FeedState {
        int id;
        QString url;
        QDateTime lastUpdate;
        QString title;
    };

    void loadFeeds();
    void processFetchResult(const FetchResult &result);
    QString detectCharset(const QByteArray &data);

    DatabaseManager *m_db;
    FeedFetcher *m_fetcher;
    FeedParser *m_parser;
    QMap<int, FeedState> m_feeds;
    bool m_fetchingAll;
};

#endif // FEEDMANAGER_H

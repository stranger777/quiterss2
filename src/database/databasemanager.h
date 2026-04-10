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
#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QDateTime>
#include <QList>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

struct FeedInfo {
    int id;
    QString title;
    QString url;
    QString category;
    QString icon;
    qint64 lastUpdate;
    int unreadCount;
};

struct ArticleInfo {
    int id;
    int feedId;
    QString guid;
    QString title;
    QString link;
    QString content;
    QString author;
    qint64 date;
    bool isRead;
    bool isStarred;
};

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent = 0);
    ~DatabaseManager();

    bool initialize(const QString &dbPath);
    bool isOpen() const;

    // Feed operations
    int addFeed(const QString &title, const QString &url, const QString &category = QString());
    bool removeFeed(int feedId);
    bool updateFeed(int feedId, const QString &title = QString(), const QString &url = QString());
    QList<FeedInfo> getFeeds() const;
    FeedInfo getFeed(int feedId) const;
    bool setFeedIcon(int feedId, const QString &iconUrl);
    bool setFeedLastUpdate(int feedId, qint64 timestamp);

    // Article operations
    int addArticle(int feedId, const ArticleInfo &article);
    bool removeArticle(int articleId);
    QList<ArticleInfo> getArticles(int feedId = -1, int limit = 100, int offset = 0) const;
    ArticleInfo getArticle(int articleId) const;
    bool markArticleRead(int articleId, bool read = true);
    bool markFeedRead(int feedId);
    int getUnreadCount(int feedId = -1) const;
    bool setArticleStarred(int articleId, bool starred = true);

    // Bulk operations
    bool addArticles(int feedId, const QList<ArticleInfo> &articles);

    // Utility
    void vacuum();
    int databaseSize() const;  // in bytes

signals:
    void databaseOpened();
    void databaseError(const QString &error);
    void feedAdded(int feedId);
    void feedRemoved(int feedId);
    void articleAdded(int articleId, int feedId);
    void unreadCountChanged(int feedId, int count);

private:
    void createTables();
    bool exec(const QString &sql, const QVariantList &bindValues = QVariantList());
    FeedInfo feedFromQuery(QSqlQuery &query) const;
    ArticleInfo articleFromQuery(QSqlQuery &query) const;

    QSqlDatabase m_db;
    bool m_initialized;
};

#endif // DATABASEMANAGER_H

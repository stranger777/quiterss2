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
#include "databasemanager.h"

#include <QDir>
#include <QFileInfo>
#include <QSqlError>
#include <QSqlQuery>

DatabaseManager::DatabaseManager(QObject *parent) :
    QObject(parent),
    m_initialized(false)
{
}

DatabaseManager::~DatabaseManager()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
}

bool DatabaseManager::initialize(const QString &dbPath)
{
    if (m_initialized)
        return true;

    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(dbPath);

    if (!m_db.open()) {
        emit databaseError(m_db.lastError().text());
        return false;
    }

    createTables();
    m_initialized = true;
    emit databaseOpened();
    return true;
}

bool DatabaseManager::isOpen() const
{
    return m_initialized && m_db.isOpen();
}

void DatabaseManager::createTables()
{
    QStringList tables = m_db.tables();

    if (!tables.contains("feeds", Qt::CaseInsensitive)) {
        exec("CREATE TABLE feeds ("
             "id INTEGER PRIMARY KEY AUTOINCREMENT, "
             "title TEXT NOT NULL DEFAULT '', "
             "url TEXT UNIQUE NOT NULL DEFAULT '', "
             "category TEXT NOT NULL DEFAULT '', "
             "icon TEXT NOT NULL DEFAULT '', "
             "lastUpdate INTEGER NOT NULL DEFAULT 0, "
             "unreadCount INTEGER NOT NULL DEFAULT 0"
             ")");
    }

    if (!tables.contains("articles", Qt::CaseInsensitive)) {
        exec("CREATE TABLE articles ("
             "id INTEGER PRIMARY KEY AUTOINCREMENT, "
             "feedId INTEGER NOT NULL, "
             "guid TEXT NOT NULL DEFAULT '', "
             "title TEXT NOT NULL DEFAULT '', "
             "link TEXT NOT NULL DEFAULT '', "
             "content TEXT NOT NULL DEFAULT '', "
             "author TEXT NOT NULL DEFAULT '', "
             "date INTEGER NOT NULL DEFAULT 0, "
             "read INTEGER NOT NULL DEFAULT 0, "
             "starred INTEGER NOT NULL DEFAULT 0, "
             "FOREIGN KEY(feedId) REFERENCES feeds(id) ON DELETE CASCADE"
             ")");
    }

    // Indexes for common queries
    exec("CREATE INDEX IF NOT EXISTS idx_articles_feedId ON articles(feedId)");
    exec("CREATE INDEX IF NOT EXISTS idx_articles_read ON articles(read)");
    exec("CREATE INDEX IF NOT EXISTS idx_articles_date ON articles(date DESC)");
    exec("CREATE INDEX IF NOT EXISTS idx_articles_feedId_date ON articles(feedId, date DESC)");
}

bool DatabaseManager::exec(const QString &sql, const QVariantList &bindValues)
{
    QSqlQuery query(m_db);
    query.prepare(sql);
    for (int i = 0; i < bindValues.size(); ++i)
        query.bindValue(i, bindValues.at(i));

    if (!query.exec()) {
        emit databaseError(query.lastError().text());
        return false;
    }
    return true;
}

// === Feed operations ===

int DatabaseManager::addFeed(const QString &title, const QString &url, const QString &category)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO feeds (title, url, category) VALUES (?, ?, ?)");
    query.addBindValue(title);
    query.addBindValue(url);
    query.addBindValue(category);

    if (query.exec())
        return query.lastInsertId().toInt();

    emit databaseError(query.lastError().text());
    return -1;
}

bool DatabaseManager::removeFeed(int feedId)
{
    return exec("DELETE FROM feeds WHERE id = ?", QVariantList() << feedId);
}

bool DatabaseManager::updateFeed(int feedId, const QString &title, const QString &url)
{
    QSqlQuery query(m_db);
    if (!title.isEmpty() && !url.isEmpty()) {
        query.prepare("UPDATE feeds SET title = ?, url = ? WHERE id = ?");
        query.addBindValue(title);
        query.addBindValue(url);
    } else if (!title.isEmpty()) {
        query.prepare("UPDATE feeds SET title = ? WHERE id = ?");
        query.addBindValue(title);
    } else if (!url.isEmpty()) {
        query.prepare("UPDATE feeds SET url = ? WHERE id = ?");
        query.addBindValue(url);
    } else {
        return false;
    }
    query.addBindValue(feedId);
    return query.exec();
}

QList<FeedInfo> DatabaseManager::getFeeds() const
{
    QList<FeedInfo> feeds;
    QSqlQuery query("SELECT id, title, url, category, icon, lastUpdate, unreadCount FROM feeds ORDER BY title", m_db);
    while (query.next())
        feeds.append(feedFromQuery(query));
    return feeds;
}

FeedInfo DatabaseManager::getFeed(int feedId) const
{
    QSqlQuery query(m_db);
    query.prepare("SELECT id, title, url, category, icon, lastUpdate, unreadCount FROM feeds WHERE id = ?");
    query.addBindValue(feedId);
    if (query.exec() && query.next())
        return feedFromQuery(query);
    return FeedInfo();
}

bool DatabaseManager::setFeedIcon(int feedId, const QString &iconUrl)
{
    return exec("UPDATE feeds SET icon = ? WHERE id = ?", QVariantList() << iconUrl << feedId);
}

bool DatabaseManager::setFeedLastUpdate(int feedId, qint64 timestamp)
{
    return exec("UPDATE feeds SET lastUpdate = ? WHERE id = ?", QVariantList() << timestamp << feedId);
}

// === Article operations ===

int DatabaseManager::addArticle(int feedId, const ArticleInfo &article)
{
    // Skip if guid already exists
    if (!article.guid.isEmpty()) {
        QSqlQuery check(m_db);
        check.prepare("SELECT COUNT(*) FROM articles WHERE feedId = ? AND guid = ?");
        check.addBindValue(feedId);
        check.addBindValue(article.guid);
        if (check.exec() && check.next() && check.value(0).toInt() > 0)
            return -2;  // duplicate
    }

    QSqlQuery query(m_db);
    query.prepare("INSERT INTO articles (feedId, guid, title, link, content, author, date, read, starred) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(feedId);
    query.addBindValue(article.guid);
    query.addBindValue(article.title);
    query.addBindValue(article.link);
    query.addBindValue(article.content);
    query.addBindValue(article.author);
    query.addBindValue(article.date);
    query.addBindValue(article.isRead ? 1 : 0);
    query.addBindValue(article.isStarred ? 1 : 0);

    if (query.exec()) {
        int id = query.lastInsertId().toInt();
        emit articleAdded(id, feedId);
        return id;
    }
    emit databaseError(query.lastError().text());
    return -1;
}

bool DatabaseManager::removeArticle(int articleId)
{
    return exec("DELETE FROM articles WHERE id = ?", QVariantList() << articleId);
}

QList<ArticleInfo> DatabaseManager::getArticles(int feedId, int limit, int offset) const
{
    QList<ArticleInfo> articles;
    QSqlQuery query(m_db);

    if (feedId >= 0) {
        query.prepare("SELECT id, feedId, guid, title, link, content, author, date, read, starred "
                      "FROM articles WHERE feedId = ? ORDER BY date DESC LIMIT ? OFFSET ?");
        query.addBindValue(feedId);
    } else {
        query.prepare("SELECT id, feedId, guid, title, link, content, author, date, read, starred "
                      "FROM articles ORDER BY date DESC LIMIT ? OFFSET ?");
    }
    query.addBindValue(limit);
    query.addBindValue(offset);

    if (query.exec()) {
        while (query.next())
            articles.append(articleFromQuery(query));
    }
    return articles;
}

ArticleInfo DatabaseManager::getArticle(int articleId) const
{
    QSqlQuery query(m_db);
    query.prepare("SELECT id, feedId, guid, title, link, content, author, date, read, starred "
                  "FROM articles WHERE id = ?");
    query.addBindValue(articleId);
    if (query.exec() && query.next())
        return articleFromQuery(query);
    return ArticleInfo();
}

bool DatabaseManager::markArticleRead(int articleId, bool read)
{
    return exec("UPDATE articles SET read = ? WHERE id = ?",
                QVariantList() << (read ? 1 : 0) << articleId);
}

bool DatabaseManager::markFeedRead(int feedId)
{
    return exec("UPDATE articles SET read = 1 WHERE feedId = ? AND read = 0",
                QVariantList() << feedId);
}

int DatabaseManager::getUnreadCount(int feedId) const
{
    QSqlQuery query(m_db);
    if (feedId >= 0) {
        query.prepare("SELECT COUNT(*) FROM articles WHERE feedId = ? AND read = 0");
        query.addBindValue(feedId);
    } else {
        query.prepare("SELECT COUNT(*) FROM articles WHERE read = 0");
    }
    if (query.exec() && query.next())
        return query.value(0).toInt();
    return 0;
}

bool DatabaseManager::setArticleStarred(int articleId, bool starred)
{
    return exec("UPDATE articles SET starred = ? WHERE id = ?",
                QVariantList() << (starred ? 1 : 0) << articleId);
}

bool DatabaseManager::addArticles(int feedId, const QList<ArticleInfo> &articles)
{
    bool success = true;
    m_db.transaction();
    foreach (const ArticleInfo &a, articles) {
        if (addArticle(feedId, a) < 0) {
            success = false;
            break;
        }
    }
    if (success)
        m_db.commit();
    else
        m_db.rollback();
    return success;
}

// === Utility ===

void DatabaseManager::vacuum()
{
    exec("VACUUM");
}

int DatabaseManager::databaseSize() const
{
    if (!m_db.isOpen())
        return 0;
    QFileInfo fi(m_db.databaseName());
    return fi.exists() ? fi.size() : 0;
}

// === Private helpers ===

FeedInfo DatabaseManager::feedFromQuery(QSqlQuery &query) const
{
    FeedInfo info;
    info.id = query.value(0).toInt();
    info.title = query.value(1).toString();
    info.url = query.value(2).toString();
    info.category = query.value(3).toString();
    info.icon = query.value(4).toString();
    info.lastUpdate = query.value(5).toLongLong();
    info.unreadCount = query.value(6).toInt();
    return info;
}

ArticleInfo DatabaseManager::articleFromQuery(QSqlQuery &query) const
{
    ArticleInfo info;
    info.id = query.value(0).toInt();
    info.feedId = query.value(1).toInt();
    info.guid = query.value(2).toString();
    info.title = query.value(3).toString();
    info.link = query.value(4).toString();
    info.content = query.value(5).toString();
    info.author = query.value(6).toString();
    info.date = query.value(7).toLongLong();
    info.isRead = query.value(8).toBool();
    info.isStarred = query.value(9).toBool();
    return info;
}

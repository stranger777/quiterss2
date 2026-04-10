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
#ifndef FEEDPARSER_H
#define FEEDPARSER_H

#include <QDateTime>
#include <QList>
#include <QObject>
#include <QXmlStreamReader>

struct ParsedArticle {
    QString guid;
    QString title;
    QString link;
    QString content;       // Full HTML content
    QString summary;       // Brief description
    QString author;
    QString authorUri;
    QString authorEmail;
    QDateTime published;   // Article publish date
    QDateTime received;    // When we fetched it
    QString category;      // Comma-separated categories
    // Media-RSS enclosures
    QString enclosureUrl;
    QString enclosureType;
    qint64 enclosureLength;
    // Comments
    QString commentsUrl;
};

struct ParsedFeed {
    QString title;
    QString link;          // htmlUrl - website URL
    QString xmlUrl;        // Feed URL
    QString description;
    QString language;
    QString lastBuildDate;
    QList<ParsedArticle> articles;
};

class FeedParser : public QObject
{
    Q_OBJECT
public:
    explicit FeedParser(QObject *parent = 0);

    // Parse XML data, return true if valid feed format detected
    bool parse(const QByteArray &xmlData, const QString &feedUrl,
               const QDateTime &fetchTime, ParsedFeed &feed);

    // Date parsing utility
    static QDateTime parseDate(const QString &dateStr);

    // Extract article content (clean HTML if possible)
    static QString extractContent(const QString &summary, const QString &content);

signals:
    void parseError(const QString &message);

private:
    enum FeedType { UnknownFeed, RssFeed, AtomFeed, RdfFeed };

    FeedType detectFeedType(const QXmlStreamReader &xml);
    void parseRss(QXmlStreamReader &xml, ParsedFeed &feed);
    void parseAtom(QXmlStreamReader &xml, ParsedFeed &feed);
    void parseRdf(QXmlStreamReader &xml, ParsedFeed &feed);

    void parseRssItem(QXmlStreamReader &xml, ParsedArticle &article);
    void parseAtomEntry(QXmlStreamReader &xml, ParsedArticle &article);
    void parseRdfItem(QXmlStreamReader &xml, ParsedArticle &article);

    void parseMediaElements(QXmlStreamReader &xml, ParsedArticle &article);
    QString readElementText(QXmlStreamReader &xml);
    QString readAttribute(const QXmlStreamReader &xml, const QString &name);

    QString m_feedUrl;
    QDateTime m_fetchTime;
};

#endif // FEEDPARSER_H

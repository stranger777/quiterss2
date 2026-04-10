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
#include "feedparser.h"

#include <QDateTime>
#include <QRegularExpression>
#include <QXmlStreamAttributes>

FeedParser::FeedParser(QObject *parent) : QObject(parent)
{
}

bool FeedParser::parse(const QByteArray &xmlData, const QString &feedUrl,
                       const QDateTime &fetchTime, ParsedFeed &feed)
{
    m_feedUrl = feedUrl;
    m_fetchTime = fetchTime;

    QXmlStreamReader xml(xmlData);

    // Skip to root element
    while (!xml.atEnd() && !xml.isStartElement())
        xml.readNext();

    if (xml.atEnd()) {
        emit parseError(xml.errorString());
        return false;
    }

    FeedType type = detectFeedType(xml);
    if (type == UnknownFeed) {
        emit parseError("Unknown feed format");
        return false;
    }

    switch (type) {
    case RssFeed: parseRss(xml, feed); break;
    case AtomFeed: parseAtom(xml, feed); break;
    case RdfFeed: parseRdf(xml, feed); break;
    default: break;
    }

    // Set xmlUrl from feed URL if not found in the feed itself
    if (feed.xmlUrl.isEmpty())
        feed.xmlUrl = feedUrl;

    return !feed.title.isEmpty() || !feed.articles.isEmpty();
}

FeedParser::FeedType FeedParser::detectFeedType(const QXmlStreamReader &xml)
{
    QStringRef name = xml.name();
    QStringRef nsUri = xml.namespaceUri();

    if (name == "rss")
        return RssFeed;
    if (name == "feed" && nsUri.toString().contains("atom"))
        return AtomFeed;
    if (name == "RDF" && nsUri.toString().contains("rdf"))
        return RdfFeed;
    // Fallback: check local name without namespace
    if (name == "feed")
        return AtomFeed;
    if (name == "RDF")
        return RdfFeed;

    return UnknownFeed;
}

// === RSS 2.0 ===

void FeedParser::parseRss(QXmlStreamReader &xml, ParsedFeed &feed)
{
    while (!xml.atEnd()) {
        xml.readNext();
        if (xml.isStartElement()) {
            QStringRef name = xml.name();
            if (name == "channel") {
                // Parse channel metadata
            } else if (name == "title" && feed.title.isEmpty()) {
                feed.title = readElementText(xml);
            } else if (name == "link" && feed.link.isEmpty()) {
                feed.link = readElementText(xml);
            } else if (name == "description" && feed.description.isEmpty()) {
                feed.description = readElementText(xml);
            } else if (name == "language") {
                feed.language = readElementText(xml);
            } else if (name == "lastBuildDate") {
                feed.lastBuildDate = readElementText(xml);
            } else if (name == "item") {
                ParsedArticle article;
                article.received = m_fetchTime;
                parseRssItem(xml, article);
                if (!article.title.isEmpty() || !article.link.isEmpty())
                    feed.articles.append(article);
            }
        } else if (xml.isEndElement() && xml.name() == "rss") {
            break;
        }
    }
}

void FeedParser::parseRssItem(QXmlStreamReader &xml, ParsedArticle &article)
{
    while (!xml.atEnd()) {
        xml.readNext();
        if (xml.isStartElement()) {
            QStringRef name = xml.name();
            if (name == "title") {
                article.title = readElementText(xml);
            } else if (name == "link") {
                article.link = readElementText(xml);
            } else if (name == "guid") {
                article.guid = readElementText(xml);
                bool isPermaLink = readAttribute(xml, "isPermaLink") != "false";
                if (isPermaLink && article.link.isEmpty())
                    article.link = article.guid;
            } else if (name == "description") {
                article.summary = readElementText(xml);
            } else if (name == "content:encoded") {
                article.content = readElementText(xml);
            } else if (name == "author") {
                article.author = readElementText(xml);
            } else if (name == "dc:creator") {
                if (article.author.isEmpty())
                    article.author = readElementText(xml);
            } else if (name == "pubDate") {
                article.published = parseDate(readElementText(xml));
            } else if (name == "dc:date") {
                if (!article.published.isValid())
                    article.published = parseDate(readElementText(xml));
            } else if (name == "category") {
                QString cat = readElementText(xml);
                if (!article.category.isEmpty())
                    article.category += ", ";
                article.category += cat;
            } else if (name == "comments") {
                article.commentsUrl = readElementText(xml);
            } else if (name == "enclosure") {
                article.enclosureUrl = readAttribute(xml, "url");
                article.enclosureType = readAttribute(xml, "type");
                article.enclosureLength = readAttribute(xml, "length").toLongLong();
            } else if (name == "media:content" || name == "media:thumbnail") {
                parseMediaElements(xml, article);
            }
        } else if (xml.isEndElement() && xml.name() == "item") {
            break;
        }
    }
}

// === Atom 1.0 ===

void FeedParser::parseAtom(QXmlStreamReader &xml, ParsedFeed &feed)
{
    while (!xml.atEnd()) {
        xml.readNext();
        if (xml.isStartElement()) {
            QStringRef name = xml.name();
            if (name == "title" && feed.title.isEmpty()) {
                feed.title = readElementText(xml);
            } else if (name == "subtitle" && feed.description.isEmpty()) {
                feed.description = readElementText(xml);
            } else if (name == "link") {
                QString rel = readAttribute(xml, "rel");
                QString href = readAttribute(xml, "href");
                if (rel.isEmpty() || rel == "alternate") {
                    if (feed.link.isEmpty())
                        feed.link = href;
                } else if (rel == "self") {
                    if (feed.xmlUrl.isEmpty())
                        feed.xmlUrl = href;
                }
            } else if (name == "updated") {
                feed.lastBuildDate = readElementText(xml);
            } else if (name == "entry") {
                ParsedArticle article;
                article.received = m_fetchTime;
                parseAtomEntry(xml, article);
                if (!article.title.isEmpty() || !article.link.isEmpty())
                    feed.articles.append(article);
            }
        } else if (xml.isEndElement() && xml.name() == "feed") {
            break;
        }
    }
}

void FeedParser::parseAtomEntry(QXmlStreamReader &xml, ParsedArticle &article)
{
    while (!xml.atEnd()) {
        xml.readNext();
        if (xml.isStartElement()) {
            QStringRef name = xml.name();
            if (name == "title") {
                article.title = readElementText(xml);
            } else if (name == "link") {
                QString rel = readAttribute(xml, "rel");
                QString href = readAttribute(xml, "href");
                if (rel.isEmpty() || rel == "alternate") {
                    if (article.link.isEmpty())
                        article.link = href;
                } else if (rel == "enclosure") {
                    article.enclosureUrl = href;
                    article.enclosureType = readAttribute(xml, "type");
                    article.enclosureLength = readAttribute(xml, "length").toLongLong();
                }
            } else if (name == "id") {
                article.guid = readElementText(xml);
                if (article.link.isEmpty() && article.guid.startsWith("http"))
                    article.link = article.guid;
            } else if (name == "summary") {
                article.summary = readElementText(xml);
            } else if (name == "content") {
                QString type = readAttribute(xml, "type");
                article.content = readElementText(xml);
                if (article.summary.isEmpty())
                    article.summary = article.content;
            } else if (name == "published") {
                article.published = parseDate(readElementText(xml));
            } else if (name == "updated") {
                if (!article.published.isValid())
                    article.published = parseDate(readElementText(xml));
            } else if (name == "author") {
                // Parse author sub-element
                while (!xml.atEnd()) {
                    xml.readNext();
                    if (xml.isStartElement()) {
                        if (xml.name() == "name")
                            article.author = readElementText(xml);
                        else if (xml.name() == "uri")
                            article.authorUri = readElementText(xml);
                        else if (xml.name() == "email")
                            article.authorEmail = readElementText(xml);
                    } else if (xml.isEndElement() && xml.name() == "author") {
                        break;
                    }
                }
            } else if (name == "category") {
                QString term = readAttribute(xml, "term");
                QString label = readAttribute(xml, "label");
                QString cat = label.isEmpty() ? term : label;
                if (!article.category.isEmpty())
                    article.category += ", ";
                article.category += cat;
            } else if (name == "media:content" || name == "media:thumbnail") {
                parseMediaElements(xml, article);
            }
        } else if (xml.isEndElement() && xml.name() == "entry") {
            break;
        }
    }
}

// === RSS 1.0 / RDF ===

void FeedParser::parseRdf(QXmlStreamReader &xml, ParsedFeed &feed)
{
    while (!xml.atEnd()) {
        xml.readNext();
        if (xml.isStartElement()) {
            QStringRef name = xml.name();
            if (name == "channel") {
                // Parse channel metadata
            } else if (name == "title" && feed.title.isEmpty()) {
                feed.title = readElementText(xml);
            } else if (name == "link" && feed.link.isEmpty()) {
                feed.link = readElementText(xml);
            } else if (name == "description" && feed.description.isEmpty()) {
                feed.description = readElementText(xml);
            } else if (name == "dc:language") {
                feed.language = readElementText(xml);
            } else if (name == "dc:date") {
                feed.lastBuildDate = readElementText(xml);
            } else if (name == "items") {
                // Skip the rdf:Seq container
                while (!xml.atEnd()) {
                    xml.readNext();
                    if (xml.isEndElement() && xml.name() == "items")
                        break;
                }
            } else if (name == "item") {
                ParsedArticle article;
                article.received = m_fetchTime;
                parseRdfItem(xml, article);
                if (!article.title.isEmpty() || !article.link.isEmpty())
                    feed.articles.append(article);
            }
        } else if (xml.isEndElement() && xml.name() == "RDF") {
            break;
        }
    }
}

void FeedParser::parseRdfItem(QXmlStreamReader &xml, ParsedArticle &article)
{
    while (!xml.atEnd()) {
        xml.readNext();
        if (xml.isStartElement()) {
            QStringRef name = xml.name();
            if (name == "title") {
                article.title = readElementText(xml);
            } else if (name == "link") {
                article.link = readElementText(xml);
            } else if (name == "guid") {
                article.guid = readElementText(xml);
            } else if (name == "description") {
                article.summary = readElementText(xml);
            } else if (name == "content:encoded") {
                article.content = readElementText(xml);
            } else if (name == "dc:creator") {
                article.author = readElementText(xml);
            } else if (name == "dc:date") {
                article.published = parseDate(readElementText(xml));
            } else if (name == "dc:subject") {
                QString cat = readElementText(xml);
                if (!article.category.isEmpty())
                    article.category += ", ";
                article.category += cat;
            }
        } else if (xml.isEndElement() && xml.name() == "item") {
            break;
        }
    }
}

// === Media-RSS ===

void FeedParser::parseMediaElements(QXmlStreamReader &xml, ParsedArticle &article)
{
    QString url = readAttribute(xml, "url");
    QString type = readAttribute(xml, "type");
    QString size = readAttribute(xml, "fileSize");

    if (!url.isEmpty() && article.enclosureUrl.isEmpty()) {
        article.enclosureUrl = url;
        article.enclosureType = type;
        article.enclosureLength = size.toLongLong();
    }

    // Read child elements like media:title, media:description
    while (!xml.atEnd()) {
        xml.readNext();
        if (xml.isStartElement()) {
            if (xml.name() == "media:title" && article.title.isEmpty()) {
                article.title = readElementText(xml);
            } else if (xml.name() == "media:description" && article.summary.isEmpty()) {
                article.summary = readElementText(xml);
            }
        } else if (xml.isEndElement() && (xml.name() == "media:content" || xml.name() == "media:thumbnail")) {
            break;
        }
    }
}

// === Utilities ===

QString FeedParser::readElementText(QXmlStreamReader &xml)
{
    QString text;
    while (!xml.atEnd()) {
        xml.readNext();
        if (xml.isCharacters()) {
            text += xml.text().toString();
        } else if (xml.isEndElement()) {
            break;
        } else if (xml.isStartElement()) {
            // Nested element - read its content as HTML
            QString nested;
            nested += "<";
            nested += xml.name().toString();
            QXmlStreamAttributes attrs = xml.attributes();
            for (int i = 0; i < attrs.count(); ++i) {
                nested += " " + attrs.at(i).name().toString() + "=\""
                          + attrs.at(i).value().toString() + "\"";
            }
            nested += ">";
            // Read until matching end tag
            int depth = 1;
            while (!xml.atEnd() && depth > 0) {
                xml.readNext();
                if (xml.isStartElement()) {
                    depth++;
                    nested += "<" + xml.name().toString();
                    QXmlStreamAttributes a2 = xml.attributes();
                    for (int i = 0; i < a2.count(); ++i) {
                        nested += " " + a2.at(i).name().toString() + "=\""
                                  + a2.at(i).value().toString() + "\"";
                    }
                    nested += ">";
                } else if (xml.isEndElement()) {
                    depth--;
                    if (depth > 0)
                        nested += "</" + xml.name().toString() + ">";
                } else if (xml.isCharacters()) {
                    nested += xml.text().toString();
                }
            }
            nested += "</" + xml.name().toString() + ">";
            text += nested;
        }
    }
    return text.trimmed();
}

QString FeedParser::readAttribute(const QXmlStreamReader &xml, const QString &name)
{
    return xml.attributes().value(name).toString();
}

QDateTime FeedParser::parseDate(const QString &dateStr)
{
    if (dateStr.isEmpty())
        return QDateTime();

    QString ds = dateStr.trimmed();

    // RFC 822 / RFC 1123: "Mon, 06 Sep 2009 16:20:00 +0000"
    // With or without day name
    static const QStringList rfc822Formats = {
        "ddd, dd MMM yyyy HH:mm:ss Z",
        "dd MMM yyyy HH:mm:ss Z",
        "ddd, dd MMM yyyy HH:mm Z",
        "dd MMM yyyy HH:mm Z",
        "ddd, d MMM yyyy HH:mm:ss Z",
        "d MMM yyyy HH:mm:ss Z",
    };
    for (const QString &fmt : rfc822Formats) {
        QDateTime dt = QDateTime::fromString(ds, fmt);
        if (dt.isValid())
            return dt.toUTC();
    }

    // ISO 8601: "2009-09-06T16:20:00Z" or "2009-09-06T16:20:00+00:00"
    static const QStringList isoFormats = {
        "yyyy-MM-ddTHH:mm:ssZ",
        "yyyy-MM-ddTHH:mm:sszzz",
        "yyyy-MM-ddTHH:mm:ss",
        "yyyy-MM-ddTHH:mmZ",
        "yyyy-MM-ddTHH:mmzzz",
        "yyyy-MM-ddTHH:mm",
        "yyyy-MM-dd HH:mm:ss Z",
        "yyyy-MM-dd HH:mm:ss",
        "yyyy-MM-dd",
    };
    for (const QString &fmt : isoFormats) {
        QDateTime dt = QDateTime::fromString(ds, fmt);
        if (dt.isValid())
            return dt.toUTC();
    }

    // Handle timezone offsets without colon: "+0000" -> "+00:00"
    QRegularExpression re("([+-]\\d{2})(\\d{2})$");
    QRegularExpressionMatch m = re.match(ds);
    if (m.hasCaptured()) {
        QString fixed = ds;
        fixed.replace(m.capturedStart(), m.capturedLength(),
                      m.captured(1) + ":" + m.captured(2));
        QDateTime dt = QDateTime::fromString(fixed, "yyyy-MM-ddTHH:mm:sszzz");
        if (dt.isValid())
            return dt.toUTC();
    }

    // Fallback: try Qt's general parsing
    QDateTime dt = QDateTime::fromString(ds, Qt::ISODate);
    if (dt.isValid())
        return dt.toUTC();

    return QDateTime();
}

QString FeedParser::extractContent(const QString &summary, const QString &content)
{
    if (!content.isEmpty())
        return content;
    return summary;
}

/****************************************************************************
** Copyright (c) 2013-2014 Debao Zhang <hello@debao.me>
** All right reserved.
**
** Permission is hereby granted, free of charge, to any person obtaining
** a copy of this software and associated documentation files (the
** "Software"), to deal in the Software without restriction, including
** without limitation the rights to use, copy, modify, merge, publish,
** distribute, sublicense, and/or sell copies of the Software, and to
** permit persons to whom the Software is furnished to do so, subject to
** the following conditions:
**
** The above copyright notice and this permission notice shall be
** included in all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
** NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
** LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
** OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
** WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**
****************************************************************************/
#include "xlsxdocpropscore_p.h"

#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QDir>
#include <QFile>
#include <QDateTime>
#include <QDebug>
#include <QBuffer>

namespace QXlsx {

DocPropsCore::DocPropsCore(CreateFlag flag)
    :AbstractOOXmlFile(flag)
{
}

bool DocPropsCore::setProperty(const QString &name, const QString &value)
{
    static QStringList validKeys;
    if (validKeys.isEmpty()) {
        validKeys << QString("title") << QString("subject")
                  << QString("keywords") << QString("description")
                  << QString("category") << QString("status")
                  << QString("created") << QString("creator");
    }

    if (!validKeys.contains(name))
        return false;

    if (value.isEmpty())
        m_properties.remove(name);
    else
        m_properties[name] = value;

    return true;
}

QString DocPropsCore::property(const QString &name) const
{
    if (m_properties.contains(name))
        return m_properties[name];

    return QString();
}

QStringList DocPropsCore::propertyNames() const
{
    return m_properties.keys();
}

void DocPropsCore::saveToXmlFile(QIODevice *device) const
{
    QXmlStreamWriter writer(device);
    const QString cp = QString("http://schemas.openxmlformats.org/package/2006/metadata/core-properties");
    const QString dc = QString("http://purl.org/dc/elements/1.1/");
    const QString dcterms = QString("http://purl.org/dc/terms/");
    const QString dcmitype = QString("http://purl.org/dc/dcmitype/");
    const QString xsi = QString("http://www.w3.org/2001/XMLSchema-instance");
    writer.writeStartDocument(QString("1.0"), true);
    writer.writeStartElement(QString("cp:coreProperties"));
    writer.writeNamespace(cp, QString("cp"));
    writer.writeNamespace(dc, QString("dc"));
    writer.writeNamespace(dcterms, QString("dcterms"));
    writer.writeNamespace(dcmitype, QString("dcmitype"));
    writer.writeNamespace(xsi, QString("xsi"));

    if (m_properties.contains(QString("title")))
        writer.writeTextElement(dc, QString("title"), m_properties[QString("title")]);

    if (m_properties.contains(QString("subject")))
        writer.writeTextElement(dc, QString("subject"), m_properties[QString("subject")]);

    writer.writeTextElement(dc, QString("creator"), m_properties.contains(QString("creator")) ? m_properties[QString("creator")] : QString("Qt Xlsx Library"));

    if (m_properties.contains(QString("keywords")))
        writer.writeTextElement(cp, QString("keywords"), m_properties[QString("keywords")]);

    if (m_properties.contains(QString("description")))
        writer.writeTextElement(dc, QString("description"), m_properties[QString("description")]);

    writer.writeTextElement(cp, QString("lastModifiedBy"), m_properties.contains(QString("creator")) ? m_properties[QString("creator")] : QString("Qt Xlsx Library"));

    writer.writeStartElement(dcterms, QString("created"));
    writer.writeAttribute(xsi, QString("type"), QString("dcterms:W3CDTF"));
    writer.writeCharacters(m_properties.contains(QString("created")) ? m_properties[QString("created")] : QDateTime::currentDateTime().toString(Qt::ISODate));
    writer.writeEndElement();//dcterms:created

    writer.writeStartElement(dcterms, QString("modified"));
    writer.writeAttribute(xsi, QString("type"), QString("dcterms:W3CDTF"));
    writer.writeCharacters(QDateTime::currentDateTime().toString(Qt::ISODate));
    writer.writeEndElement();//dcterms:created

    if (m_properties.contains(QString("category")))
        writer.writeTextElement(cp, QString("category"), m_properties[QString("category")]);

    if (m_properties.contains(QString("status")))
        writer.writeTextElement(cp, QString("contentStatus"), m_properties[QString("status")]);

    writer.writeEndElement(); //cp:coreProperties
    writer.writeEndDocument();
}

bool DocPropsCore::loadFromXmlFile(QIODevice *device)
{
    QXmlStreamReader reader(device);

    const QString cp = QString("http://schemas.openxmlformats.org/package/2006/metadata/core-properties");
    const QString dc = QString("http://purl.org/dc/elements/1.1/");
    const QString dcterms = QString("http://purl.org/dc/terms/");

    while (!reader.atEnd()) {
         QXmlStreamReader::TokenType token = reader.readNext();
         if (token == QXmlStreamReader::StartElement) {
             const QStringRef nsUri = reader.namespaceUri();
             const QStringRef name = reader.name();
             if (name == QString("subject") && nsUri == dc) {
                 setProperty(QString("subject"), reader.readElementText());
             } else if (name == QString("title") && nsUri == dc) {
                 setProperty(QString("title"), reader.readElementText());
             } else if (name == QString("creator") && nsUri == dc) {
                 setProperty(QString("creator"), reader.readElementText());
             } else if (name == QString("description") && nsUri == dc) {
                 setProperty(QString("description"), reader.readElementText());
             } else if (name == QString("keywords") && nsUri == cp) {
                 setProperty(QString("keywords"), reader.readElementText());
             } else if (name == QString("created") && nsUri == dcterms) {
                 setProperty(QString("created"), reader.readElementText());
             } else if (name == QString("category") && nsUri == cp) {
                 setProperty(QString("category"), reader.readElementText());
             } else if (name == QString("contentStatus") && nsUri == cp) {
                 setProperty(QString("status"), reader.readElementText());
             }
         }

         if (reader.hasError()) {
             qDebug()<<"Error when read doc props core file."<<reader.errorString();

         }
    }
    return true;
}

} //namespace

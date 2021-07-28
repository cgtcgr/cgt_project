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
#include "xlsxcontenttypes_p.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>
#include <QMapIterator>
#include <QBuffer>
#include <QDebug>

namespace QXlsx {

ContentTypes::ContentTypes(CreateFlag flag)
    :AbstractOOXmlFile(flag)
{
    m_package_prefix = QString("application/vnd.openxmlformats-package.");
    m_document_prefix = QString("application/vnd.openxmlformats-officedocument.");

    m_defaults.insert(QString("rels"), m_package_prefix + QString("relationships+xml"));
    m_defaults.insert(QString("xml"), QString("application/xml"));
}

void ContentTypes::addDefault(const QString &key, const QString &value)
{
    m_defaults.insert(key, value);
}

void ContentTypes::addOverride(const QString &key, const QString &value)
{
    m_overrides.insert(key, value);
}

void ContentTypes::addDocPropApp()
{
    addOverride(QString("/docProps/app.xml"), m_document_prefix + QString("extended-properties+xml"));
}

void ContentTypes::addDocPropCore()
{
    addOverride(QString("/docProps/core.xml"), m_package_prefix + QString("core-properties+xml"));
}

void ContentTypes::addStyles()
{
    addOverride(QString("/xl/styles.xml"), m_document_prefix + QString("spreadsheetml.styles+xml"));
}

void ContentTypes::addTheme()
{
    addOverride(QString("/xl/theme/theme1.xml"), m_document_prefix + QString("theme+xml"));
}

void ContentTypes::addWorkbook()
{
    addOverride(QString("/xl/workbook.xml"), m_document_prefix + QString("spreadsheetml.sheet.main+xml"));
}

void ContentTypes::addWorksheetName(const QString &name)
{
    addOverride(QString("/xl/worksheets/%1.xml").arg(name), m_document_prefix + QString("spreadsheetml.worksheet+xml"));
}

void ContentTypes::addChartsheetName(const QString &name)
{
    addOverride(QString("/xl/chartsheets/%1.xml").arg(name), m_document_prefix + QString("spreadsheetml.chartsheet+xml"));
}

void ContentTypes::addDrawingName(const QString &name)
{
    addOverride(QString("/xl/drawings/%1.xml").arg(name), m_document_prefix + QString("drawing+xml"));
}

void ContentTypes::addChartName(const QString &name)
{
    addOverride(QString("/xl/charts/%1.xml").arg(name), m_document_prefix + QString("drawingml.chart+xml"));
}

void ContentTypes::addCommentName(const QString &name)
{
    addOverride(QString("/xl/%1.xml").arg(name), m_document_prefix + QString("spreadsheetml.comments+xml"));
}

void ContentTypes::addTableName(const QString &name)
{
    addOverride(QString("/xl/tables/%1.xml").arg(name), m_document_prefix + QString("spreadsheetml.table+xml"));
}

void ContentTypes::addExternalLinkName(const QString &name)
{
    addOverride(QString("/xl/externalLinks/%1.xml").arg(name), m_document_prefix + QString("spreadsheetml.externalLink+xml"));
}

void ContentTypes::addSharedString()
{
    addOverride(QString("/xl/sharedStrings.xml"), m_document_prefix + QString("spreadsheetml.sharedStrings+xml"));
}

void ContentTypes::addVmlName()
{
    addOverride(QString("vml"), m_document_prefix + QString("vmlDrawing"));
}

void ContentTypes::addCalcChain()
{
    addOverride(QString("/xl/calcChain.xml"), m_document_prefix + QString("spreadsheetml.calcChain+xml"));
}

void ContentTypes::addVbaProject()
{
    //:TODO
    addOverride(QString("bin"), QString("application/vnd.ms-office.vbaProject"));
}

void ContentTypes::clearOverrides()
{
    m_overrides.clear();
}

void ContentTypes::saveToXmlFile(QIODevice *device) const
{
    QXmlStreamWriter writer(device);

    writer.writeStartDocument(QString("1.0"), true);
    writer.writeStartElement(QString("Types"));
    writer.writeAttribute(QString("xmlns"), QString("http://schemas.openxmlformats.org/package/2006/content-types"));

    {
    QMapIterator<QString, QString> it(m_defaults);
    while (it.hasNext()) {
        it.next();
        writer.writeStartElement(QString("Default"));
        writer.writeAttribute(QString("Extension"), it.key());
        writer.writeAttribute(QString("ContentType"), it.value());
        writer.writeEndElement();//Default
    }
    }

    {
    QMapIterator<QString, QString> it(m_overrides);
    while (it.hasNext()) {
        it.next();
        writer.writeStartElement(QString("Override"));
        writer.writeAttribute(QString("PartName"), it.key());
        writer.writeAttribute(QString("ContentType"), it.value());
        writer.writeEndElement(); //Override
    }
    }

    writer.writeEndElement();//Types
    writer.writeEndDocument();

}

bool ContentTypes::loadFromXmlFile(QIODevice *device)
{
    m_defaults.clear();
    m_overrides.clear();

    QXmlStreamReader reader(device);
    while (!reader.atEnd()) {
        QXmlStreamReader::TokenType token = reader.readNext();
        if (token == QXmlStreamReader::StartElement) {
            if (reader.name() == QLatin1String("Default")) {
                QXmlStreamAttributes attrs = reader.attributes();
                QString extension = attrs.value(QLatin1String("Extension")).toString();
                QString type = attrs.value(QLatin1String("ContentType")).toString();
                m_defaults.insert(extension, type);
            } else if (reader.name() == QLatin1String("Override")) {
                QXmlStreamAttributes attrs = reader.attributes();
                QString partName = attrs.value(QLatin1String("PartName")).toString();
                QString type = attrs.value(QLatin1String("ContentType")).toString();
                m_overrides.insert(partName, type);
            }
        }

        if (reader.hasError()) {
            qDebug()<<reader.errorString();
        }
    }
    return true;
}

} //namespace QXlsx

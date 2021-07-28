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

#include "xlsxdrawinganchor_p.h"
#include "xlsxdrawing_p.h"
#include "xlsxmediafile_p.h"
#include "xlsxchart.h"
#include "xlsxworkbook.h"
#include "xlsxutility_p.h"

#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QBuffer>
#include <QDir>

namespace QXlsx {

/*
     The vertices that define the position of a graphical object
     within the worksheet in pixels.

             +------------+------------+
             |     A      |      B     |
       +-----+------------+------------+
       |     |(x1,y1)     |            |
       |  1  |(A1)._______|______      |
       |     |    |              |     |
       |     |    |              |     |
       +-----+----|    OBJECT    |-----+
       |     |    |              |     |
       |  2  |    |______________.     |
       |     |            |        (B2)|
       |     |            |     (x2,y2)|
       +---- +------------+------------+

     Example of an object that covers some of the area from cell A1 to  B2.

     Based on the width and height of the object we need to calculate 8 vars:

         col_start, row_start, col_end, row_end, x1, y1, x2, y2.

     We also calculate the absolute x and y position of the top left vertex of
     the object. This is required for images.

     The width and height of the cells that the object occupies can be
     variable and have to be taken into account.
*/

//anchor

DrawingAnchor::DrawingAnchor(Drawing *drawing, ObjectType objectType)
    :m_drawing(drawing), m_objectType(objectType)
{
    m_drawing->anchors.append(this);
    m_id = m_drawing->anchors.size();//must be unique in one drawing{x}.xml file.
}

DrawingAnchor::~DrawingAnchor()
{

}

void DrawingAnchor::setObjectPicture(const QImage &img)
{
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    img.save(&buffer, "PNG");

    m_pictureFile = QSharedPointer<MediaFile>(new MediaFile(ba, QString("png"), QString("image/png")));
    m_drawing->workbook->addMediaFile(m_pictureFile);

    m_objectType = Picture;
}

void DrawingAnchor::setObjectGraphicFrame(QSharedPointer<Chart> chart)
{
    m_chartFile = chart;
    m_drawing->workbook->addChartFile(chart);

    m_objectType = GraphicFrame;
}

QPoint DrawingAnchor::loadXmlPos(QXmlStreamReader &reader)
{
    Q_ASSERT(reader.name() == QLatin1String("pos"));

    QPoint pos;
    QXmlStreamAttributes attrs = reader.attributes();
    pos.setX(attrs.value(QLatin1String("x")).toString().toInt());
    pos.setY(attrs.value(QLatin1String("y")).toString().toInt());
    return pos;
}

QSize DrawingAnchor::loadXmlExt(QXmlStreamReader &reader)
{
    Q_ASSERT(reader.name() == QLatin1String("ext"));

    QSize size;
    QXmlStreamAttributes attrs = reader.attributes();
    size.setWidth(attrs.value(QLatin1String("cx")).toString().toInt());
    size.setHeight(attrs.value(QLatin1String("cy")).toString().toInt());
    return size;
}

XlsxMarker DrawingAnchor::loadXmlMarker(QXmlStreamReader &reader, const QString &node)
{
    Q_ASSERT(reader.name() == node);

    int col = 0;
    int colOffset = 0;
    int row = 0;
    int rowOffset = 0;
    while (!reader.atEnd()) {
        reader.readNextStartElement();
        if (reader.tokenType() == QXmlStreamReader::StartElement) {
            if (reader.name() == QLatin1String("col")) {
                col = reader.readElementText().toInt();
            } else if (reader.name() == QLatin1String("colOff")) {
                colOffset = reader.readElementText().toInt();
            } else if (reader.name() == QLatin1String("row")) {
                row = reader.readElementText().toInt();
            } else if (reader.name() == QLatin1String("rowOff")) {
                rowOffset = reader.readElementText().toInt();
            }
        } else if (reader.tokenType() == QXmlStreamReader::EndElement
                   && reader.name() == node) {
            break;
        }
    }

    return XlsxMarker(row, col, rowOffset, colOffset);
}

void DrawingAnchor::loadXmlObject(QXmlStreamReader &reader)
{
    if (reader.name() == QLatin1String("sp")) {
        //Shape
        m_objectType = Shape;
        loadXmlObjectShape(reader);
    } else if (reader.name() == QLatin1String("grpSp")) {
        //Group Shape
        m_objectType = GroupShape;
        loadXmlObjectGroupShape(reader);
    } else if (reader.name() == QLatin1String("graphicFrame")) {
        //Graphic Frame
        m_objectType = GraphicFrame;
        loadXmlObjectGraphicFrame(reader);
    } else if (reader.name() == QLatin1String("cxnSp")) {
        //Connection Shape
        m_objectType = ConnectionShape;
        loadXmlObjectConnectionShape(reader);
    } else if (reader.name() == QLatin1String("pic")) {
        //Picture
        m_objectType = Picture;
        loadXmlObjectPicture(reader);
    }
}

void DrawingAnchor::loadXmlObjectConnectionShape(QXmlStreamReader &reader)
{
    Q_UNUSED(reader)
}

void DrawingAnchor::loadXmlObjectGraphicFrame(QXmlStreamReader &reader)
{
    Q_ASSERT(reader.name() == QLatin1String("graphicFrame"));

    while (!reader.atEnd()) {
        reader.readNextStartElement();
        if (reader.tokenType() == QXmlStreamReader::StartElement) {
            if (reader.name() == QLatin1String("chart")) {
                QString rId = reader.attributes().value(QLatin1String("r:id")).toString();
                QString name = m_drawing->relationships()->getRelationshipById(rId).target;
                QString path = QDir::cleanPath(splitPath(m_drawing->filePath())[0] + QLatin1String("/") + name);

                bool exist = false;
                QList<QSharedPointer<Chart> > cfs = m_drawing->workbook->chartFiles();
                for (int i=0; i<cfs.size(); ++i) {
                    if (cfs[i]->filePath() == path) {
                        //already exist
                        exist = true;
                        m_chartFile = cfs[i];
                    }
                }
                if (!exist) {
                    m_chartFile = QSharedPointer<Chart> (new Chart(m_drawing->sheet, Chart::F_LoadFromExists));
                    m_chartFile->setFilePath(path);
                    m_drawing->workbook->addChartFile(m_chartFile);
                }
            }
        } else if (reader.tokenType() == QXmlStreamReader::EndElement
                   && reader.name() == QLatin1String("graphicFrame")) {
            break;
        }
    }

    return;
}

void DrawingAnchor::loadXmlObjectGroupShape(QXmlStreamReader &reader)
{
    Q_UNUSED(reader)
}

void DrawingAnchor::loadXmlObjectPicture(QXmlStreamReader &reader)
{
    Q_ASSERT(reader.name() == QLatin1String("pic"));

    while (!reader.atEnd()) {
        reader.readNextStartElement();
        if (reader.tokenType() == QXmlStreamReader::StartElement) {
            if (reader.name() == QLatin1String("blip")) {
                QString rId = reader.attributes().value(QLatin1String("r:embed")).toString();
                QString name = m_drawing->relationships()->getRelationshipById(rId).target;
                QString path = QDir::cleanPath(splitPath(m_drawing->filePath())[0] + QLatin1String("/") + name);

                bool exist = false;
                QList<QSharedPointer<MediaFile> > mfs = m_drawing->workbook->mediaFiles();
                for (int i=0; i<mfs.size(); ++i) {
                    if (mfs[i]->fileName() == path) {
                        //already exist
                        exist = true;
                        m_pictureFile = mfs[i];
                    }
                }
                if (!exist) {
                    m_pictureFile = QSharedPointer<MediaFile> (new MediaFile(path));
                    m_drawing->workbook->addMediaFile(m_pictureFile, true);
                }
            }
        } else if (reader.tokenType() == QXmlStreamReader::EndElement
                   && reader.name() == QLatin1String("pic")) {
            break;
        }
    }

    return;
}

void DrawingAnchor::loadXmlObjectShape(QXmlStreamReader &reader)
{
    Q_UNUSED(reader)
}

void DrawingAnchor::saveXmlPos(QXmlStreamWriter &writer, const QPoint &pos) const
{
    writer.writeEmptyElement(QString("xdr:pos"));
    writer.writeAttribute(QString("x"), QString::number(pos.x()));
    writer.writeAttribute(QString("y"), QString::number(pos.y()));
}

void DrawingAnchor::saveXmlExt(QXmlStreamWriter &writer, const QSize &ext) const
{
    writer.writeStartElement(QString("xdr:ext"));
    writer.writeAttribute(QString("cx"), QString::number(ext.width()));
    writer.writeAttribute(QString("cy"), QString::number(ext.height()));
    writer.writeEndElement(); //xdr:ext
}

void DrawingAnchor::saveXmlMarker(QXmlStreamWriter &writer, const XlsxMarker &marker, const QString &node) const
{
    writer.writeStartElement(node); //xdr:from or xdr:to
    writer.writeTextElement(QString("xdr:col"), QString::number(marker.col()));
    writer.writeTextElement(QString("xdr:colOff"), QString::number(marker.colOff()));
    writer.writeTextElement(QString("xdr:row"), QString::number(marker.row()));
    writer.writeTextElement(QString("xdr:rowOff"), QString::number(marker.rowOff()));
    writer.writeEndElement();
}

void DrawingAnchor::saveXmlObject(QXmlStreamWriter &writer) const
{
    if (m_objectType == Picture)
        saveXmlObjectPicture(writer);
    else if (m_objectType == ConnectionShape)
        saveXmlObjectConnectionShape(writer);
    else if (m_objectType == GraphicFrame)
        saveXmlObjectGraphicFrame(writer);
    else if (m_objectType == GroupShape)
        saveXmlObjectGroupShape(writer);
    else if (m_objectType == Shape)
        saveXmlObjectShape(writer);
}

void DrawingAnchor::saveXmlObjectConnectionShape(QXmlStreamWriter &writer) const
{
    Q_UNUSED(writer)
}

void DrawingAnchor::saveXmlObjectGraphicFrame(QXmlStreamWriter &writer) const
{
    writer.writeStartElement(QString("xdr:graphicFrame"));
    writer.writeAttribute(QString("macro"), QString());

    writer.writeStartElement(QString("xdr:nvGraphicFramePr"));
    writer.writeEmptyElement(QString("xdr:cNvPr"));
    writer.writeAttribute(QString("id"), QString::number(m_id));
    writer.writeAttribute(QString("name"),QString("Chart %1").arg(m_id));
    writer.writeEmptyElement(QString("xdr:cNvGraphicFramePr"));
    writer.writeEndElement();//xdr:nvGraphicFramePr

    writer.writeStartElement(QString("xdr:xfrm"));
    writer.writeEndElement(); //xdr:xfrm

    writer.writeStartElement(QString("a:graphic"));
    writer.writeStartElement(QString("a:graphicData"));
    writer.writeAttribute(QString("uri"), QString("http://schemas.openxmlformats.org/drawingml/2006/chart"));

    int idx = m_drawing->workbook->chartFiles().indexOf(m_chartFile);
    m_drawing->relationships()->addDocumentRelationship(QString("/chart"), QString("../charts/chart%1.xml").arg(idx+1));

    writer.writeEmptyElement(QString("c:chart"));
    writer.writeAttribute(QString("xmlns:c"), QString("http://schemas.openxmlformats.org/drawingml/2006/chart"));
    writer.writeAttribute(QString("xmlns:r"), QString("http://schemas.openxmlformats.org/officeDocument/2006/relationships"));
    writer.writeAttribute(QString("r:id"), QString("rId%1").arg(m_drawing->relationships()->count()));

    writer.writeEndElement(); //a:graphicData
    writer.writeEndElement(); //a:graphic
    writer.writeEndElement(); //xdr:graphicFrame
}

void DrawingAnchor::saveXmlObjectGroupShape(QXmlStreamWriter &writer) const
{
    Q_UNUSED(writer)
}

void DrawingAnchor::saveXmlObjectPicture(QXmlStreamWriter &writer) const
{
    Q_ASSERT(m_objectType == Picture && !m_pictureFile.isNull());

    writer.writeStartElement(QString("xdr:pic"));

    writer.writeStartElement(QString("xdr:nvPicPr"));
    writer.writeEmptyElement(QString("xdr:cNvPr"));
    writer.writeAttribute(QString("id"), QString::number(m_id));
    writer.writeAttribute(QString("name"), QString("Picture %1").arg(m_id));

    writer.writeStartElement(QString("xdr:cNvPicPr"));
    writer.writeEmptyElement(QString("a:picLocks"));
    writer.writeAttribute(QString("noChangeAspect"), QString("1"));
    writer.writeEndElement(); //xdr:cNvPicPr

    writer.writeEndElement(); //xdr:nvPicPr

    m_drawing->relationships()->addDocumentRelationship(QString("/image"), QString("../media/image%1.%2")
                                                     .arg(m_pictureFile->index()+1)
                                                     .arg(m_pictureFile->suffix()));

    writer.writeStartElement(QString("xdr:blipFill"));
    writer.writeEmptyElement(QString("a:blip"));
    writer.writeAttribute(QString("xmlns:r"), QString("http://schemas.openxmlformats.org/officeDocument/2006/relationships"));
    writer.writeAttribute(QString("r:embed"), QString("rId%1").arg(m_drawing->relationships()->count()));
    writer.writeStartElement(QString("a:stretch"));
    writer.writeEmptyElement(QString("a:fillRect"));
    writer.writeEndElement(); //a:stretch
    writer.writeEndElement();//xdr:blipFill

    writer.writeStartElement(QString("xdr:spPr"));

    writer.writeStartElement(QString("a:prstGeom"));
    writer.writeAttribute(QString("prst"), QString("rect"));
    writer.writeEmptyElement(QString("a:avLst"));
    writer.writeEndElement(); //a:prstGeom

    writer.writeEndElement(); //xdr:spPr

    writer.writeEndElement(); //xdr:pic
}

void DrawingAnchor::saveXmlObjectShape(QXmlStreamWriter &writer) const
{
    Q_UNUSED(writer)
}

//absolute anchor

DrawingAbsoluteAnchor::DrawingAbsoluteAnchor(Drawing *drawing, ObjectType objectType)
    :DrawingAnchor(drawing, objectType)
{

}

bool DrawingAbsoluteAnchor::loadFromXml(QXmlStreamReader &reader)
{
    Q_ASSERT(reader.name() == QLatin1String("absoluteAnchor"));

    while (!reader.atEnd()) {
        reader.readNextStartElement();
        if (reader.tokenType() == QXmlStreamReader::StartElement) {
            if (reader.name() == QLatin1String("pos")) {
                pos = loadXmlPos(reader);
            } else if (reader.name() == QLatin1String("ext")) {
                ext = loadXmlExt(reader);
            } else {
                loadXmlObject(reader);
            }
        } else if (reader.tokenType() == QXmlStreamReader::EndElement
                   && reader.name() == QLatin1String("absoluteAnchor")) {
            break;
        }
    }
    return true;
}

void DrawingAbsoluteAnchor::saveToXml(QXmlStreamWriter &writer) const
{
    writer.writeStartElement(QString("xdr:absoluteAnchor"));
    saveXmlPos(writer, pos);
    saveXmlExt(writer, ext);

    saveXmlObject(writer);

    writer.writeEmptyElement(QString("xdr:clientData"));
    writer.writeEndElement(); //xdr:absoluteAnchor
}

//one cell anchor

DrawingOneCellAnchor::DrawingOneCellAnchor(Drawing *drawing, ObjectType objectType)
    :DrawingAnchor(drawing, objectType)
{

}

bool DrawingOneCellAnchor::loadFromXml(QXmlStreamReader &reader)
{
    Q_ASSERT(reader.name() == QLatin1String("oneCellAnchor"));
    while (!reader.atEnd()) {
        reader.readNextStartElement();
        if (reader.tokenType() == QXmlStreamReader::StartElement) {
            if (reader.name() == QLatin1String("from")) {
                from = loadXmlMarker(reader, QLatin1String("from"));
            } else if (reader.name() == QLatin1String("ext")) {
                ext = loadXmlExt(reader);
            } else {
                loadXmlObject(reader);
            }
        } else if (reader.tokenType() == QXmlStreamReader::EndElement
                   && reader.name() == QLatin1String("oneCellAnchor")) {
            break;
        }
    }
    return true;
}

void DrawingOneCellAnchor::saveToXml(QXmlStreamWriter &writer) const
{
    writer.writeStartElement(QString("xdr:oneCellAnchor"));

    saveXmlMarker(writer, from, QString("xdr:from"));
    saveXmlExt(writer, ext);

    saveXmlObject(writer);

    writer.writeEmptyElement(QString("xdr:clientData"));
    writer.writeEndElement(); //xdr:oneCellAnchor
}

/*
   Two cell anchor

   This class specifies a two cell anchor placeholder for a group
   , a shape, or a drawing element. It moves with
   cells and its extents are in EMU units.
*/
DrawingTwoCellAnchor::DrawingTwoCellAnchor(Drawing *drawing, ObjectType objectType)
    :DrawingAnchor(drawing, objectType)
{

}

bool DrawingTwoCellAnchor::loadFromXml(QXmlStreamReader &reader)
{
    Q_ASSERT(reader.name() == QLatin1String("twoCellAnchor"));
    while (!reader.atEnd()) {
        reader.readNextStartElement();
        if (reader.tokenType() == QXmlStreamReader::StartElement) {
            if (reader.name() == QLatin1String("from")) {
                from = loadXmlMarker(reader, QLatin1String("from"));
            } else if (reader.name() == QLatin1String("to")) {
                to = loadXmlMarker(reader, QLatin1String("to"));
            } else {
                loadXmlObject(reader);
            }
        } else if (reader.tokenType() == QXmlStreamReader::EndElement
                   && reader.name() == QLatin1String("twoCellAnchor")) {
            break;
        }
    }
    return true;
}

void DrawingTwoCellAnchor::saveToXml(QXmlStreamWriter &writer) const
{
    writer.writeStartElement(QString("xdr:twoCellAnchor"));
    writer.writeAttribute(QString("editAs"), QString("oneCell"));

    saveXmlMarker(writer, from, QString("xdr:from"));
    saveXmlMarker(writer, to, QString("xdr:to"));

    saveXmlObject(writer);

    writer.writeEmptyElement(QString("xdr:clientData"));
    writer.writeEndElement(); //xdr:twoCellAnchor
}

} // namespace QXlsx

#include <iostream>
#include <QPainter>
#include <QFont>
#include "videoShow.h"

int videoShow::currentNum = 0;

videoShow::videoShow(QWidget *parent)
    : QLabel(parent), m_currentPoint(-1, -1), m_bTemperatureStream(FALSE) {
    connect(this, SIGNAL(newFrameComing(int, DWORD)), this,
            SLOT(process(int, DWORD)));
    setMouseTracking(true);
}

void videoShow::mouseMoveEvent(QMouseEvent *event) {
    m_currentPoint = event->pos();
    repaint();
}

void videoShow::mousePressEvent(QMouseEvent *event) {
    Q_UNUSED(event);

    currentNum = m_num;
    emit mousePressed();
}

void videoShow::paintEvent(QPaintEvent *event) {
    QLabel::paintEvent(event);

    if (m_bTemperatureStream && m_currentPoint.x() >= 0) {
        const struct_CamInfo *caminfo = m_magDev.GetCamInfo();
        QSize qsize = size();
        int x = m_currentPoint.x() * caminfo->intFPAWidth / qsize.width();
        int y = m_currentPoint.y() * caminfo->intFPAHeight / qsize.height();

        int temperature =
            m_magDev.GetTemperatureProbe(x, caminfo->intFPAHeight - 1 - y, 5);
        QPainter painter(this);

        painter.setFont(QFont("arial", 12, 2));
        painter.setPen(QColor(0, 255, 0));

        x = m_currentPoint.x();
        y = m_currentPoint.y();
        if (x > qsize.width() - 30) {
            x -= 34;
        }

        if (y < 22) {
            y += 34;
        }
        painter.drawText(x, y, QString::number(temperature / 1000.0, 'f', 1));
    }
}

void videoShow::leaveEvent(QEvent *event) {
    Q_UNUSED(event);
    m_currentPoint.setX(-1);
}

void videoShow::process(int intCameraTemperature, DWORD dwCamState) {
    Q_UNUSED(intCameraTemperature);
    Q_UNUSED(dwCamState);

    m_magDev.Lock();

    const UCHAR *pData = NULL;
    const BITMAPINFO *pInfo = NULL;

    const struct_CamInfo *caminfo = m_magDev.GetCamInfo();

    BOOL bTemperatureStream = m_magDev.GetOutputBMPdata(&pData, &pInfo);
    BOOL bVideoStream = m_magDev.GetOutputVideoData(&pData, &pInfo);

    if (!bTemperatureStream && !bVideoStream) {
        //qDebug() << "Get stream failed\n";
        m_magDev.Unlock();
        return;
    }

    if (bVideoStream) {
        QImage image(caminfo->intVideoWidth, caminfo->intVideoHeight,
                     QImage::Format_RGB888);
        UCHAR *pImgData = image.bits();
        const UCHAR *pEnd =
            pImgData + caminfo->intVideoWidth * caminfo->intVideoHeight * 3;
        for (UCHAR *p = pImgData; p < pEnd;) {
            *p++ = *(pData + 2);
            *p++ = *(pData + 1);
            *p++ = *pData;
            pData += 3;
        }
        QPixmap pix = QPixmap::fromImage(image.mirrored(false, true))
                          .scaled(size(), Qt::KeepAspectRatio);
        setPixmap(pix);
    } else if (bTemperatureStream) {
        QImage image(caminfo->intVideoWidth, caminfo->intVideoHeight,
                     QImage::Format_Indexed8);
        memcpy(image.bits(), pData,
               caminfo->intVideoWidth * caminfo->intVideoHeight);

        UINT *pClrMap = (UINT *)((char *)pInfo + sizeof(BITMAPINFOHEADER));
        QVector<QRgb> colortable(256);
        for (int i = 0; i < 256; ++i) {
            colortable[i] = pClrMap[i] + 0xff000000;
        }
        image.setColorTable(colortable);

        QPixmap pix = QPixmap::fromImage(image.mirrored(false, true))
                          .scaled(size(), Qt::KeepAspectRatio);
        setPixmap(pix);
    }

    if (bTemperatureStream) {
        m_bTemperatureStream = TRUE;
    }

    m_magDev.Unlock();
}

void videoShow::newFrameEmit(int intCameraTemperature, DWORD dwCamState) {
    emit newFrameComing(intCameraTemperature, dwCamState);
}

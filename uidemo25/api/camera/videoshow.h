#ifndef VIDEOPANEL_H
#define VIDEOPANEL_H

#include <QLabel>
#include <QMouseEvent>
#include <QPoint>
#include "MagDevice.h"

class videoShow : public QLabel {
    Q_OBJECT
  public:
    explicit videoShow(QWidget *parent = 0);
    void setNum(int num) { m_num = num; }
    void newFrameEmit(int intCameraTemperature, DWORD dwCamState);
    void reset() { m_bTemperatureStream = FALSE; }

signals:
    void mousePressed();
    void newFrameComing(int intCameraTemperature, DWORD dwCamState);

  private slots:
    void process(int intCameraTemperature, DWORD dwCamState);

  private:
    virtual void mouseMoveEvent(QMouseEvent *);
    virtual void mousePressEvent(QMouseEvent *);
    virtual void paintEvent(QPaintEvent *);
    virtual void leaveEvent(QEvent *);

  public:
    static int currentNum;
    CMagDevice m_magDev;

  private:
    int m_num;
    QPoint m_currentPoint;
    BOOL m_bTemperatureStream;
};

#endif // VIDEOPANEL_H

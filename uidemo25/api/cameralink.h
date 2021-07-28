#ifndef CAMERALINK_H
#define CAMERALINK_H

#include <QObject>
#include <QVector>
#include <QImage>
#include "camera/MagService.h"
#include "videowidget.h"
#include "QsLog.h"
#include "camera/MagDevice.h"
class cameraLink:public QObject
{
    Q_OBJECT
public:
    void stop();
    bool linkCamera();
    static cameraLink *GetInstance();
    bool play(VideoWidget *w1, int c);

private:
    cameraLink();
    void refresh();
    void link();
private:

    CMagService m_magSer;// label视频标签集合
    std::vector<struct_TerminalList> m_vTermList;

};

#endif // CAMERALINK_H

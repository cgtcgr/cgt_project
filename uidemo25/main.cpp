#include "frmmain.h"
#include "appinit.h"
#include "quiwidget.h"
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AppInit::Instance()->start();
    AppInit::Instance()->initStyle(":/qss/blackvideo.css");

    //可以自己更换图标
    //QUIConfig::IconNormal = 0xf067;
    //QUIConfig::IconMax = 0xf067;
qDebug()<<"start app";
    frmMain w;
    w.show();

    return a.exec();
}

#include "frmmain.h"
#include "appinit.h"
#include "quiwidget.h"
#include "QsLog.h"
#include "task/taskmanager.h"
#include "task/periodtask.h"
#include "sql/sqlmanager.h"
#include "mount/mountnet.h"
#include <QDebug>
#include "mount/commonconfig.h"
#include <QSplashScreen>
#include <QPixmap>
#include "userframe/csplashscreen.h"
#include "userframe/animatgion.h"
#include "mount/mountalarm.h"
void initLog()
{
    using namespace QsLogging;
    // 1. init the logging mechanism
    Logger& logger = Logger::instance();
    logger.setLoggingLevel(QsLogging::TraceLevel);
    const QString sLogPath(QDir(QApplication::applicationDirPath()).filePath("log.txt"));

    // 2. add two destinations
    DestinationPtr fileDestination(DestinationFactory::MakeFileDestination(
    sLogPath, DisableLogRotation, MaxSizeBytes(1024*1024), MaxOldLogCount(2)));
    logger.addDestination(fileDestination);

    // 3. start logging
    QLOG_INFO() << "Program started";
    QLOG_INFO() << "Built with Qt" << QT_VERSION_STR << "running on" << qVersion();

}
void initServerice()
{
    QLOG_INFO() << "initServerice";
    CommonConfig::GetInstance();
    sqlManager::GetInstance()->createPatrolData();
    QLOG_INFO() << "initServerice sql";
    CTaskManager::GetInstance()->start();
    QLOG_INFO() << "initServerice task";
    MountNet::GetInstance()->setIpPort(CommonConfig::GetInstance()->mountIP,58088);
    if(MountNet::GetInstance()->init())
    {
        QLOG_INFO() << "mount connect success";
    }
    mountAlarm::GetInstance();
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap pixmap("startimg.png");
    QSplashScreen screen(pixmap);
    screen.show();
    QFont font;//实例化QFont对象
    font.setFamily("微软雅黑");//字体
    font.setPixelSize(25);//文字像素大小
    font.setPointSize(16);//文字大小
    screen.setFont(font);
    screen.showMessage("程序启动中，请稍后", Qt::AlignCenter, Qt::white);
    AppInit::Instance()->start();
    AppInit::Instance()->initStyle(":/qss/blackvideo.css");
    Sleep(1000);
    screen.showMessage("加载日志服务中", Qt::AlignCenter, Qt::white);
    Sleep(1000);
    initLog();
    screen.showMessage("正在与巡检设备建立连接", Qt::AlignCenter, Qt::white);
    Sleep(1000);
    initServerice();
    screen.showMessage("正在打开控制系统", Qt::AlignCenter, Qt::white);
    frmMain w;
    w.show();
    screen.finish(&w);


    return a.exec();
}

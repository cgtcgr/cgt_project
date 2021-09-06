#include "commonconfig.h"
#include <QDebug>
#include <QFile>
#include <QJsonObject>
#include "QsLog.h"
CommonConfig *CommonConfig::GetInstance()
{
    static CommonConfig instance;
    return &instance;
}

CommonConfig::CommonConfig()
{
    softVer= "V1.1";
    hardVer= "1";
    serialNum="1";
    mountSpeed=30;
    swingSpeed=10;
    taskTimeout=300;
    lowerPoer=20;
    mountMoveLimits=100000000;
    swingMoveLimits=2600;
    mountResetPos=10000;
    swingResetPosL=1500;
    swingResetPosR=1500;
    cameraLIP="192.168.1.241";
    cameraRIP="192.168.1.242";
    mountIP="192.168.1.240";
    storagePic="D:\bin";
    mountMotorRate=15;
    mountMotorAccTime=100;
    swingMotorRate=10;
    swingMotorAccTime=100;

    lineChartAYMin =250;
    lineChartAYMax=350;
    lineChartBYMin=50;
    lineChartBYMax=150;
    lineChartAXMax = 200;
    lineChartBXMax = 200;

    TAlarmContinueTimes = 6;
    TAlarmContinueTimeQuantum=24;

    uploadAlarmInfoTime = 30*60*1000;

    MountDebug = 0;
    readTaskInfo();
}


bool CommonConfig::readTaskInfo()
{
    QLOG_INFO()<<"PeriodTask::readTaskInfo";
    QFile file("config");
    if(!file.open(QIODevice::ReadOnly))
    {
        QLOG_INFO()<<"PeriodTask::readTaskInfo open file fail";
        return  false;
    }
    QByteArray ba = file.readAll();
    QLOG_INFO()<<"PeriodTask::readTaskInfo1"<<ba;
    QJsonParseError e;
    QJsonDocument doucment = QJsonDocument::fromJson(ba,&e);
    if(e.error == QJsonParseError::NoError && !doucment.isNull())
    {
         QLOG_INFO()<<"PeriodTask::readTaskInfo2";
       // 解析文件
        if (doucment.isObject()) { // JSON 文档为对象
            QJsonObject object = doucment.object();  // 转化为对象
            if (object.contains("softVer") &&object.value("softVer").isString()) {  // 包含指定的 key
                softVer = object.value("softVer").toString();  // 将 value 转化为字符串
                QLOG_INFO() << "softVer : " << softVer;
            }
            if (object.contains("hardVer") &&object.value("hardVer").isString()) {  // 包含指定的 key
                hardVer = object.value("hardVer").toString();  // 将 value 转化为字符串
                QLOG_INFO() << "hardVer : " << hardVer;
            }
            if (object.contains("serialNum") &&object.value("serialNum").isString()) {  // 包含指定的 key
                serialNum = object.value("serialNum").toString();  // 将 value 转化为字符串
                QLOG_INFO() << "serialNum : " << serialNum;
            }

            if (object.contains("mountSpeed") &&object.value("mountSpeed").isDouble()) {  // 包含指定的 key
                mountSpeed = object.value("mountSpeed").toInt();  // 将 value 转化为字符串
                QLOG_INFO() << "mountSpeed : " << mountSpeed;
            }
            if (object.contains("swingSpeed") &&object.value("swingSpeed").isDouble()) {  // 包含指定的 key
                swingSpeed = object.value("swingSpeed").toInt();  // 将 value 转化为字符串
                QLOG_INFO() << "swingSpeed : " << swingSpeed;
            }
            if (object.contains("taskTimeout") &&object.value("taskTimeout").isDouble()) {  // 包含指定的 key
                taskTimeout = object.value("taskTimeout").toInt();  // 将 value 转化为字符串
                QLOG_INFO() << "taskTimeout : " << taskTimeout;
            }
            if (object.contains("lowerPoer") &&object.value("lowerPoer").isDouble()) {  // 包含指定的 key
                lowerPoer = object.value("lowerPoer").toInt();  // 将 value 转化为字符串
                QLOG_INFO() << "lowerPoer : " << lowerPoer;
            }
            if (object.contains("mountMoveLimits") &&object.value("mountMoveLimits").isDouble()) {  // 包含指定的 key
                mountMoveLimits = object.value("mountMoveLimits").toInt();  // 将 value 转化为字符串
                QLOG_INFO() << "mountMoveLimits : " << mountMoveLimits;
            }

            if (object.contains("swingMoveLimits") &&object.value("swingMoveLimits").isDouble()) {  // 包含指定的 key
                swingMoveLimits = object.value("swingMoveLimits").toInt();  // 将 value 转化为字符串
                QLOG_INFO() << "swingMoveLimits : " << swingMoveLimits;
            }
            if (object.contains("mountResetPos") &&object.value("mountResetPos").isDouble()) {  // 包含指定的 key
                mountResetPos = object.value("mountResetPos").toInt();  // 将 value 转化为字符串
                QLOG_INFO() << "mountResetPos : " << mountResetPos;
            }
            if (object.contains("swingResetPosL") &&object.value("swingResetPosL").isDouble()) {  // 包含指定的 key
                swingResetPosL = object.value("swingResetPosL").toInt();  // 将 value 转化为字符串
                QLOG_INFO() << "swingResetPosL : " << swingResetPosL;
            }
            if (object.contains("swingResetPosR") &&object.value("swingResetPosR").isDouble()) {  // 包含指定的 key
                swingResetPosR = object.value("swingResetPosR").toInt();  // 将 value 转化为字符串
                QLOG_INFO() << "swingResetPosR : " << swingResetPosR;
            }

            if (object.contains("cameraLIP") &&object.value("cameraLIP").isString()) {  // 包含指定的 key
                cameraLIP = object.value("cameraLIP").toString();  // 将 value 转化为字符串
                QLOG_INFO() << "cameraLIP : " << cameraLIP;
            }
            if (object.contains("cameraRIP") &&object.value("cameraRIP").isString()) {  // 包含指定的 key
                cameraRIP = object.value("cameraRIP").toString();  // 将 value 转化为字符串
                QLOG_INFO() << "cameraRIP : " << cameraRIP;
            }
            if (object.contains("mountIP") &&object.value("mountIP").isString()) {  // 包含指定的 key
                mountIP = object.value("mountIP").toString();  // 将 value 转化为字符串
                QLOG_INFO() << "mountIP : " << mountIP;
            }
            if (object.contains("storagePic") &&object.value("storagePic").isString()) {  // 包含指定的 key
                storagePic = object.value("storagePic").toString();  // 将 value 转化为字符串
                QLOG_INFO() << "storagePic : " << storagePic;
            }

            if (object.contains("mountMotorRate") &&object.value("mountMotorRate").isDouble()) {  // 包含指定的 key
                mountMotorRate = object.value("mountMotorRate").toInt();  // 将 value 转化为字符串
                QLOG_INFO() << "mountMotorRate : " << mountMotorRate;
            }
            if (object.contains("mountMotorAccTime") &&object.value("mountMotorAccTime").isDouble()) {  // 包含指定的 key
                mountMotorAccTime = object.value("mountMotorAccTime").toInt();  // 将 value 转化为字符串
                QLOG_INFO() << "mountMotorAccTime : " << mountMotorAccTime;
            }
            if (object.contains("swingMotorRate") &&object.value("swingMotorRate").isDouble()) {  // 包含指定的 key
                swingMotorRate = object.value("swingMotorRate").toInt();  // 将 value 转化为字符串
                QLOG_INFO() << "swingMotorRate : " << swingMotorRate;
            }
            if (object.contains("swingMotorAccTime") &&object.value("swingMotorAccTime").isDouble()) {  // 包含指定的 key
                swingMotorAccTime = object.value("swingMotorAccTime").toInt();  // 将 value 转化为字符串
                QLOG_INFO() << "swingMotorAccTime : " << swingMotorAccTime;
            }
            if (object.contains("lineChartAYMin") &&object.value("lineChartAYMin").isDouble()) {  // 包含指定的 key
                lineChartAYMin = object.value("lineChartAYMin").toInt();  // 将 value 转化为字符串
                QLOG_INFO() << "lineChartAYMin : " << lineChartAYMin;
            }
            if (object.contains("lineChartAYMax") &&object.value("lineChartAYMax").isDouble()) {  // 包含指定的 key
                lineChartAYMax = object.value("lineChartAYMax").toInt();  // 将 value 转化为字符串
                QLOG_INFO() << "lineChartAYMax : " << lineChartAYMax;
            }
            if (object.contains("lineChartBYMin") &&object.value("lineChartBYMin").isDouble()) {  // 包含指定的 key
                lineChartBYMin = object.value("lineChartBYMin").toInt();  // 将 value 转化为字符串
                QLOG_INFO() << "lineChartBYMin : " << lineChartBYMin;
            }
            if (object.contains("lineChartBYMax") &&object.value("lineChartBYMax").isDouble()) {  // 包含指定的 key
                lineChartBYMax = object.value("lineChartBYMax").toInt();  // 将 value 转化为字符串
                QLOG_INFO() << "lineChartBYMax : " << lineChartBYMax;
            }
            if (object.contains("lineChartAXMax") &&object.value("lineChartAXMax").isDouble()) {  // 包含指定的 key
                lineChartAXMax = object.value("lineChartAXMax").toInt();  // 将 value 转化为字符串
                QLOG_INFO() << "lineChartAXMax : " << lineChartAXMax;
            }
            if (object.contains("lineChartBXMax") &&object.value("lineChartBXMax").isDouble()) {  // 包含指定的 key
                lineChartBXMax = object.value("lineChartBXMax").toInt();  // 将 value 转化为字符串
                QLOG_INFO() << "lineChartBXMax : " << lineChartBXMax;
            }
            if (object.contains("TAlarmContinueTimes") &&object.value("TAlarmContinueTimes").isDouble()) {  // 包含指定的 key
                TAlarmContinueTimes = object.value("TAlarmContinueTimes").toInt();  // 将 value 转化为字符串
                QLOG_INFO() << "TAlarmContinueTimes : " << TAlarmContinueTimes;
            }
            if (object.contains("TAlarmContinueTimeQuantum") &&object.value("TAlarmContinueTimeQuantum").isDouble()) {  // 包含指定的 key
                TAlarmContinueTimeQuantum = object.value("TAlarmContinueTimeQuantum").toInt();  // 将 value 转化为字符串
                QLOG_INFO() << "TAlarmContinueTimeQuantum : " << TAlarmContinueTimeQuantum;
            }
            if (object.contains("uploadAlarmInfoTime") &&object.value("uploadAlarmInfoTime").isDouble()) {  // 包含指定的 key
                uploadAlarmInfoTime = object.value("uploadAlarmInfoTime").toInt();  // 将 value 转化为字符串
                QLOG_INFO() << "uploadAlarmInfoTime : " << uploadAlarmInfoTime;
            }
            if (object.contains("MountDebug") &&object.value("MountDebug").isDouble()) {  // 包含指定的 key
                MountDebug = object.value("MountDebug").toInt();  // 将 value 转化为字符串
                QLOG_INFO() << "MountDebug : " << MountDebug;
            }
        }

        file.close();
        return true;
    }
      file.close();
      return false;
}

//"softVer":1,
//"hardVer": 1,
//"serialNum":1,
//"mountSpeed":30,
//"swingSpeed":10,
//"taskTimeout":300,
//"lowerPoer"：20,
//"mountMoveLimits"100000000,

//"swingMoveLimits":2600,
//"mountResetPos":10000,
//"swingResetPosL":1500,
//"swingResetPosR":1500,
//"cameraLIP":"192.168.1.241",
//"cameraRIP":"192.168.1.242",
//"mountIP":"192.168.1.240",
//"storagePic":"D:\Room1",
//"mountMotorRate":15,
//"mountMotorAccTime":100,
//"swingMotorRate":10
//"swingMotorAccTime":100

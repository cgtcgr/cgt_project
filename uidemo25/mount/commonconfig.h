#ifndef COMMONCONFIG_H
#define COMMONCONFIG_H

#include <QObject>
#include <QJsonParseError>
#include <QJsonDocument>
class CommonConfig
{
public:
    static CommonConfig *GetInstance();
    CommonConfig();
    QString softVer;
    QString hardVer;
    QString serialNum;

    int mountSpeed;
    int swingSpeed;
    int taskTimeout;
    int lowerPoer;
    int mountMoveLimits;
    int swingMoveLimits;
    int mountResetPos;
    int swingResetPosL;
    int swingResetPosR;
    QString cameraLIP;
    QString cameraRIP;
    QString mountIP;
    QString storagePic;
    int mountMotorRate;
    int mountMotorAccTime;
    int swingMotorRate;
    int swingMotorAccTime;
    int lineChartAYMin;
    int lineChartAYMax;
    int lineChartBYMin;
    int lineChartBYMax;

    int lineChartAXMax;
    int lineChartBXMax;

    int TAlarmContinueTimes;
    int TAlarmContinueTimeQuantum;

    int uploadAlarmInfoTime;

    int MountDebug;
private:
    bool readTaskInfo();

};

#endif // COMMONCONFIG_H
/*
"softVer":软件版本
"hardVer": 硬件版本
"serialNum":对应设备序列号

"mountSpeed":挂轨速度
"swingSpeed":摇臂速度
"taskTimeout":任务超时时间
"lowerPoer"：低电量阈值
"mountMoveLimits"： 底盘运动范围
"swingMoveLimits":  摇臂运动范围
"mountResetPos":  复位后底盘停留位置
"swingResetPosL":  复位后左摇臂停留位置
"swingResetPosR":  复位后右摇臂停留位置
"cameraLIP":  相机左IP配置
"cameraRIP":
"mountIP": 机器人IP配置
"storagePic" :"图片存储位置"
{
    "softVer": "V1.1",
    "hardVer": "1",
    "serialNum":"1",
    "mountSpeed":30,
    "swingSpeed":10,
    "taskTimeout":300,
    "lowerPoer":20,
    "mountMoveLimits":100000000,
    "swingMoveLimits":2600,
    "mountResetPos":10000,
    "swingResetPosL":1500,
    "swingResetPosR":1500,
    "cameraLIP":"192.168.1.241",
    "cameraRIP":"192.168.1.242",
    "mountIP":"192.168.1.240",
    "storagePic":"D:\Room1",
    "mountMotorRate":15,
    "mountMotorAccTime":100,
    "swingMotorRate":10,
    "swingMotorAccTime":100,
    "lineChartAYMin" :250, //A图 Y坐标 最小值
    "lineChartAYMax":350,
    "lineChartBYMin":50,
    "lineChartBYMax":150,
    "lineChartAXMax":200,//A图 X坐标 最大值
    "lineChartBXMax":200//B图 X坐标 最大值
}
*/

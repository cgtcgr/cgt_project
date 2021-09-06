#include "mountalarm.h"
#include "QsLog.h"
#include <QFile>
#include <QDateTime>
#include "commonconfig.h"
mountAlarm *mountAlarm::GetInstance()
{
    static mountAlarm instance;
    return &instance;
}

mountAlarm::mountAlarm()
{
    m_connect = false;
    cli = nullptr;
    sendTimeOut = CommonConfig::GetInstance()->uploadAlarmInfoTime;
    ser = new QTcpServer(this);
    ser->listen(QHostAddress::LocalHost,9999);
    if(!ser->listen(QHostAddress::AnyIPv4,quint16(9999))){
        return;
    }
    connect(ser,SIGNAL(newConnection()),this,SLOT(newconnected()));
    powerAlarm = new QTimer(this);
    motorAlarm = new QTimer(this);
    connect(powerAlarm ,SIGNAL(timeout()) , this , SLOT(sendPowerAlarm()));
    connect(motorAlarm ,SIGNAL(timeout()) , this , SLOT(sendMotorAlarm()));

}
void mountAlarm::newconnected()
{
    //如果服务端有一个待处理的连接，就返回真，否则返回假
    if(ser->hasPendingConnections()){
        //将下一个挂起的连接作为已连接的qtcsocket对象返回。
        cli = ser->nextPendingConnection();
        //套接字是否有效
        if(!cli->isValid()){
            return;
        }
        m_connect = true;
        QLOG_INFO()<<"mountAlarm cli connect success";
        connect(cli,SIGNAL(readyRead()),this,SLOT(recvData()));
        connect(cli,SIGNAL(disconnected()),this,SLOT(disconnected()));
    }
}
void mountAlarm::sendData(QString str)
{
    QLOG_INFO()<<"mountAlarm::sendData"<<str;
    saveFile(str);
    if(m_connect && !cli)
    {
        cli->write(str.toStdString().c_str(),str.length());
    }
    else {
        QLOG_INFO()<<"mountAlarm::sendData fail ,no cli connect";
    }
}
void mountAlarm::setLowPower(bool flag , int power)
{
    if(flag)
    {
        QString str0 = "\r\n";
        QString str1 = QString("机器人：Room1");
        QString str2 = QString("报警类型:报警");
        QString str4 = QString("报警时间:") + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        QString str5 = QString("当前电量:") + QString::number(power);
        powerStr.clear();
        powerStr = str1 + str0+ str2 +str0+ str4 +str0+ str5 +str0;
        saveFile(powerStr);
        if(!powerAlarm->isActive())
        {
            QLOG_INFO()<<"mountAlarm::setLowPower true"<<powerStr;
            powerAlarm->start(sendTimeOut);
        }
    }
    else
    {
        if(powerAlarm->isActive())
        {
            QLOG_INFO()<<"mountAlarm::setLowPower false";
            powerAlarm->stop();
        }
    }
//    2、
//    机器人：Room1
//    报警类型：低电量报警
//    报警时间：2021-08-10 21:37:18
//    当前电量： 0.0%

}
void mountAlarm::setMotorFault(bool flag)
{
    if(flag)
    {
        QString str0 = "\r\n";
        QString str1 = QString("机器人：Room1");
        QString str2 = QString("报警类型:电机故障");
        QString str4 = QString("报警时间:") + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

        motorStr.clear();
        motorStr = str1 + str0+ str2 +str0+ str4 +str0;
        saveFile(motorStr);
        if(!motorAlarm->isActive())
        {
            QLOG_INFO()<<"mountAlarm::setLowPower true"<<motorStr;
            motorAlarm->start(sendTimeOut);
        }
    }
    else
    {
        if(motorAlarm->isActive())
        {
            QLOG_INFO()<<"mountAlarm::setLowPower false";
            motorAlarm->stop();
        }
    }

}
void mountAlarm::recvData()
{
    QByteArray str = cli->readAll();

}

void mountAlarm::disconnected()
{
    m_connect = false;
    cli = nullptr;
}

void mountAlarm::saveFile(QString str)
{
    QFile file("alarminfo.txt");
    if(!file.open(QFile::WriteOnly | QFile::Append | QFile::Text)){
        return;
    }
    QTextStream out(&file);
    out << str;
    file.flush();
    file.close();
}

void mountAlarm::sendPowerAlarm()
{
    if(m_connect)
    {
       cli->write(powerStr.toStdString().c_str(),powerStr.length());
    }
}

void mountAlarm::sendMotorAlarm()
{
    if(m_connect)
    {
       cli->write(motorStr.toStdString().c_str(),motorStr.length());
    }
}

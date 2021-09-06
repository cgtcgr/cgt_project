#include "taskresult.h"
#include "QsLog.h"
#include "mount/commonconfig.h"
#include "task/periodtask.h"
#include "mount/mountalarm.h"
#include <QDateTime>
TaskResult *TaskResult::GetInstance()
{
    static TaskResult instance;
    return &instance;
}

TaskResult::TaskResult()
{
    ALARMTIMES = CommonConfig::GetInstance()->TAlarmContinueTimes;
    connect(PeriodTask::getInstance(),SIGNAL(sendTaskListChange()),this ,SLOT(recvTaskListChange()));
}

bool TaskResult::setTaskResult(QString name,QString time,int max, int min)
{
    QLOG_INFO()<<"TaskResult::setTaskResult";
    emit sendTaskResult(name , time, max, min);
    emit sendSaveTaskPic(name,time);
    return true;
}

bool TaskResult::setTaskState(QString name, QString time, int state)
{
    Q_UNUSED(state);
    emit sendTaskStart(name,time);
    return true;
}

bool TaskResult::saveResult(QString name,QString time,int max, int min)
{
    if(!m_taskNameVector.contains(name))
    {
        m_taskNameVector.push_back(name);
        singleTaskResult task;
        task.name = name;
        task.MaxT.push_back(max);
        task.MinT.push_back(min);
        task.time.push_back(time);
        m_taskResultVector.push_back(&task);
    }
    else
    {
        for(int i = 0 ; i<m_taskResultVector.size(); i++)
        {
            if(m_taskResultVector.at(i)->name == name)
            {
                if(m_taskResultVector.at(i)->MaxT.size()<ALARMTIMES)
                {
                    m_taskResultVector.at(i)->MaxT.push_back(max);
                    m_taskResultVector.at(i)->MinT.push_back(min);
                    m_taskResultVector.at(i)->time.push_back(time);
                }
                else if(m_taskResultVector.at(i)->MaxT.size() == ALARMTIMES)
                {
                    m_taskResultVector.at(i)->MaxT.pop_front();
                    m_taskResultVector.at(i)->MaxT.push_back(max);
                    m_taskResultVector.at(i)->MinT.pop_front();
                    m_taskResultVector.at(i)->MinT.push_back(min);
                    m_taskResultVector.at(i)->time.pop_front();
                    m_taskResultVector.at(i)->time.push_back(time);
                }
            }
        }
    }
    return true;
}
bool TaskResult::isTContinue(QVector<int> & T)
{
    if(T.size()!=ALARMTIMES)
    {
        return false;
    }
    for(int i = 0;i <ALARMTIMES-1;i++)
    {
        if(T.at(i)>T.at(i+1))
        {
            return false;
        }
    }
    return true;
}
bool TaskResult::isTMoreLimit(QString name,int max,int min)
{
    for(auto it = task.begin();it!=task.end();it++)
    {
        if(it->pointName == name)
        {
            if(max > it->cathodeBarThreshold)
            {
//                3、
//                机器人：Room1
//                报警类型：阈值报警
//                报警点位: 1135-B-11
//                报警时间: 2021-08-11 17:09:04
//                阈值范围:320℃
//                当前温度:2021-08-11 17:09:04 321℃
                QString str0 = "\r\n";
                QString str1 = QString("机器人：Room1");
                QString str2 = QString("报警类型:");
                QString str3 = QString("报警点位:") + name;
                QString str4 = QString("报警时间:") + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
                QString str5 = QString("阈值范围:") + QString::number(it->cathodeBarThreshold);
                QString str6 = QString("当前温度:") + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QString::number(max);
                QString str = str1 + str0+ str2 +str0+ str3 +str0+ str4 +str0+ str5 +str0+ str6;
                mountAlarm::GetInstance()->sendData(str);
           }
            if(min > it->slotThreshold)
            {
                QString str0 = "\r\n";
                QString str1 = QString("机器人：Room1");
                QString str2 = QString(QStringLiteral("报警类型"));
                QString str3 = QString(QStringLiteral("报警点位:")) + name;
                QString str4 = QString("报警时间:") + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
                QString str5 = QString("阈值范围:") + QString::number(it->slotThreshold);
                QString str6 = QString("当前温度:") + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + QString::number(min);
                QString str = str1 + str0+ str2 +str0+ str3 +str0+ str4 +str0+ str5 +str0+ str6;
                mountAlarm::GetInstance()->sendData(str);
            }
        }
    }
    return false;
}

void TaskResult::recvTaskListChange()
{
    qDebug()<<"TaskResult::recvTaskListChange";
    PeriodTask::getInstance()->getTaskList(task);
    qDebug()<<"TaskResult::recvTaskListChange"<<task.size();
}

/*
1、
机器人：Room1
报警类型: 槽底/阴极钢棒
报警点位: 1135-B-11
报警时间: 2021-08-11 17:09:04
报警温度:2021-08-11 17:09:04 74.4℃、2021-08-10 21:09:07 72.2℃、2021-08-10 19:09:12 71.8℃、2021-08-10 17:09:06 71.4℃、2021-08-10 15:08:56 70.3℃、2021-08-10 13:09:08 68.8℃
2、
机器人：Room1
报警类型：低电量报警
报警时间：2021-08-10 21:37:18
当前电量： 0.0%
3、
机器人：Room1
报警类型：阈值报警
报警点位: 1135-B-11
报警时间: 2021-08-11 17:09:04
阈值范围:320℃
当前温度:2021-08-11 17:09:04 321℃
4、
机器人：Room1
报警类型：电机故障
报警时间：2021-08-10 21:37:18
*/

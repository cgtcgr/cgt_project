#include "periodtask.h"
#include <QTime>
#include "mount/cmountstate.h"
#include "sql/sqlmanager.h"
#include <QJsonObject>
#include "QsLog/QsLog.h"
 PeriodTask::PeriodTask()
 {
     m_timer = new QTimer(this);
     connect(m_timer ,SIGNAL(timeout()),this ,SLOT(execPeriodTask()));
     m_timer->start(1000*60);
     readTaskInfo();
 }

bool PeriodTask::setExecTimeList(std::list<PeriodTask::execTime> &execTimeList)
{
    qDebug()<<"PeriodTask::setExecTimeList";
    std::unique_lock<std::mutex> lock(m_execTimeMutex);
    m_execTimeList.swap(execTimeList);
    return true;
}

bool PeriodTask::getTaskList(std::list<taskInfo> & task)
{
    qDebug()<<"PeriodTask::getTaskList";
    std::unique_lock<std::mutex> lock(m_execTimeMutex);
    task = m_taskList;
    return true;
}

bool PeriodTask::getTaskInfo(QString name,taskInfo & task)
{
    qDebug()<<"PeriodTask::getTaskInfo";
    std::unique_lock<std::mutex> lock(m_execTimeMutex);
    auto it = m_taskList.begin();
    for(; it != m_taskList.end(); it ++)
    {
        if(it->pointName == name)
        {
            task.Id = it->Id;
            task.pointName = it->pointName;
            task.pos = it->pos;
            task.cameraLeftPos = it->cameraLeftPos;
            task.cameraRightPos = it->cameraRightPos;
            return true;
        }
    }
    return false;
}

bool PeriodTask::insertTaskList(taskInfo &values)
{
    qDebug()<<"PeriodTask::insertTaskList";
    {
        std::unique_lock<std::mutex> lock(m_execTimeMutex);
        if(m_taskList.size() == 0)
        {
            values.Id = 0;
            m_taskList.push_back(values);
            listToJson();
        }
        else
        {
            int i = 0;
            auto it = m_taskList.begin();

            for(; it != m_taskList.end(); it ++)
            {
                i++;
                if(it->pos >= values.pos)
                {
                    break;
                }
            }
            values.Id = i;
            m_taskList.insert(it,values);

            int j = 0;
            it = m_taskList.begin();
            for(; it != m_taskList.end();it ++)
            {
                it->Id = j++;
            }
            listToJson();
        }
    }
    emit sendTaskListChange();
    return true;
}

bool PeriodTask::readTaskInfo()
{
    QLOG_INFO()<<"PeriodTask::readTaskInfo";
    QFile file("taskInfo.json");
    if(!file.open(QIODevice::ReadOnly))
    {
        QLOG_INFO()<<"PeriodTask::readTaskInfo open file fail";
        return  false;
    }
    QByteArray ba = file.readAll();
    QJsonParseError e;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(ba,&e);
    if(e.error == QJsonParseError::NoError && !jsonDoc.isNull())
    {
        QLOG_INFO()<<"PeriodTask::readTaskInfo data fail";
        arr = jsonDoc.array();
        jsonToList();
        file.close();
        return true;
    }
    file.close();
    return false;
}

bool PeriodTask::jsonToList()
{
    QLOG_INFO()<<"PeriodTask::jsonToList"<<arr.size();
    std::unique_lock<std::mutex> lock(m_execTimeMutex);
    for(int i = 0 ;i< arr.size(); i++)
    {
        taskInfo task;
        QJsonValue value = arr.at(i);
        task.Id = value["Id"].toInt();
        task.pointName = value["pointName"].toString();
        task.pos = value["pos"].toInt();
        task.cameraLeftPos = value["cameraLeftPos"].toInt();
        task.cameraRightPos = value["cameraRightPos"].toInt();
        task.cathodeBarThreshold = value["cathodeBarThreshold"].toInt();
        task.slotThreshold= value["slotThreshold"].toInt();
        m_taskList.push_back(task);
    }
    return true;
}

bool PeriodTask::listToJson()
{
    QLOG_INFO()<<"PeriodTask::listToJson"<<m_taskList.size();
    auto it = m_taskList.begin();
    QJsonArray taskArr;
    for(; it != m_taskList.end();it ++)
    {
        QJsonObject tjson;
        QJsonObject value;
        value.insert("Id",it->Id);
        value.insert("pointName",it->pointName);
        value.insert("pos",it->pos);
        value.insert("cameraLeftPos",it->cameraLeftPos);
        value.insert("cameraRightPos",it->cameraRightPos);
        value.insert("cathodeBarThreshold",it->cathodeBarThreshold);
        value.insert("slotThreshold",it->slotThreshold);
        tjson.insert("task",value);
        QJsonValue v = tjson["task"];
        taskArr.push_back(v);
    }
    arr.swap(taskArr);
    QJsonDocument jsondoc(arr);

    QFile file("taskInfo.json");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QFile::Truncate))
    {
        return  false;
    }
    file.write(jsondoc.toJson());
    file.close();
    return true;
}

//bool PeriodTask::insertTaskSql(int i , taskInfo &values)
//{
//    QStringList value;
//    value<<QString::number(values.Id)<<values.pointName<<QString::number(values.pos)
//            <<QString::number(values.cameraLeftPos)<<QString::number(values.cameraRightPos);
//    QStringList names;
//    names<<"id"<<"taskPointName"<<"devpos"<<"cameraLeftPos"<<"cameraRightPos";
//    //sqlManager::GetInstance()->insertData("taskInfo",names,value);
//    return true;
//}

bool PeriodTask::isPointNameExist(taskInfo &values)
{
    std::unique_lock<std::mutex> lock(m_execTimeMutex);
    auto it = m_taskList.begin();
    for(; it != m_taskList.end();it ++)
    {
        if(it->pointName == values.pointName)
        {
            break;
        }
    }
    if(it == m_taskList.end())
    {
        return false;
    }
    else
    {
        return true;
    }
}

void PeriodTask::execPeriodTask()
{
    qDebug()<<"PeriodTask::execPeriodTask";
    if(!CMountState::GetInstance()->getAutoState())
    {
        return;
    }
    QTime current_time = QTime::currentTime();

    execTime info(current_time.hour(),current_time.minute());
    qDebug()<<"PeriodTask::execPeriodTask"<<current_time.hour()<<current_time.minute();
    for(auto it = m_execTimeList.begin();it!=m_execTimeList.end();it++)
    {
        if(*it == info)
        {
           CTaskManager::GetInstance()->addTask(m_taskList);
        }
    }
}

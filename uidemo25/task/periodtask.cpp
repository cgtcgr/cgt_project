#include "periodtask.h"
#include <QTime>
#include "mount/cmountstate.h"
#include "sql/sqlmanager.h"
#include <QJsonObject>

 PeriodTask::PeriodTask()
 {
     m_timer = new QTimer(this);
     connect(m_timer ,SIGNAL(timeout()),this ,SLOT(execPeriodTask()));
     m_timer->start(1000*60);
     readTaskInfo();
 }

bool PeriodTask::setExecTimeList(std::list<PeriodTask::execTime> &execTimeList)
{
    std::unique_lock<std::mutex> lock(m_execTimeMutex);
    m_execTimeList.swap(execTimeList);
    return true;
}
bool PeriodTask::getTaskList(std::list<taskInfo> & task)
{
    std::unique_lock<std::mutex> lock(m_execTimeMutex);
    task = m_taskList;
    return true;
}

bool PeriodTask::insertTaskList(taskInfo &values)
{
    qDebug()<<"PeriodTask::insertTaskList1";
    std::unique_lock<std::mutex> lock(m_execTimeMutex);
    if(m_taskList.size() == 0)
    {
        //qDebug()<<"PeriodTask::insertTaskList2";
        values.Id = 0;
        m_taskList.push_back(values);
        listToJson();
    }
    else
    {
        //qDebug()<<"PeriodTask::insertTaskList3";
        int i = 0;
        auto it = m_taskList.begin();

        for(; it != m_taskList.end();it ++)
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
    return true;
}

bool PeriodTask::readTaskInfo()
{
    qDebug()<<"PeriodTask::readTaskInfo";
    QFile file("taskInfo.json");
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug()<<"PeriodTask::readTaskInfo open file fail";
        return  false;
    }
    QByteArray ba = file.readAll();
   // qDebug()<<"PeriodTask::readTaskInfo1"<<ba;
    QJsonParseError e;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(ba,&e);
    if(e.error == QJsonParseError::NoError && !jsonDoc.isNull())
    {
       // qDebug()<<"PeriodTask::readTaskInfo2";
        arr = jsonDoc.array();
        jsonToList();
        file.close();
        return true;
    }
//qDebug()<<"PeriodTask::readTaskInfo3";
      file.close();
      return false;

}

bool PeriodTask::jsonToList()
{
    //qDebug()<<"PeriodTask::jsonToList"<<arr.size();
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
        m_taskList.push_back(task);
    }
    return true;
}

bool PeriodTask::listToJson()
{
    qDebug()<<"PeriodTask::listToJson";
    //std::unique_lock<std::mutex> lock(m_execTimeMutex);
       qDebug()<<"PeriodTask::listToJson111";
    auto it = m_taskList.begin();
       qDebug()<<"PeriodTask::listToJson222";
    QJsonArray taskArr;
    for(; it != m_taskList.end();it ++)
    {
        qDebug()<<"PeriodTask::listToJson1";
        QJsonObject tjson;
        QJsonObject value;
        value.insert("Id",it->Id);
        value.insert("pointName",it->pointName);
        value.insert("pos",it->pos);
        value.insert("cameraLeftPos",it->cameraLeftPos);
        value.insert("cameraRightPos",it->cameraRightPos);
        tjson.insert("task",value);
        QJsonValue v = tjson["task"];
        taskArr.push_back(v);
    }
    arr.swap(taskArr);
    QJsonDocument jsondoc(arr);
    qDebug()<<"PeriodTask::listToJson2";
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
    if(!CMountState::GetInstance()->getAutoState())
    {
        return;
    }
    QTime current_time =QTime::currentTime();

    execTime info(current_time.hour(),current_time.minute());

    for(auto it = m_execTimeList.begin();it!=m_execTimeList.end();it++)
    {
        if(*it == info)
        {
           CTaskManager::GetInstance()->addTask(m_taskList);
        }
    }
}

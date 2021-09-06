#include "taskmanager.h"
#include <mount/cmountstate.h>
#include "mount/mountnet.h"
//#include "mount/steeringnet.h"
#include <windows.h>
#include "mount/tempmanager.h"
#include <QTime>
#include "task/taskresult.h"
#include "QsLog/QsLog.h"
#include "sql/sqlmanager.h"
void CTaskManager::run()
{
    while (!m_shuttingDown)
    {
        // 等待条件触发
        {          
            while (m_taskList.size() == 0)
            {
                std::unique_lock<std::mutex> _lock(m_executeTaskMutex);
                m_executeTaskCond.wait(_lock);
                if (m_shuttingDown)
                {
                    return;
                }
            }
        }
        taskInfo currentTask;
        if (!getNextTask(currentTask))
        {
            continue;
        }
        isExecuteTaskImmediate();
        // 如果待执行任务执行时间超时执行日期，则删除该任务

        //while(true)
        int errnoNo = 0;
        executeTask(currentTask , errnoNo);
    }
}

bool CTaskManager::isExecuteTaskImmediate()
{
    //判断设备是否具备可执行任务条件
    // 1 - 任务队列空
    // 2 - 低电量
    // 3 - 故障中
    // 4 - 人工干预中
    // 5 - 手持遥控模式中
    if(!CMountState::GetInstance()->getAutoState())
    {
        m_taskList.clear();
        return false;
    }
    return true;
}

bool CTaskManager::getNextTask(taskInfo& currentTask ,bool bDelete)
{
    //Q_UNUSED(bDelete);
    {
        std::unique_lock<std::mutex> lock(m_taskListMutex);

        if (m_taskList.size() == 0)
        {
            //ROS_WARN("CTaskManager getNextTask  no task");
            return false;
        }

        taskInfo front_task = m_taskList.front();
        currentTask = front_task;

        m_taskList.pop_front();
    }
    return true;
}

bool CTaskManager::executeTask(taskInfo & currentTask , int &errorNo)
{
    QLOG_INFO()<<"executeTask start"<<currentTask.pointName;
    if(!MountNet::GetInstance()->sendMovePsotion(currentTask.pos,0))
    {
        QLOG_INFO()<<"executeTask mount move fail "<<currentTask.pointName<<currentTask.pos;
        return false;
    }
    QLOG_INFO()<<"executeTask "<<currentTask.pointName<<"mount move stop";
//    if(!MountNet::GetInstance()->sendMovePsotion(currentTask.cameraLeftPos,1))
//    {
//        QLOG_INFO()<<"executeTask swing move fail"<<currentTask.pointName<<currentTask.pos;
//    }
//    if(!MountNet::GetInstance()->sendMovePsotion(currentTask.cameraRightPos,2))
//    {
//        QLOG_INFO()<<"executeTask fail 2"<<currentTask.pointName<<currentTask.pos;
//    }
    if(!MountNet::GetInstance()->sendMovePsotion(currentTask.cameraLeftPos,3,currentTask.cameraRightPos))
    {
        QLOG_INFO()<<"executeTask swing move all fail "<<currentTask.pointName<<currentTask.pos;
        return false;
    }
    QLOG_INFO()<<"executeTask "<<currentTask.pointName<<"swing move stop";
    //獲取溫度
    int maxL = 0;
    int maxR = 0;
    tempManager::getInstance()->gettem(maxL ,maxR);
    QLOG_INFO()<<"executeTask "<<currentTask.pointName<<"get tem";
    //保存結果

    QDateTime current_date_time =QDateTime::currentDateTime();
    QString time = current_date_time.toString("yyyy-MM-dd hh:mm:ss");
    TaskResult::GetInstance()->setTaskState(currentTask.pointName,time,0);

    QStringList value;
    value<<time <<currentTask.pointName  <<QString::number(maxL)<<QString::number(maxR)<<"true";
    QStringList name;
    name<<"time"<<"taskPointName"<<"upT"<<"downT"<<"result";
    sqlManager::GetInstance()->insertData("taskResult",name,value);
    QLOG_INFO()<<"executeTask "<<currentTask.pointName<<"save result";
    TaskResult::GetInstance()->setTaskResult(currentTask.pointName,time,maxL,maxR);
    QLOG_INFO()<<"executeTask stop"<<currentTask.pointName;
    if(CMountState::GetInstance()->getAutoState() && m_taskList.empty())
    {
       MountNet::GetInstance()->resetmountPos();
    }
    return true;
}

bool CTaskManager::addTask(std::list<taskInfo> &task)
{
    QLOG_INFO()<<"CTaskManager::addTask"<<task.size();
    std::unique_lock<std::mutex> lock(m_taskListMutex);
    std::list<taskInfo> ltask = task;
    m_taskList.swap(ltask);
    m_executeTaskCond.notify_all();
    return true;
}

#include "taskmanager.h"
#include <mount/cmountstate.h>
#include "mount/mountnet.h"
#include "mount/steeringnet.h"

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
                    re       turn;
                }
            }
        }

        taskInfo currentTask;
        if (!getNextTask(currentTask))
        {
            continue;
        }

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
        return false;
    }
    return true;
}

bool CTaskManager::getNextTask(taskInfo& currentTask ,bool bDelete)
{
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
    if(!MountNet::GetInstance()->sendMovePsotion(currentTask.pos))
    {
        return false;
    }
    if(!steeringNet::GetInstance(0)->WritePos(0,currentTask.cameraLeftPos,0,2000))
    {
        return false;
    }
    if(!steeringNet::GetInstance(1)->WritePos(0,currentTask.cameraRightPos,0,2000))
    {
        return false;
    }

    //获取温度成功后
    //if()
    return true;
}

bool CTaskManager::addTask(std::list<taskInfo> &task)
{
    std::unique_lock<std::mutex> lock(m_taskListMutex);
    std::list<taskInfo> ltask = task;
    m_taskList.swap(ltask);
    m_executeTaskCond.notify_all();
    return true;
}









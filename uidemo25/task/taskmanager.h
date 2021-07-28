#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QObject>
#include <QThread>
#include <mutex>
#include <condition_variable>
#include <vector>
//
class taskInfo
{
    public:
        int Id;
        QString pointName;
        int pos;
       // bool cameraLeft;
        //bool cameraRight;
        int cameraLeftPos;
        int cameraRightPos;
        int cathodeBarThreshold;
        int slotThreshold;
        void operator =(taskInfo & info)
        {
            Id = info.Id;
            pointName = info.pointName;
            pos = info.pos;
            //cameraLeft = info.cameraLeft;
           // cameraRight = info.cameraRight;
            cameraLeftPos = info.cameraLeftPos;
            cameraRightPos = info.cameraRightPos;
            cathodeBarThreshold = info.cathodeBarThreshold;
            slotThreshold = info.slotThreshold;
        }
};

class CTaskManager :public QThread
{
    public:
        static CTaskManager *GetInstance()
        {
            static CTaskManager instance;
            return &instance;
        }
        bool addTask(std::list<taskInfo> &task);

    protected:
        void run();

    private:
        bool m_isTask;
        bool m_shuttingDown;
        std::mutex m_executeTaskMutex;
        std::condition_variable m_executeTaskCond;

        /// 任务列表锁
        std::mutex m_taskListMutex;
        std::list<taskInfo>	m_taskList;

    private:
        CTaskManager(){
            m_shuttingDown = false;
        }
        ~CTaskManager(){
            m_shuttingDown = true;
            m_executeTaskCond.notify_all();
            quit();
            wait();
        }
        bool isExecuteTaskImmediate();
        bool getNextTask(taskInfo &currentTask ,bool bDelete = true);
        bool executeTask(taskInfo &currentTask, int &errorNo);
};

#endif // TASKMANAGER_H

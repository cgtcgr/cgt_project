#ifndef PERIODTASK_H
#define PERIODTASK_H

#include <QObject>
#include <list>
#include <mutex>
#include "taskmanager.h"
#include <QTimer>
#include <QJsonDocument>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
class PeriodTask :public QObject
{
    Q_OBJECT
public:
    struct execTime
    {
        uchar hour;
        uchar min;
        uchar sec;
        execTime(uchar h,uchar m)
        {
            hour = h;min = m;
        }
        bool operator ==(execTime h)
        {
            return ((hour == h.hour) && (min == h.min));
        }
        bool operator()(uchar h, uchar m)
        {
            return ((hour == h) && (abs(min - m) < 2));
        }
    };

    static PeriodTask * getInstance(){
        static PeriodTask instance;
        return &instance;
    }

    bool setExecTimeList(std::list<execTime> &execTimeList);
    bool getTaskList(std::list<taskInfo> & task);
    bool insertTaskList(taskInfo &values);
    bool isPointNameExist(taskInfo &values);
    // bool insertTaskSql(int i, taskInfo &values);
    bool readTaskInfo();
    bool getTaskInfo(QString name, taskInfo &task);
private slots:
    void execPeriodTask();
signals:
    void sendTaskListChange();
private:
    PeriodTask();
    std::mutex m_execTimeMutex;
    std::list<execTime> m_execTimeList;

    std::list<taskInfo>	m_taskList;

    QTimer * m_timer;
    QJsonArray arr;
    bool jsonToList();
    bool listToJson();
};

#endif // PERIODTASK_H

#ifndef TASKRESULT_H
#define TASKRESULT_H

#include <QObject>
#include <QVector>
#include "task/periodtask.h"
class TaskResult :public QObject
{
    Q_OBJECT
public:
    struct singleTaskResult
    {
        QString name;
        QVector<int> MaxT;
        QVector<int> MinT;
        QVector<QString> time;
    };
    static TaskResult *GetInstance();
    TaskResult();
    bool setTaskResult(QString name,QString time,int max, int min);
    bool setTaskState(QString name,QString time,int state);
public slots:
    void recvTaskListChange();
signals:
    void sendTaskResult(QString name,QString time,int max, int min);
    void sendTaskStart(QString name,QString time);
    void sendSaveTaskPic(QString name,QString time);
private:
    QVector<singleTaskResult*>  m_taskResultVector;
    QVector<QString> m_taskNameVector;
    int ALARMTIMES;
        std::list<taskInfo> task;
private:
    bool saveResult(QString name, QString time, int max, int min);
    bool isTContinue(QVector<int> &T);
    bool isTMoreLimit(QString name, int max, int min);
};

#endif // TASKRESULT_H

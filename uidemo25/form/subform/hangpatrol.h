#ifndef HANGPATROL_H
#define HANGPATROL_H

#include <QWidget>
#include "task/periodtask.h"
#include <QDebug>

namespace Ui {
class hangpatrol;
}

class hangpatrol : public QWidget
{
    Q_OBJECT

public:
    explicit hangpatrol(QWidget *parent = nullptr);
    ~hangpatrol();

public slots:
    bool updateTaskList(std::list<taskInfo> &tasklist);
    void recvTaskListChange();
    void recvTaskResult(QString name, QString time, int max, int min);
private slots:
    void on_pushButton_clicked();

private:
    Ui::hangpatrol *ui;
};

#endif // HANGPATROL_H

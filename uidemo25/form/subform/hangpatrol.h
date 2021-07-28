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
private:
    Ui::hangpatrol *ui;
};

#endif // HANGPATROL_H

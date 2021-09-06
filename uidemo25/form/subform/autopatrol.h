#ifndef AUTOPATROL_H
#define AUTOPATROL_H

#include <QWidget>

namespace Ui {
class autoPatrol;
}

class autoPatrol : public QWidget
{
    Q_OBJECT
public:
    explicit autoPatrol(QWidget *parent = nullptr);
    ~autoPatrol();
public slots:
    void recvTaskResult(QString name,QString time,int max, int min);
    void recvTaskStart(QString name, QString time);
private:
    Ui::autoPatrol *ui;
    bool updateTaskList(QString name, QString time, int max, int min);
};

#endif // AUTOPATROL_H

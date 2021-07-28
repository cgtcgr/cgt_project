#ifndef FRMDATALOG_H
#define FRMDATALOG_H

#include <QWidget>
#include "task/taskmanager.h"
#include "task/periodtask.h"
#include "QTableWidgetItem"
namespace Ui {
class frmDataLog;
}

class frmDataLog : public QWidget
{
    Q_OBJECT

public:
    explicit frmDataLog(QWidget *parent = 0);
    ~frmDataLog();
    bool updateTaskList(std::list<taskInfo> &tasklist);

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_m_addPointBtn_clicked();

private:
    Ui::frmDataLog *ui;
};

#endif // FRMDATALOG_H

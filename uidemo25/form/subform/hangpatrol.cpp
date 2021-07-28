#include "hangpatrol.h"
#include "ui_hangpatrol.h"

hangpatrol::hangpatrol(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::hangpatrol)
{
    ui->setupUi(this);
    std::list<taskInfo> task;
    PeriodTask::getInstance()->getTaskList(task);
    updateTaskList(task);
}

hangpatrol::~hangpatrol()
{
    delete ui;
}
bool hangpatrol::updateTaskList(std::list<taskInfo>& tasklist)
{
    qDebug()<<"hangpatrol::updateTaskList"<<tasklist.size();
    ui->tableWidget->clearContents();
    auto it = tasklist.begin();
    int i = 0;
    ui->tableWidget->setRowCount(tasklist.size());
    for(; it != tasklist.end();it ++)
    {
        qDebug()<<"hangpatrol::updateTaskList";
        QTableWidgetItem *name1= new QTableWidgetItem(it->pointName);
        ui->tableWidget->setItem(i,0,name1);
        QTableWidgetItem *name2=new QTableWidgetItem(QString::number(it->pos));
        ui->tableWidget->setItem(i++,1,name2);
    }
    ui->tableWidget->show();
    return true;
}

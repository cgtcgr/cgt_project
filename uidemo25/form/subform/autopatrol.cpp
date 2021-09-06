#include "autopatrol.h"
#include "ui_autopatrol.h"
#include "task/taskresult.h"
#include <QDebug>
autoPatrol::autoPatrol(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::autoPatrol)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnWidth(1, 150);
    ui->tableWidget->setColumnWidth(0, 70);
    ui->tableWidget->setColumnWidth(2, 75);
    ui->tableWidget->setColumnWidth(3, 75);

       // connect(PeriodTask::getInstance(),SIGNAL(sendTaskListChange()),this ,SLOT(recvTaskListChange()));
    connect(TaskResult::GetInstance(),SIGNAL(sendTaskResult(QString,QString,int , int ))
            ,this,SLOT( recvTaskResult(QString ,QString ,int , int )));
    connect(TaskResult::GetInstance(),SIGNAL(sendTaskStart(QString,QString))
            ,this,SLOT( recvTaskStart(QString ,QString )));
}

autoPatrol::~autoPatrol()
{
    delete ui;
}

void autoPatrol::recvTaskResult(QString name,QString time,int max, int min)
 {
    qDebug()<<"autoPatrol::recvTaskResult";
    updateTaskList(name,time,max, min);
 }
bool autoPatrol::updateTaskList(QString name,QString time,int max, int min)
{
    qDebug()<<"autoPatrol::updateTaskList"<<name<<time<<max<<min;
    int rowIndex = ui->tableWidget->rowCount();
    qDebug()<<"autoPatrol::updateTaskList"<<name<<time<<max<<min<<rowIndex;
     ui->tableWidget->setRowCount(rowIndex + 1);//总行数增加1

    QTableWidgetItem *name1= new QTableWidgetItem(name);
    ui->tableWidget->setItem(rowIndex,0,name1);
    QTableWidgetItem *name2=new QTableWidgetItem(time);
    ui->tableWidget->setItem(rowIndex,1,name2);
    QTableWidgetItem *name3= new QTableWidgetItem(QString::number((double)max/1000.0, 'f', 1));
    ui->tableWidget->setItem(rowIndex,2,name3);
    QTableWidgetItem *name4=new QTableWidgetItem(QString::number((double)min/1000.0, 'f', 1));
    ui->tableWidget->setItem(rowIndex,3,name4);

    ui->tableWidget->show();
    return true;
}
void autoPatrol::recvTaskStart(QString name,QString time)
{
    ui->textEdit->setText(name + " are patroling");
}

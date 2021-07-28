#include "frmdatalog.h"
#include "ui_frmdatalog.h"
#include <QTime>
#include <QDebug>
#include <QMessageBox>
#include "task/periodtask.h"
frmDataLog::frmDataLog(QWidget *parent) : QWidget(parent), ui(new Ui::frmDataLog)
{
         qDebug()<<"frmDataLog init success11";
    ui->setupUi(this);
    ui->timeEdit->setDisplayFormat("HH:mm");
    ui->timeEdit_2->setDisplayFormat("HH:mm");
    std::list<taskInfo> task;

    PeriodTask::getInstance()->getTaskList(task);
    qDebug()<<"frmDataLog init success1";
    updateTaskList(task);
    qDebug()<<"frmDataLog init success";

}
//QTimeEdit
frmDataLog::~frmDataLog()
{
    delete ui;
}

void frmDataLog::on_pushButton_2_clicked()
{
    QTime time = ui->timeEdit->time();
    QString strTime = time.toString("HH:mm");
    int index = ui->tableWidget_2->rowCount();
    QTableWidgetItem *item = new QTableWidgetItem(QString::number(index+1));
    ui->tableWidget_2->setItem(index+1,0,item);
    item = new QTableWidgetItem(strTime);
    ui->tableWidget_2->setItem(index+1,1,item);
}

void frmDataLog::on_pushButton_3_clicked()
{
   // qDebug()<<"frmDataLog::on_pushButton_3_clicked";
    QTime time = ui->timeEdit_2->time();
    QString strTime = time.toString("HH:mm");
   // qDebug()<<"frmDataLog::on_pushButton_3_clicked"<<strTime;
    int index = ui->tableWidget_2->rowCount();
    ui->tableWidget_2->setRowCount(++index);
     //qDebug()<<"frmDataLog::on_pushButton_3_clicked"<<strTime<<index;
    QTableWidgetItem *item = new QTableWidgetItem();
    item->setText("cgt");
    ui->tableWidget_2->setItem(index,0,item);
}

void frmDataLog::on_m_addPointBtn_clicked()
{
    //获取挂轨水平垂直位置
    QString pointName = ui->lineEdit_2->text();
    if(pointName.isEmpty())
    {
        QMessageBox::information(this, "warn", "please input pointName");
    }
    int devPos = 88;
    int cameraLP = 99;
    int cameraRP = 100;
    taskInfo task;
    task.pointName = pointName;
    task.pos = devPos;
    task.cameraLeftPos = cameraLP;
    task.cameraRightPos = cameraRP;
    if(PeriodTask::getInstance()->isPointNameExist(task))
    {
        QMessageBox::information(this, "warn", "point is exist");
    }
    else
    {
        if(PeriodTask::getInstance()->insertTaskList(task))
        {
            std::list<taskInfo> task;
            PeriodTask::getInstance()->getTaskList(task);
            updateTaskList(task);
        }
    }
}

bool frmDataLog::updateTaskList(std::list<taskInfo>& tasklist)
{
    //qDebug()<<"frmDataLog::updateTaskList"<<tasklist.size();
    ui->tableWidget->clearContents();
    auto it = tasklist.begin();
    int i = 0;
    ui->tableWidget->setRowCount(tasklist.size());
    for(; it != tasklist.end();it ++)
    {
        QTableWidgetItem *name1= new QTableWidgetItem(it->pointName);
        ui->tableWidget->setItem(i,0,name1);
        QTableWidgetItem *name2=new QTableWidgetItem(QString::number(it->pos));
        ui->tableWidget->setItem(i++,1,name2);
    }
    ui->tableWidget->show();
    return true;
}

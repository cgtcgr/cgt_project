#include "hangpatrol.h"
#include "ui_hangpatrol.h"
#include "mount/cmountstate.h"
#include <list>
#include "task/taskmanager.h"
#include "task/taskresult.h"
hangpatrol::hangpatrol(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::hangpatrol)
{
    ui->setupUi(this);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);		//设置整行选择
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);		//设置只选择一行

    connect(PeriodTask::getInstance(),SIGNAL(sendTaskListChange()),this ,SLOT(recvTaskListChange()));
    std::list<taskInfo> task;
    PeriodTask::getInstance()->getTaskList(task);
    updateTaskList(task);
    connect(TaskResult::GetInstance(),SIGNAL(sendTaskResult(QString,QString,int , int ))
            ,this,SLOT( recvTaskResult(QString ,QString ,int , int )));
}

hangpatrol::~hangpatrol()
{
    delete ui;
}

void hangpatrol::recvTaskResult(QString name,QString time,int max, int min)
 {
    qDebug()<<"hangpatrol::recvTaskResult";
    if(CMountState::GetInstance()->getAutoState())
    {
        return;
    }
    //double maxt = (double)maxTem/1000.0 ;
   // QString str = QString::number((double)max/1000.0, 'f', 1);
    ui->lineEdit->setText(QString::number((double)max/1000.0, 'f', 1));
    ui->lineEdit_2->setText(QString::number((double)min/1000.0, 'f', 1));

 }
void hangpatrol::recvTaskListChange()
{
    qDebug()<<"hangpatrol::recvTaskListChange";
    std::list<taskInfo> task;
    PeriodTask::getInstance()->getTaskList(task);
    updateTaskList(task);
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
        QString pos = QString::number(it->pos) + ";" + QString::number(it->cameraLeftPos) + ";" + QString::number(it->cameraRightPos);
        QTableWidgetItem *name1= new QTableWidgetItem(it->pointName);
        ui->tableWidget->setItem(i,0,name1);
        QTableWidgetItem *name2=new QTableWidgetItem(pos);
        ui->tableWidget->setItem(i++,1,name2);
    }
    ui->tableWidget->show();
    return true;
}

void hangpatrol::on_pushButton_clicked()
{
    if(CMountState::GetInstance()->getAutoState())
    {
        return;
    }

    QList<QTableWidgetItem*> items = ui->tableWidget->selectedItems();
    QTableWidgetItem *item = items.at(0);
    QString text = item->text(); //获取内容
    qDebug()<<text;
    taskInfo task;
     if(!PeriodTask::getInstance()->getTaskInfo(text,task))
     {
        // QLOG_INFO()<<"can not find task";
        return;
     }
     std::list<taskInfo>  taskList;
     taskList.push_back(task);
     CTaskManager::GetInstance()->addTask(taskList);
}

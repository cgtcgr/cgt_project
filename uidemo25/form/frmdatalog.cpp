#include "frmdatalog.h"
#include "ui_frmdatalog.h"
#include <QTime>
#include <QDebug>
#include <QMessageBox>
#include "task/periodtask.h"
#include "mount/mountnet.h"
#include "QsLog/QsLog.h"
#include <list>
#include "userframe/frmessagebox.h"
#include "mount/commonconfig.h"
#include <QPropertyAnimation>
#include "userframe/animatgion.h"
frmDataLog::frmDataLog(QWidget *parent) : QWidget(parent), ui(new Ui::frmDataLog)
{
    qDebug()<<"frmDataLog init success11";
    ui->setupUi(this);
    m_selectMode = false;
    ui->timeEdit_2->setDisplayFormat("HH:mm");
    std::list<taskInfo> task;
    PeriodTask::getInstance()->getTaskList(task);
    updateTaskList(task);
    readTaskInfo();
}

frmDataLog::~frmDataLog()
{
    delete ui;
}

void frmDataLog::on_pushButton_2_clicked()
{
    m_selectMode = true;
    QString text = ui->lineEdit->text();
    std::list<PeriodTask::execTime> execTimeList;
    ui->tableWidget_2->clearContents();
     ui->tableWidget_2->setRowCount(24);

    int index = 0;
    QStringList times;
    for(int i = 0; i< 24 ; i= i+text.toInt())
    {
        qDebug()<<"on_pushButton_2_clicked"<<i;
        //ui->tableWidget_2->setRowCount(ui->tableWidget_2->rowCount()+1);
        QString time = QString::number(i) + ":00";
        times<<time;
        QTableWidgetItem *name1= new QTableWidgetItem(time);
        ui->tableWidget_2->setItem(index,0,name1);
        index++;
        PeriodTask::execTime exec(i,0);
        execTimeList.push_back(exec);
    }
    updateTimefile(times);
    if(PeriodTask::getInstance()->setExecTimeList(execTimeList))
    {

    }
    ui->tableWidget_2->show();
}

void frmDataLog::on_pushButton_3_clicked()
{
    if(m_selectMode)
    {
        ui->tableWidget->clearContents();
        ui->tableWidget_2->setRowCount(0);
        m_selectMode = false;
    }
    QTime time = ui->timeEdit_2->time();
    QString strTime = time.toString("HH:mm");
    qDebug()<<strTime;
    int index = ui->tableWidget_2->rowCount();
    ui->tableWidget_2->setRowCount(++index);
    QTableWidgetItem *name1= new QTableWidgetItem(strTime);
    ui->tableWidget_2->setItem(index-1,0,name1);

}
void frmDataLog::on_pushButton_clicked()
{
    std::list<PeriodTask::execTime> execTimeList;
    int rows = ui->tableWidget_2->rowCount();
    qDebug()<<rows;
    QStringList times;
    for(int i=0; i<rows; i++)
    {
        QString tableText = ui->tableWidget_2->item(i,0)->text();
        times<<tableText;
        qDebug()<<tableText;
        QStringList value = tableText.split(":");
        //qDebug()<<value;
        if(value.size()==2)
        {
            PeriodTask::execTime exec(value.at(0).toInt(),value.at(1).toInt());
            execTimeList.push_back(exec);
        }
    }
    updateTimefile(times);
    if(PeriodTask::getInstance()->setExecTimeList(execTimeList))
    {

    }
}
void frmDataLog::on_m_addPointBtn_clicked()
{
    //获取挂轨水平垂直位置
    QString pointName = ui->lineEdit_2->text();
    QString s1 = ui->lineEdit_4->text();
    QString s2 = ui->lineEdit_3->text();
    if(pointName.isEmpty())
    {
        QMessageBox::information(this, "warn", "please input pointName");
    }
    int devPos = MountNet::GetInstance()->getMountPos();
    int cameraLP = MountNet::GetInstance()->getSwingAPos();
    int cameraRP = MountNet::GetInstance()->getSwingBPos();
    if((devPos>CommonConfig::GetInstance()->mountMoveLimits) || (cameraLP>devPos>CommonConfig::GetInstance()->swingMoveLimits)
            || (cameraRP > CommonConfig::GetInstance()->swingMoveLimits))
    {
         QMessageBox::information(this, "警告", "超过限位");
    }
    taskInfo task;
    task.pointName = pointName;
    task.pos = devPos;
    task.cameraLeftPos = cameraLP;
    task.cameraRightPos = cameraRP;
    task.cathodeBarThreshold = s1.toInt();
    task.slotThreshold = s2.toInt();
    if(PeriodTask::getInstance()->isPointNameExist(task))
    {
        QMessageBox::information(this, "警告", "点位已存在");
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
    QLOG_INFO()<<"frmDataLog::updateTaskList"<<tasklist.size();
    ui->tableWidget->clearContents();
    auto it = tasklist.begin();
    int i = 0;
    ui->tableWidget->setRowCount(tasklist.size());
    for(; it != tasklist.end();it ++)
    {
        QTableWidgetItem *name1= new QTableWidgetItem(it->pointName);
        ui->tableWidget->setItem(i,0,name1);
        QString pos = QString::number(it->pos) + ";" + QString::number(it->cameraLeftPos) + ";" + QString::number(it->cameraRightPos);
        QTableWidgetItem *name2=new QTableWidgetItem(pos);
        ui->tableWidget->setItem(i++,1,name2);
    }
    ui->tableWidget->show();
    return true;
}

bool frmDataLog::readTaskInfo()
{
    QLOG_INFO()<<"PeriodTask::readTaskInfo";
    QFile file("taskSchduleInfo.json");
    if(!file.open(QIODevice::ReadOnly))
    {
        QLOG_INFO()<<"PeriodTask::readTaskInfo open file fail";
        return  false;
    }
    QByteArray ba = file.readAll();
    QJsonParseError e;
    QJsonDocument doucment = QJsonDocument::fromJson(ba,&e);
    if(e.error == QJsonParseError::NoError && !doucment.isNull())
    {

        if (doucment.isObject()) { // JSON 文档为对象
            QJsonObject object = doucment.object();  // 转化为对象
            if (object.contains("timeInfo") &&object.value("timeInfo").isString()) {  // 包含指定的 key
                QString timeInfo = object.value("timeInfo").toString();  // 将 value 转化为字符串
                QLOG_INFO() << "timeInfo : " << timeInfo;

                QStringList times = timeInfo.split(";");
                updateTimeTable(times);
                on_pushButton_clicked();
            }
        }
        file.close();
        return true;
    }
      file.close();
      return false;
}

void frmDataLog::updateTimeTable( QStringList& times)
{
    auto it = times.begin();
    ui->tableWidget_2->setRowCount(times.size());
    int index = 0;
    for(;it != times.end();it++)
    {
        QTableWidgetItem *name1= new QTableWidgetItem(*it);
        ui->tableWidget_2->setItem(index,0,name1);
        index++;
    }
}
void frmDataLog::updateTimefile( QStringList& times)
{
    auto it = times.begin();
    QString time;
    for(; it != times.end();it ++)
    {
        time = time + *it + ";";
    }
    QJsonObject value;
    value.insert("timeInfo",time);

    QJsonDocument jsondoc(value);

    QFile file("taskSchduleInfo.json");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QFile::Truncate))
    {
        return;
    }
    file.write(jsondoc.toJson());
    file.close();
    return;
}

void frmDataLog::on_tabWidget_currentChanged(int index)
{
    if(index == 0)
    {
       animatgion::wigShow(ui->tab,0);
       //animatgion::wigShow(ui->tab,1,ui->tab_2,1);
    }
    else if(index == 1)
    {
        animatgion::wigShow(ui->tab_2,1);
         //animatgion::wigShow(ui->tab_2,0,ui->tab,1);
    }
}

//#define ANIMATION_COVER
//void frmDataLog::wigShow(QWidget *wig, int direct)
//{
//int w = wig->size().width();
//int h = wig->size().height();
//int timeVal = 500;
//#ifdef ANIMATION_COVER //覆盖效果
//    int startx = (direct > 0)? -w : w;
//    QPropertyAnimation *animal1= new QPropertyAnimation(wig,"pos");
//    animal1->setDuration(timeVal);
//    animal1->setStartValue(QPoint(startx,0));
//    animal1->setEndValue(QPoint(0,0));
//    animal1->setEasingCurve(QEasingCurve::OutCubic);
//    animal1->start();
//#endif
//#ifdef ANIMATION_SLIDE //两个界面同时平推
//    int startx = (direct > 0)? -w : w;

//    tmpWig = content->childAt(0,0);
//    QPropertyAnimation *animal1= new QPropertyAnimation(wig,"pos");
//    animal1->setDuration(timeVal);
//    animal1->setStartValue(QPoint(startx,0));
//    animal1->setEndValue(QPoint(0,0));
//    animal1->setEasingCurve(QEasingCurve::OutCubic);

//    QPropertyAnimation *animal2= new QPropertyAnimation(tmpWig,"pos");
//    animal2->setDuration(timeVal);
//    animal2->setStartValue(QPoint(0,0));
//    animal2->setEndValue(QPoint(-startx,0));
//    animal2->setEasingCurve(QEasingCurve::OutCubic);

//    QParallelAnimationGroup *group = new QParallelAnimationGroup;
//    group->addAnimation(animal1);
//    group->addAnimation(animal2);
//    group->start();
//    wig->raise();
//#endif
//#ifdef ANIMATION_ZUO1
//    if(direct > 0){//从左上角慢慢变大出现
//        QPropertyAnimation *animal1= new QPropertyAnimation(wig,"size");
//        animal1->setDuration(timeVal);
//        animal1->setStartValue(QSize(0,0));
//        animal1->setEndValue(QSize(w,h));
//        animal1->setEasingCurve(QEasingCurve::OutCubic);
//        wig->raise();
//        animal1->start();
//    }else{  //右下角慢慢变大出现
//        QPropertyAnimation *animal1= new QPropertyAnimation(wig,"geometry");
//        animal1->setDuration(timeVal);
//        animal1->setStartValue(QRect(w,h, 0, 0));
//        animal1->setEndValue(QRect(0,0, w, h));
//        animal1->setEasingCurve(QEasingCurve::OutCubic);
//        wig->raise();
//        animal1->start();
//    }
//#endif
//#ifdef ANIMATION_ZUO2
//    if(direct > 0){//从左上角慢慢变大出现
//        QPropertyAnimation *animal1= new QPropertyAnimation(wig,"size");
//        animal1->setDuration(timeVal);
//        animal1->setStartValue(QSize(0,0));
//        animal1->setEndValue(QSize(w,h));
//        animal1->setEasingCurve(QEasingCurve::OutCubic);
//        wig->raise();
//        animal1->start();
//    }else{  //覆盖层从右下角开始慢慢消失显示出要显示的界面
//        QWidget *tmp = content->childAt(w-1, h-1);//这个地方必须等到上个动画完毕,否则偶尔会出现混乱
//        if(tmp == NULL){
//            qDebug()<<"no wig";
//            return;
//        }
//        wig->stackUnder(tmp);  //把要显示的wig放到当前显示的下面
//        QPropertyAnimation *animal1= new QPropertyAnimation(tmp,"size");
//        animal1->setDuration(timeVal);
//        animal1->setStartValue(QSize(w,h));
//        animal1->setEndValue(QSize(0,0));
//        animal1->setEasingCurve(QEasingCurve::OutCubic);
//        animal1->start();
//    }
//#endif
//#ifdef ANIMATION_CENTER
//    QPropertyAnimation *animal1= new QPropertyAnimation(wig,"geometry");
//    animal1->setDuration(timeVal);
//    animal1->setStartValue(QRect(w/2,h/2, 0, 0));
//    animal1->setEndValue(QRect(0,0, w, h));
//    wig->raise();
//    animal1->start();
//#endif
//#ifdef ANIMATION_CENTER2
//    QPropertyAnimation *animal1= new QPropertyAnimation(wig,"geometry");
//    animal1->setDuration(timeVal);
//    animal1->setStartValue(QRect(w/2,h/2, 0, 0));
//    animal1->setEndValue(QRect(0,0, w, h));
//    animal1->setEasingCurve(QEasingCurve::OutCubic);
//    wig->raise();
//    animal1->start();
//#endif
//#ifdef ANIMATION_DUIJIAO //对角变化
//    tmpWig = content->childAt(0,0);
//    QPropertyAnimation *animal1= new QPropertyAnimation(wig,"geometry");
//    animal1->setDuration(timeVal);
//    (direct > 0)? animal1->setStartValue(QRect(0,0, 0, 0)):animal1->setStartValue(QRect(w,h, 0, 0));
//    (direct > 0)? animal1->setEndValue(QRect(0,0, w, h)):animal1->setEndValue(QRect(0,0, w, h));

//    QPropertyAnimation *animal2= new QPropertyAnimation(tmpWig,"geometry");
//    animal2->setDuration(timeVal);
//    (direct > 0)? animal2->setStartValue(QRect(0,0, w, h)):animal2->setStartValue(QRect(0,0, w, h));
//    (direct > 0)? animal2->setEndValue(QRect(w,h, 0, 0)):animal2->setEndValue(QRect(0,0, 0, 0));

//    QParallelAnimationGroup *group = new QParallelAnimationGroup;
//    group->addAnimation(animal1);
//    group->addAnimation(animal2);
//    group->start();
//    wig->raise();
//#endif
//}

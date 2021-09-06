#include "mountinfoshow.h"
#include "ui_mountinfoshow.h"
#include "mount/commonconfig.h"
#include "QsLog.h"
#include "mount/mountalarm.h"
mountInfoShow::mountInfoShow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mountInfoShow),
    X(0),
    Y(0),
    Z(0),
    nErrerMotor(0),
    powerRate(100),
    A(0),
    lowPowerAlarm(false),
    motorAlarm(false)
{
    ui->setupUi(this);
}

mountInfoShow::~mountInfoShow()
{
    delete ui;
}

void mountInfoShow::setAuto(bool flag)
{
    if(flag)
    {
        ui->label_9->setText(QString("自动"));
    }
    else
    {
        ui->label_9->setText(QString("手动"));
    }
}
void mountInfoShow::setInfo(int x, int y, int z, uchar errM, uchar power, short a)
{
    X= x;Y= y;Z=z;nErrerMotor = errM;powerRate=power;A=a;
    ui->lineEdit->setText(QString::number(x));
    ui->lineEdit_2->setText(QString::number(y));
    ui->lineEdit_3->setText(QString::number(z));
    ui->lineEdit_4->setText(QString::number(power));
    ui->lineEdit_5->setText(QString::number(a));

    if(A>0)
    {
        ui->label_6->setText(QString("充电"));
    }
    else if(A==0)
    {
        ui->label_6->setText(QString("关机"));
    }
    else if(A<0)
    {
        ui->label_6->setText(QString("运行"));
    }
    if(powerRate < CommonConfig::GetInstance()->lowerPoer)
    {
        if(!lowPowerAlarm)
        {
            lowPowerAlarm = true;
            mountAlarm::GetInstance()->setLowPower(lowPowerAlarm,powerRate);
        }
        ui->label_7->setText(QString("电量不足"));
    }
    else
    {
        if(lowPowerAlarm)
        {
            lowPowerAlarm = false;
            mountAlarm::GetInstance()->setLowPower(lowPowerAlarm,powerRate);
        }
        ui->label_7->setText(QString("电量充足"));
    }
    if(nErrerMotor)
    {
        if(!motorAlarm)
        {
            motorAlarm = true;
            mountAlarm::GetInstance()->setMotorFault(motorAlarm);
        }
        ui->label_8->setText(QString("电机故障"));
    }
    else
    {
        if(motorAlarm)
        {
            motorAlarm = false;
            mountAlarm::GetInstance()->setMotorFault(motorAlarm);
        }
        ui->label_8->setText(QString("电机正常"));
    }
}

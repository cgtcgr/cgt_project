#include "robotcontrol.h"
#include "ui_robotcontrol.h"
#include <QDebug>

static const int RATE = 200;
static const int MOSTSPEED = 300;
static const int MOSTANGLESPEED = 20000;
static const int ACCSPEED = 60;
static const int ACCANGLESPEED = 5000;

robotControl::robotControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::robotControl)
{
    ui->setupUi(this);
    m_devFront = new QTimer(this);
    m_devBack = new QTimer(this);

    connect(m_devFront, SIGNAL(timeout()), this, SLOT(updateMoveForwad()));
    connect(m_devBack, SIGNAL(timeout()), this, SLOT(updateMoveBack()));

}

robotControl::~robotControl()
{
    delete ui;
}

void robotControl::on_front_btn_clicked()
{
    QLOG_INFO()<<"on_front_btn_clicked";
    if(CMountState::GetInstance()->getAutoState())
    {
        return;
    }

    MountNet::GetInstance()->sendMoveMount(true);
}

void robotControl::on_back_btn_clicked()
{
    if(CMountState::GetInstance()->getAutoState())
    {
        return;
    }
    QLOG_INFO()<<"on_back_btn_clicked";
    MountNet::GetInstance()->sendMoveMount(false);
}

void robotControl::on_up_btn_clicked()
{
    if(CMountState::GetInstance()->getAutoState())
    {
        return;
    }
    QLOG_INFO()<<"on_up_btn_clicked";
     MountNet::GetInstance()->sendMoveSwingA(true);
}

void robotControl::on_down_btn_clicked()
{
    if(CMountState::GetInstance()->getAutoState())
    {
        return;
    }
    QLOG_INFO()<<"on_down_btn_clicked";
    MountNet::GetInstance()->sendMoveSwingA(false);
}

void robotControl::on_up1_btn_clicked()
{
    if(CMountState::GetInstance()->getAutoState())
    {
        return;
    }
    QLOG_INFO()<<"on_up1_btn_clicked";
     // steeringNet::GetInstance(1)->MoveFrontStep();
    MountNet::GetInstance()->sendMoveSwingB(true);
}

void robotControl::on_down1_btn_clicked()
{
    if(CMountState::GetInstance()->getAutoState())
    {
        return;
    }
    QLOG_INFO()<<"on_down1_btn_clicked";
     MountNet::GetInstance()->sendMoveSwingB(false);
}


void robotControl::on_hand_btn_clicked()
{
    QLOG_INFO()<<"on_hand_btn_clicked";
    CMountState::GetInstance()->setAutoState(false);
}

void robotControl::on_auto_btn_clicked()
{
    QLOG_INFO()<<"on_auto_btn_clicked";
    CMountState::GetInstance()->setAutoState(true);
}

//车体控制


void robotControl::updateMoveForwad()
{
    if(CMountState::GetInstance()->getAutoState())
    {
        return;
    }

     MountNet::GetInstance()->sendMoveMount(true);

}

void robotControl::updateMoveBack()
{
    if(CMountState::GetInstance()->getAutoState())
    {
        return;
    }

     MountNet::GetInstance()->sendMoveMount(false);
}

void robotControl::on_front_btn_pressed()
{
    QLOG_INFO()<<"on_front_btn_pressed";
    MountNet::GetInstance()->sendMoveMount(true);
}

void robotControl::on_front_btn_released()
{
    QLOG_INFO()<<"on_front_btn_released";
    MountNet::GetInstance()->sendMoveStop();
}

void robotControl::on_back_btn_pressed()
{
    QLOG_INFO()<<"on_back_btn_pressed";
   MountNet::GetInstance()->sendMoveMount(false);
}

void robotControl::on_back_btn_released()
{
    QLOG_INFO()<<"on_back_btn_released";
   MountNet::GetInstance()->sendMoveStop();
}

#include "robotcontrolbak.h"
#include "ui_robotcontrolbak.h"

robotControlBak::robotControlBak(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::robotControlBak)
{
    ui->setupUi(this);
}

robotControlBak::~robotControlBak()
{
    delete ui;
}

void robotControlBak::on_front_btn_pressed()
{
    if(CMountState::GetInstance()->getAutoState())
    {
        return;
    }
    MountNet::GetInstance()->sendMoveMount(true);
}

void robotControlBak::on_front_btn_released()
{
    if(CMountState::GetInstance()->getAutoState())
    {
        return;
    }
    MountNet::GetInstance()->sendMoveStop();
}

void robotControlBak::on_back_btn_pressed()
{
    if(CMountState::GetInstance()->getAutoState())
    {
        return;
    }
    MountNet::GetInstance()->sendMoveMount(false);
}

void robotControlBak::on_back_btn_released()
{
    if(CMountState::GetInstance()->getAutoState())
    {
        return;
    }
    MountNet::GetInstance()->sendMoveStop();
}

void robotControlBak::on_up_btn_pressed()
{
    if(CMountState::GetInstance()->getAutoState())
    {
        return;
    }
    MountNet::GetInstance()->sendMoveSwingA(false);
}

void robotControlBak::on_up_btn_released()
{
    if(CMountState::GetInstance()->getAutoState())
    {
        return;
    }
    MountNet::GetInstance()->sendMoveStop();
}

void robotControlBak::on_down_btn_pressed()
{
    if(CMountState::GetInstance()->getAutoState())
    {
        return;
    }
    MountNet::GetInstance()->sendMoveSwingA(true);
}

void robotControlBak::on_down_btn_released()
{
    if(CMountState::GetInstance()->getAutoState())
    {
        return;
    }
    MountNet::GetInstance()->sendMoveStop();
}

void robotControlBak::on_up1_btn_pressed()
{
    if(CMountState::GetInstance()->getAutoState())
    {
        return;
    }
    MountNet::GetInstance()->sendMoveSwingB(true);
}

void robotControlBak::on_up1_btn_released()
{
    if(CMountState::GetInstance()->getAutoState())
    {
        return;
    }
    MountNet::GetInstance()->sendMoveStop();
}

void robotControlBak::on_down1_btn_pressed()
{
    if(CMountState::GetInstance()->getAutoState())
    {
        return;
    }
    MountNet::GetInstance()->sendMoveSwingB(false);
}

void robotControlBak::on_down1_btn_released()
{
    if(CMountState::GetInstance()->getAutoState())
    {
        return;
    }
    MountNet::GetInstance()->sendMoveStop();
}

void robotControlBak::on_hand_btn_clicked()
{
    qDebug()<<"on_hand_btn_clicked";
    CMountState::GetInstance()->setAutoState(false);
}

void robotControlBak::on_auto_btn_clicked()
{
    qDebug()<<"on_auto_btn_clicked";
    CMountState::GetInstance()->setAutoState(true);
}

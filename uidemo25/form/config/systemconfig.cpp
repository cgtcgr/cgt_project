#include "systemconfig.h"
#include "ui_systemconfig.h"
#include "mount/mountnet.h"

systemConfig::systemConfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::systemConfig)
{
    ui->setupUi(this);


}

systemConfig::~systemConfig()
{
    delete ui;
}

void systemConfig::on_horizontalSlider_valueChanged(int value)
{
    ui->lineEdit->setText(QString::number(value));
    MountNet::GetInstance()->sendMoveSpeed(value);
}

void systemConfig::on_pushButton_clicked()
{
    MountNet::GetInstance()->PackJG(0);
}

void systemConfig::on_horizontalSlider_2_valueChanged(int value)
{
    ui->lineEdit_2->setText(QString::number(value));
    MountNet::GetInstance()->sendSwingMoveSpeed(value);
}

void systemConfig::on_pushButton_2_clicked()
{
    //mountInfoShow::GetInstance()->setInfo(100,100,100,0,88,-88);
}

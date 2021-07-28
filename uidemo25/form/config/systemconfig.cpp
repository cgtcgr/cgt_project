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

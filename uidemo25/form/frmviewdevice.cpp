#include "frmviewdevice.h"
#include "ui_frmviewdevice.h"
#include "QHBoxLayout"

frmViewDevice::frmViewDevice(QWidget *parent) : QWidget(parent), ui(new Ui::frmViewDevice)
{
    ui->setupUi(this);
//    QHBoxLayout *layout = new QHBoxLayout(this);
//    layout->addWidget(VideoPanel::GetInstance());
}

frmViewDevice::~frmViewDevice()
{
    delete ui;
}

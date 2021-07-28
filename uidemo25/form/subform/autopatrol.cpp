#include "autopatrol.h"
#include "ui_autopatrol.h"

autoPatrol::autoPatrol(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::autoPatrol)
{
    ui->setupUi(this);
}

autoPatrol::~autoPatrol()
{
    delete ui;
}

#include "about.h"
#include "ui_about.h"
#include <QDesktopServices>
#include <QUrl>
About::About(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    this->setFixedSize(530,360);
    ui->label_4->setText("<a href=\"http://www.huaraytech.com\">www.huaraytech.com</a>");
    ui->label_4->setOpenExternalLinks(true);
}

About::~About()
{
    delete ui;
}

void About::on_btn_Ok_clicked()
{
    close();
}

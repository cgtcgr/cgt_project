#include "versiondlg.h"
#include "ui_versiondlg.h"

VersionDlg::VersionDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VersionDlg)
{
    ui->setupUi(this);
    setFixedSize(450,200);
}

VersionDlg::~VersionDlg()
{
    delete ui;
}

void VersionDlg::on_btn_Ok_clicked()
{
    accept();
}

#include "frmessagebox.h"
#include "ui_frmessagebox.h"
#include <QTimer>
#include <QDebug>


#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

frmessagebox *frmessagebox::self = 0;
frmessagebox::frmessagebox(QWidget *parent) :
    QDialog(parent)
    ,ui(new Ui::frmessagebox)
    , closeSec(0)
    , currentSec(0)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint |
                         Qt::WindowMinMaxButtonsHint /*| Qt::Tool*/);
    this->setProperty("Form", true);
    this->setProperty("CanMove", true);
    this->setModal(false);

    connect(ui->m_yesBtn,SIGNAL(clicked(bool)),this,SLOT(accept()));
    connect(ui->m_noBtn,SIGNAL(clicked(bool)),this,SLOT(reject()));

    ui->m_msgLabel->setAlignment(Qt::AlignCenter);

    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(checkSec()));
    timer->start();
}

frmessagebox::~frmessagebox()
{
    delete ui;
}

void frmessagebox::checkSec()
{
    if (closeSec == 0) {
        return;
    }
    if (currentSec < closeSec) {
        currentSec++;
    } else {
        this->close();
    }

    QString str(tr("关闭倒计时：    "));
    QString sec_str = QString::number(closeSec - currentSec + 1, 10);
    QString s(" s");

    ui->m_timeLabel->setText(str + sec_str + s);
}
void frmessagebox::setMessage(QString msg, int type, int a, QString tips2)
{
    this->closeSec = a;
    this->currentSec = 0;
    ui->m_timeLabel->clear();

    checkSec();

    if (type == 0) {
        ui->labIcoMain->setStyleSheet("border-image: url(:/image/msg_info.png);");
        ui->m_noBtn->setVisible(false);
        ui->lab_Title->setText(tr("提示"));
    } else if (type == 1) {
        ui->labIcoMain->setStyleSheet("border-image: url(:/image/msg_question.png);");
        ui->lab_Title->setText(tr("询问"));
    } else if (type == 2) {
        ui->labIcoMain->setStyleSheet("border-image: url(:/image/msg_error.png);");
        ui->m_noBtn->setVisible(false);
        ui->lab_Title->setText(tr("错误"));
    }
    ui->m_msgLabel->setText(msg);

    //
    if(tips2.isEmpty())
    {
        ui->lab_context->hide();
    }
    else
    {
        ui->lab_context->setText(tips2); //<提示框增加一行显示
    }
}



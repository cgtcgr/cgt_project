#include "frmdata.h"
#include "ui_frmdata.h"
#include "quiwidget.h"
#include "iconfont.h"
#include "frmdatalog.h"

frmData::frmData(QWidget *parent) : QWidget(parent), ui(new Ui::frmData)
{
    qDebug()<<"frmData::frmData";
    ui->setupUi(this);
       qDebug()<<"frmData::frmData1";
    this->initForm();
            qDebug()<<"frmData::frmData2";
    this->initNav();
                        qDebug()<<"frmData::frmData3";
    this->initIcon();
    qDebug()<<"frmData::frmData success";

}

frmData::~frmData()
{
    delete ui;
}

void frmData::initForm()
{
    ui->widgetLeft->setProperty("flag", "btnNavLeft");
    ui->widgetLeft->setFixedWidth(AppNav::NavSubLeft ? 150 : 120);
}

void frmData::initNav()
{
    ui->stackedWidget->addWidget(new QWidget);

    QList<QString> names, texts;
//    names << "btnDataLog" << "btnDataAlarm" << "btnDataUser";
//    texts << "运 行 记 录" << "报 警 记 录" << "操 作 记 录";
//    icons << 0xea44 << 0xea5b << 0xea59;
    names << "btnPointDataLog"<< "btnTaskDataLog";
    texts << "点 位 数 据" << "任 务 数 据" ;
    icons << 0xea44 << 0xea5b;
    //根据设定实例化导航按钮对象
    for (int i = 0; i < texts.count(); i++) {
        QToolButton *btn = new QToolButton;
        if (AppNav::NavSubLeft) {
            btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            btn->setMinimumHeight(30);
        } else {
            btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
            btn->setMinimumHeight(80);
        }

        btn->setObjectName(names.at(i));
        btn->setText(" " + texts.at(i));
        btn->setCheckable(true);
        btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        connect(btn, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
        //ui->layoutNav->addWidget(btn);
        btns << btn;
    }

    //底部加上弹簧顶上去
    QSpacerItem *verticalSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    //ui->layoutNav->addItem(verticalSpacer);

    btns.at(0)->click();
}

void frmData::initIcon()
{
    int count = btns.count();
    for (int i = 0; i < count; ++i) {
        AppNav::setIconNavSub(btns.at(i), icons.at(i));
    }
}

void frmData::buttonClicked()
{
    //切换到当前窗体
    QAbstractButton *btn = (QAbstractButton *)sender();
    ui->stackedWidget->setCurrentIndex(btns.indexOf(btn));

    //取消其他按钮选中
    foreach (QAbstractButton *b, btns) {
        b->setChecked(b == btn);
    }
}



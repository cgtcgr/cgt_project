#include "frmconfig.h"
#include "ui_frmconfig.h"
#include "quiwidget.h"
#include "iconfont.h"
#include "config/systemconfig.h"
frmConfig::frmConfig(QWidget *parent) : QWidget(parent), ui(new Ui::frmConfig)
{
    ui->setupUi(this);
    this->initForm();
    this->initNav();
    this->initIcon();
}

frmConfig::~frmConfig()
{
    delete ui;
}

void frmConfig::initForm()
{
    ui->widgetLeft->setProperty("flag", "btnNavLeft");
    ui->widgetLeft->setFixedWidth(AppNav::NavSubLeft ? 150 : 120);
}

void frmConfig::initNav()
{
    QList<QString> names, texts;
    names << "btnConfigSystem" << "btnConfigNvr" << "btnConfigIpc" << "btnConfigPoll" << "btnConfigSave" << "btnConfigDb" << "btnConfigUpload";
    texts << "系 统 设 置" << "相机管理" << "阈 值 设 置" << "短 信 设 置" << "视 频 上 传";
    icons << 0xe671 << 0xe9e8 << 0xe9f5 << 0xea14 << 0xe6da;

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
        ui->layoutNav->addWidget(btn);
        btns << btn;
    }

    //底部加上弹簧顶上去
    QSpacerItem *verticalSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    ui->layoutNav->addItem(verticalSpacer);

    btns.at(0)->click();

    systemConfig * sysconfig = new systemConfig;
    ui->stackedWidget->addWidget(sysconfig);
}

void frmConfig::initIcon()
{
    int count = btns.count();
    for (int i = 0; i < count; ++i) {
        AppNav::setIconNavSub(btns.at(i), icons.at(i));
    }
}

void frmConfig::buttonClicked()
{
    //切换到当前窗体
    QAbstractButton *btn = (QAbstractButton *)sender();
    ui->stackedWidget->setCurrentIndex(btns.indexOf(btn));

    //取消其他按钮选中
    foreach (QAbstractButton *b, btns) {
        b->setChecked(b == btn);
    }
}

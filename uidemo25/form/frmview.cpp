#include "frmview.h"
#include "ui_frmview.h"
#include "quiwidget.h"
#include "iconfont.h"
#include "appinit.h"
#include "frmviewdevice.h"
#include <QTabWidget>
#include "mount/cmountstate.h"
#include "mount/mountnet.h"
frmView::frmView(QWidget *parent) :
    QWidget(parent),
    m_robotControl(new robotControl(this)),
    m_autoPatrol(new autoPatrol(this)),
    m_hangpatrol(new hangpatrol(this))
{
    qDebug()<<"frmView start init";
        qDebug()<<"frmView start init.....";
        ui = new Ui::frmView;
    ui->setupUi(this);
     qDebug()<<"frmView start init1";
    this->initForm();
    qDebug()<<"frmView start init2";
    this->initTree();

    this->initNav();
    qDebug()<<"frmView start init success";
    connect(CMountState::GetInstance(),SIGNAL(sendMountState(bool)),this ,SLOT(getMountState(bool)));
    connect(MountNet::GetInstance(),SIGNAL(mountPos(int)), this ,SLOT(getMountPos(int)));
}

frmView::~frmView()
{
    delete ui;
}
void frmView::getMountPos(int pos)
{
    ui->lineEdit->setText(QString::number(pos));
}
void frmView::getMountState(bool flag)
{
    if(flag)
    {
        ui->lineEdit_2->setText("AUTO");
    }
    else
    {
        ui->lineEdit_2->setText("HAND");
    }
}

void frmView::initForm()
{
    connect(AppInit::Instance(), SIGNAL(changeStyle()), this, SLOT(initTree()));
}

void frmView::initTree()
{
//    ui->treeWidget->clear();
//    ui->treeWidget->setAnimated(false);
//    ui->treeWidget->setHeaderHidden(true);
//    //ui->treeWidget->setIndentation(0);
//    //ui->treeWidget->setHeaderLabel("设备列表");

//    QStringList texts;
//    texts << "录像机#1" << "录像机#2" << "录像机#3";

//    QPixmap iconParent = IconFont::Instance()->getPixmap(QUIConfig::TextColor, 0xea23, 18, 20, 20);
//    QPixmap iconChild = IconFont::Instance()->getPixmap(QUIConfig::TextColor, 0xea07, 18, 20, 20);

//    for (int i = 0; i < texts.count(); i++) {
//        //添加父节点
//        QTreeWidgetItem *itemParent = new QTreeWidgetItem(ui->treeWidget);
//        itemParent->setText(0, texts.at(i));
//        itemParent->setIcon(0, iconParent);

//        //循环添加子节点
//        for (int j = 0; j < 10; j++) {
//            QTreeWidgetItem *itemChild = new QTreeWidgetItem(itemParent);
//            itemChild->setText(0, QString("摄像机#%1").arg(j + 1));
//            itemChild->setIcon(0, iconChild);
//        }
//    }

//    ui->treeWidget->expandAll();
}

void frmView::initNav()
{
    QTabWidget * tabWidget  = new QTabWidget(this);
    tabWidget->addTab(m_robotControl,"机器人控制");
    tabWidget->addTab(m_autoPatrol,"自动巡检");
    tabWidget->addTab(m_hangpatrol,"手动巡检");
    ui->stackedWidget->addWidget(tabWidget);
    frmDataLog *control = new frmDataLog;
    ui->stackedWidget->addWidget(control);


    //    QList<QString> texts;
//    texts  << "自动巡检"<<"手动巡检";

//    //这里临时用qlabel加粗显示,可以改成自己的widget窗体 相机控制  挂轨控制
//    QString qss = "font:80pt;";
//    for (int i = 0; i < texts.count(); i++) {
//        QLabel *lab = new QLabel;
//        lab->setAlignment(Qt::AlignCenter);
//        lab->setText(texts.at(i));
//        lab->setStyleSheet(qss);
//        lab->setAutoFillBackground(true);
//        ui->stackedWidget->addWidget(lab);
//    }
}

void frmView::setIndex(int index)
{
    ui->stackedWidget->setCurrentIndex(index);
}

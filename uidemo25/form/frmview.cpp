#include "frmview.h"
#include "ui_frmview.h"
#include "quiwidget.h"
#include "iconfont.h"
#include "appinit.h"
#include "frmviewdevice.h"
#include <QTabWidget>
#include "mount/cmountstate.h"
#include "mount/mountnet.h"
#include "userframe/animatgion.h"
frmView::frmView(QWidget *parent) :
    QWidget(parent),
    m_robotControl(new robotControl(this)),
    m_autoPatrol(new autoPatrol(this)),
    m_hangpatrol(new hangpatrol(this))
{
    qDebug()<<"frmView start init";
    ui = new Ui::frmView;
    ui->setupUi(this);
    this->initForm();
    this->initTree();

    this->initNav();
    connect(CMountState::GetInstance(),SIGNAL(sendMountState(bool)),this ,SLOT(getMountState(bool)));
    connect(MountNet::GetInstance(),SIGNAL(mountPos(int)), this ,SLOT(getMountPos(int)));
    connect(MountNet::GetInstance(),SIGNAL(sendMMXY(int,int,int)), this ,SLOT(getMountAndSwingPos(int,int,int)));
    connect(MountNet::GetInstance(), SIGNAL(sendMountState(int,int ,int,uchar,uchar,short)), this, SLOT(setInfo(int,int ,int,uchar,uchar,short)));
}

frmView::~frmView()
{
    delete ui;
}
void frmView::getMountPos(int pos)
{
    //->lineEdit->setText(QString::number(pos));
}

void frmView::getMountAndSwingPos(int x,int y,int z)
{
    //ui->lineEdit->setText(QString::number(x)+QString(" ")+QString::number(y)+QString(" ")+QString::number(z));
}
void frmView::setInfo(int x, int y, int z, uchar errM, uchar power, short a)
{
    ui->widget_2->setInfo(x,y,z,errM,power,a);
}
void frmView::getMountState(bool flag)
{
    ui->widget_2->setAuto(flag);
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
    QFont font;
    font.setFamily("微软雅黑");//字体
    //font.setPixelSize(12);//文字像素大小
    font.setPointSize(12);//文字大小
    font.setBold(true);//粗体
    //font.setLetterSpacing(QFont::PercentageSpacing,10);//间距
    tabWidget->setFont(font);
    ui->stackedWidget->addWidget(tabWidget);
    frmDataLog *control = new frmDataLog;
    ui->stackedWidget->addWidget(control);
}

void frmView::setIndex(int index)
{
    ui->stackedWidget->setCurrentIndex(index);
    animatgion::wigShow(ui->stackedWidget->currentWidget(),0,3);
}

#include "tipsitem.h"
#include "ui_tipsitem.h"
#include <QDebug>
#include <QLayoutItem>
#include <QDateTime>

static int id  = 0;
TipsItem::TipsItem(QString tim,QString msg,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TipsItem)
{
    ui->setupUi(this);
    setContext(tim,msg);
    //uint timSec = QDateTime::currentDateTime().toTime_t();
    m_id = "ID:" + QString::number(id++);

    m_timer = new QTimer(this);
    m_timer->setInterval(3000);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(checkclose()));
    m_timer->start();
}

TipsItem::~TipsItem()
{
    qDebug()<< "TipsItem::~TipsItem";
    delete ui;
    if(m_timer->isActive())
    {
        m_timer->stop();
    }
}

void TipsItem::setContext(QString tim, QString msg)
{
    ui->label->setText(tim);
    ui->label_4->setText(msg);
}

void TipsItem::on_btn_close_clicked()
{
    close();
    if(m_timer->isActive())
    {
        m_timer->stop();
    }
    emit closeTips(m_id);
}

void TipsItem::checkclose()
{
    m_timer->stop();
    close();
    emit closeTips(m_id);
}

///////////////////////////////////////////

tipsGroup::tipsGroup(QWidget *parent)
    : QWidget(parent)
{
     hlayout = new QVBoxLayout(this);
     hlayout->addStretch();

     hlayout->setSpacing(0);
     this->setLayout(hlayout);
}

tipsGroup::~tipsGroup()
{

}

void tipsGroup::AddItems(TipsItem *item)
{
    //>5个先删除最后一个
    QMutexLocker lock(&m_mutex);
    if(m_currentItem.size() >= 5)
    {
        TipsItem *item_ = m_currentItem.last();
        m_currentItem.pop_back();
        if(item_)
        {
            delete item_;
        }
    }

    m_currentItem.push_front(item);

    // 更新界面
    UpdateUI();
}

void tipsGroup::DeleteItem(QString id)
{
    qDebug()<<"DeleteItem before:"<< m_currentItem.size();
    QMutexLocker lock(&m_mutex);
    if(m_currentItem.isEmpty())
    {
        this->hide();
        qDebug()<<"hide()-----------";
        return;
    }

    /// 根据ID查找对应的item并删除

    for(auto iter : m_currentItem)
    {
       if(iter->getId() == id)
       {
           qDebug()<<"removeOne"<< m_currentItem.size();
           m_currentItem.removeOne(iter);

           if(iter)
                delete iter;
           break;
       }
    }

    qDebug()<<"DeleteItem after:"<< m_currentItem.size();
    // 更新界面
    UpdateUI();
}

void tipsGroup::UpdateUI()
{
    if(m_currentItem.isEmpty())
    {
        this->hide();
        qDebug()<<"hide()-----------";
        return;
    }
    this->show();

    //清除所有
    QLayoutItem *child;
    while ((child = hlayout->takeAt(0)) != 0)
    {
        hlayout->removeWidget(child->widget());
    }

    hlayout->addStretch();
    for(auto iter : m_currentItem)
    {
       hlayout->addWidget(iter);
    }
    hlayout->setSpacing(0);
    this->setLayout(hlayout);
}

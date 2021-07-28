#include "waittingwidget.h"
#include <QPainter>

WaittingWidget::WaittingWidget()
{

}

WaittingWidget::~WaittingWidget()
{
    if(animationtimeline->state() == QTimeLine::Running)
    {
        animationtimeline->stop();
    }
}

WaittingWidget::WaittingWidget(QString pixurl,int tinypixcount,QString text,QWidget *p):BaseWidget(p)
{
    //setStyleSheet("QWidget{background:white;}");
    _index=0;
    m_text=text;

    QPixmap pix=QPixmap(pixurl);
    animationtimeline = new QTimeLine(800,this);
    animationtimeline->setFrameRange(0,12);
    animationtimeline->setCurveShape(QTimeLine::LinearCurve);
    connect(animationtimeline,SIGNAL(frameChanged(int)),SLOT(slot_valuechange(int)));
    animationtimeline->setLoopCount(0);

    for(int i=0;i<tinypixcount;i++)//进入
    {
        m_listpix<<pix.copy(i*(pix.width()/tinypixcount),0,pix.width()/tinypixcount,pix.height());
    }
}

void WaittingWidget::setTips(QString str)
{
    m_text = str;
}
void WaittingWidget::paintEvent(QPaintEvent *p)
{
    BaseWidget::paintEvent(p);
    QPainter pter(this);
    pter.drawPixmap(width()/2-m_listpix.value(0).width()*2,(height()-m_listpix.value(0).height())/2,m_listpix.at(_index));

    if(!m_text.isEmpty())
    {
        QFont ft("微软雅黑");
        ft.setPixelSize(14);
        QFontMetrics metric(ft);
        pter.setPen(QColor(0,160,230));
        pter.setFont(ft);
        pter.drawText(width()/2-m_listpix.value(0).width()+10,
                      (height()-metric.height())/2,metric.width(m_text),metric.height(),Qt::AlignLeft,m_text);
    }

}

void WaittingWidget::showEvent(QShowEvent *e)
{
    QTimeLine::State timeState;
    timeState = animationtimeline->state();

    BaseWidget::showEvent(e);
    if(this->isHidden())
    {
        if(timeState == QTimeLine::Running)
        {
            animationtimeline->stop();
        }
    }
    else
    {
        if((timeState == QTimeLine::NotRunning) |  (timeState == QTimeLine::Paused))
        {
            animationtimeline->start();
        }
        raise();
    }
}

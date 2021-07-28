#include "waitprocess.h"
//#include "ui_waitprocess.h"
#include <QDebug>

WaitProcess::WaitProcess(QWidget *parent) :
    QWidget(parent),
    m_timer(nullptr),
    m_persent(0)
{
    //setWindowFlags(Qt::FramelessWindowHint);
    this->resize(50,50);
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &WaitProcess::update_ui);
    m_timer->start(80);
}

WaitProcess::~WaitProcess()
{
    m_timer->stop();

}

void WaitProcess::update_ui()
{
    m_persent+=5;
    if(m_persent == 100)
    {
       m_persent = 0;
    }
    update();
}

void WaitProcess::paintEvent(QPaintEvent *event)
{
    paintPie();
}
void WaitProcess::paintPie()
{
    QPainter p;
    p.begin(this);
    p.setRenderHint(QPainter::Antialiasing);

    int m_rotateAngle = 360*m_persent/100;

    int side = qMin(width(), height());
    QRectF outRect(0, 0, side, side);
    QRectF inRect(side*0.2, side*0.2, side*0.6, side*0.6);
    //QString valueStr = QString("%1%").arg(QString::number(m_persent));

    //画外圆
    p.setPen(Qt::NoPen);
    p.setBrush(QBrush(QColor(97, 117, 118)));
    p.drawEllipse(outRect);

    if(m_rotateAngle != 0)
    {
        p.setBrush(QBrush(QColor(0, 160, 230)));
        p.drawPie(outRect, (90-m_rotateAngle)*16, m_rotateAngle*16);
    }
    else
    {
        p.setBrush(QBrush(QColor(97, 117, 118)));
        p.drawEllipse(outRect);
    }

    //画遮罩
    p.setBrush(palette().window().color());
    p.drawEllipse(inRect);

//    QFont f = QFont("Microsoft YaHei", 15, QFont::Bold);
//    p.setFont(f);
//    p.setPen(QColor("#555555"));
//    p.drawText(inRect, Qt::AlignCenter, valueStr);
    p.end();
    //画文字
}


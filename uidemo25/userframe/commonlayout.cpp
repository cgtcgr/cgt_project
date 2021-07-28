#include "commonlayout.h"

commonlayout::commonlayout(QVector<info *> & Info):
m_vector(Info)
{
    int size = m_vector.size()/5 +1;
    m_w1 = new QWidget();
    m_w2 = new QWidget();
    m_w2->setMinimumHeight(size*70);
    m_w3 = new QWidget();
    this->addWidget(m_w1);
    this->setStretch(0,1);
    this->addWidget(m_w2);
    this->setStretch(1,size);
    this->addWidget(m_w3);
    this->setStretch(2,1);

    m_tipl1 = new QLabel(m_w1);
    m_tipl1->setMinimumSize(600,30);
    m_tipl2 = new QLabel(m_w1);
    m_tipl2->setMinimumSize(600,30);
    m_tipl1->setText(QString(""));
    m_tipl2->setText(QString(""));
    QFont font("Microsoft YaHei", 14, 50);
    m_tipl1->setFont(font);
    m_tipl2->setFont(font);
    QVBoxLayout *hlay = new QVBoxLayout(m_w1);
    hlay->addWidget(m_tipl1);
    hlay->addWidget(m_tipl2);

    int i = 0;
    for(auto it :m_vector)
    {
        QPushButton * m = new QPushButton(QString::fromStdString(std::to_string(i++)),m_w2);
        m->setText(it->Name);
        m_buttonVector.push_back(m);
    }
    for(auto it :m_buttonVector)
    {
        it->setMaximumSize(120,150);
        it->setMinimumSize(120,150);
    }
    QGridLayout * grid = new QGridLayout(m_w2);
    for(int i = 0;i<size;i++)
    {
        for(int j = 0;j<5;j++)
        {
            if((5*i+j) == m_buttonVector.size())
            {
                break;
            }
            grid->addWidget(m_buttonVector[5*i+j],i,j);
        }
    }

    QHBoxLayout *vlay = new QHBoxLayout(m_w3);
    m_YESBtn  = new QPushButton("YES",m_w3);
    m_YESBtn->setMinimumSize(60,50);
    m_NOBtn = new QPushButton("NO",m_w3);
    m_NOBtn->setMinimumSize(60,50);
    m_stopBtn = new QPushButton("STOP",m_w3);
    m_stopBtn->setMinimumSize(60,50);
    QSpacerItem * mspace = new QSpacerItem(200,50);
    vlay->addSpacerItem(mspace);
    vlay->addWidget(m_stopBtn);
    QSpacerItem * mspace2 = new QSpacerItem(100,50);
    vlay->addSpacerItem(mspace2);
    vlay->addWidget(m_YESBtn);
    vlay->addWidget(m_NOBtn);
}

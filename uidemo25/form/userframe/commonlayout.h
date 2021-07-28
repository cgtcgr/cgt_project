#ifndef COMMONLAYOUT_H
#define COMMONLAYOUT_H

#include <QVBoxLayout>
#include <QVector>
#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpacerItem>

class commonlayout: public QVBoxLayout
{
    Q_OBJECT
public:
    struct info
    {
        QString Name;
        int Id;
        int Type;
        info(QString name,int id,int type):Name(name),Id(id),Type(type){}
        info(QString name,int id):Name(name),Id(id){}
    };

public:
    commonlayout(QVector<info *> &Info);
    QVector<QPushButton *> m_buttonVector;
    QWidget *m_w1;
    QWidget *m_w2;
    QWidget *m_w3;
    QLabel * m_tipl1;
    QLabel * m_tipl2;
    QPushButton * m_YESBtn;
    QPushButton * m_NOBtn;
    QPushButton * m_stopBtn;
private:
    QVector<info *> m_vector;

};

#endif // COMMONLAYOUT_H

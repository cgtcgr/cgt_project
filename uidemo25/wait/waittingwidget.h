#ifndef WAITTINGWIDGET_H
#define WAITTINGWIDGET_H

#include <QObject>
#include <QWidget>
#include "basewidget.h"
#include <QLabel>
#include <QDebug>
#include <QTimeLine>
#include <QLineEdit>

class WaittingWidget : public BaseWidget
{
    Q_OBJECT

public:
    WaittingWidget();
    ~WaittingWidget();
    explicit WaittingWidget(QString pixurl,int tinypixcount,QString text,QWidget *p = nullptr);
    void setTips(QString str);

protected:
    void paintEvent(QPaintEvent *);
    void showEvent(QShowEvent *e);

public slots:
    void slot_valuechange(int i) {_index=i; update(); }
private:
    QVector<QPixmap> m_listpix;
    QString m_text;
    int _index;
    QTimeLine *animationtimeline;
};

#endif // WAITTINGWIDGET_H

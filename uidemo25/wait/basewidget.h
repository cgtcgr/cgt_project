#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include <QWidget>
#include <QObject>

class BaseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BaseWidget(QWidget *parent = 0);
    ~BaseWidget();
protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);

signals:

public slots:
};

#endif // BASEWIDGET_H

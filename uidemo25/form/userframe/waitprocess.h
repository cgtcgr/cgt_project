#ifndef WAITPROCESS_H
#define WAITPROCESS_H

#include <QWidget>
#include <QTimer>
#include <QPainter>

class WaitProcess : public QWidget
{
    Q_OBJECT

public:
    explicit WaitProcess(QWidget *parent = 0);
     ~WaitProcess();

protected:
    void paintEvent(QPaintEvent *event) override;
private:
    void paintPie();
private slots:
    void update_ui();
private:
    QTimer * m_timer;
    int m_persent;

};

#endif // WAITPROCESS_H

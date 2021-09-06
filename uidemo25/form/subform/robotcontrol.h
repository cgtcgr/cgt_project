#ifndef ROBOTCONTROL_H
#define ROBOTCONTROL_H

#include <QWidget>
#include "mount/cmountstate.h"
#include "mount/mountnet.h"
//#include <QTimer>
namespace Ui {
class robotControl;
}

class robotControl : public QWidget
{
    Q_OBJECT

public:
    explicit robotControl(QWidget *parent = nullptr);
    ~robotControl();

private slots:

    void on_hand_btn_clicked();

    void on_auto_btn_clicked();

    void on_front_btn_pressed();

    void on_front_btn_released();

    void on_back_btn_pressed();

    void on_back_btn_released();

    void on_down_btn_pressed();

    void on_down_btn_released();

    void on_up_btn_pressed();

    void on_up_btn_released();

    void on_up1_btn_pressed();

    void on_up1_btn_released();

    void on_down1_btn_pressed();

    void on_down1_btn_released();

    void on_pushButton_clicked();

private:
    Ui::robotControl *ui;
//    QTimer * m_devFront;
//    QTimer * m_devBack;
};

#endif // ROBOTCONTROL_H

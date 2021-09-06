#ifndef ROBOTCONTROLBAK_H
#define ROBOTCONTROLBAK_H

#include <QWidget>
#include "mount/cmountstate.h"
#include "mount/mountnet.h"
namespace Ui {
class robotControlBak;
}

class robotControlBak : public QWidget
{
    Q_OBJECT

public:
    explicit robotControlBak(QWidget *parent = nullptr);
    ~robotControlBak();

private slots:
    void on_front_btn_pressed();

    void on_front_btn_released();

    void on_back_btn_pressed();

    void on_back_btn_released();

    void on_up_btn_pressed();

    void on_up_btn_released();

    void on_down_btn_pressed();

    void on_down_btn_released();

    void on_up1_btn_pressed();

    void on_up1_btn_released();

    void on_down1_btn_pressed();

    void on_down1_btn_released();

    void on_hand_btn_clicked();

    void on_auto_btn_clicked();

private:
    Ui::robotControlBak *ui;
};

#endif // ROBOTCONTROLBAK_H

#ifndef MOUNTINFOSHOW_H
#define MOUNTINFOSHOW_H

#include <QWidget>

namespace Ui {
class mountInfoShow;
}

class mountInfoShow : public QWidget
{
    Q_OBJECT

public:
    explicit mountInfoShow(QWidget *parent = nullptr);
    ~mountInfoShow();
public slots:
    void setAuto(bool flag);
    void setInfo(int x, int y , int z,uchar errM,uchar power,short a);
private:
    Ui::mountInfoShow *ui;
    int X;
    int Y;
    int Z;
    uchar nErrerMotor;
    uchar powerRate;
    short A;
    bool lowPowerAlarm;
    bool motorAlarm;
};

#endif // MOUNTINFOSHOW_H

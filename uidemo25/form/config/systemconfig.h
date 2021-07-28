#ifndef SYSTEMCONFIG_H
#define SYSTEMCONFIG_H

#include <QWidget>

namespace Ui {
class systemConfig;
}

class systemConfig : public QWidget
{
    Q_OBJECT

public:
    explicit systemConfig(QWidget *parent = nullptr);
    ~systemConfig();

private slots:
    void on_horizontalSlider_valueChanged(int value);

private:
    Ui::systemConfig *ui;
};

#endif // SYSTEMCONFIG_H

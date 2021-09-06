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

    void on_pushButton_clicked();

    void on_horizontalSlider_2_valueChanged(int value);

    void on_pushButton_2_clicked();

private:
    Ui::systemConfig *ui;
};

#endif // SYSTEMCONFIG_H

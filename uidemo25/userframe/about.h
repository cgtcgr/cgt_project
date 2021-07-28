#ifndef ABOUT_H
#define ABOUT_H

#include <QWidget>
#include <QPushButton>
#include <QLayout>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

namespace Ui {
class About;
}

class About : public QWidget
{
    Q_OBJECT

public:
    explicit About(QWidget *parent = 0);
    ~About();
private slots:

    void on_btn_Ok_clicked();

private:
    Ui::About *ui;
};

#endif // ABOUT_H

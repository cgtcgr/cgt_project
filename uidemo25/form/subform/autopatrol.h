#ifndef AUTOPATROL_H
#define AUTOPATROL_H

#include <QWidget>

namespace Ui {
class autoPatrol;
}

class autoPatrol : public QWidget
{
    Q_OBJECT

public:
    explicit autoPatrol(QWidget *parent = nullptr);
    ~autoPatrol();

private:
    Ui::autoPatrol *ui;
};

#endif // AUTOPATROL_H

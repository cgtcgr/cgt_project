#ifndef FRMESSAGEBOX_H
#define FRMESSAGEBOX_H

#include <QDialog>
#include <QMutex>
namespace Ui {
class frmessagebox;
}

class frmessagebox : public QDialog
{
    Q_OBJECT

public:
    explicit frmessagebox(QWidget *parent = 0);
    ~frmessagebox();

    static frmessagebox *Instance()
    {
        static QMutex mutex;

        if (!self) {
            QMutexLocker locker(&mutex);

            if (!self) {
                self = new frmessagebox;
            }
        }

        return self;
    }
    void setMessage(QString msg, int type, int a  = 0,QString tips2 = "");

public slots:
    void checkSec();

private:
    Ui::frmessagebox *ui;
    static frmessagebox *self;

    qint16 closeSec;
    qint16 currentSec;
    QTimer *timer;
};

#endif // FRMESSAGEBOX_H

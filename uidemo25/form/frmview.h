#ifndef FRMVIEW_H
#define FRMVIEW_H

#include <QWidget>
#include "subform/robotcontrol.h"
#include "subform/autopatrol.h"
#include "subform/hangpatrol.h"
#include <QDebug>
#include "frmdatalog.h"
namespace Ui {
class frmView;
}

class frmView : public QWidget
{
    Q_OBJECT

public:
    explicit frmView(QWidget *parent = 0);
    ~frmView();

private:
    Ui::frmView *ui;
    robotControl * m_robotControl;
    autoPatrol * m_autoPatrol;
    hangpatrol * m_hangpatrol;
public slots:
    void getMountState(bool flag);
    void getMountPos(int pos);
    void getMountAndSwingPos(int x, int y, int z);
    void setInfo(int x, int y, int z, uchar errM, uchar power, short a);
private slots:
    void initForm();
    void initTree();
    void initNav();
    void setIndex(int index);
};

#endif // FRMVIEW_H

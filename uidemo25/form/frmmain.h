#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QWidget>
#include <QMenu>
#include "userframe/about.h"
class FramelessHelper;
class QAbstractButton;

namespace Ui
{
class frmMain;
}

class frmMain : public QWidget
{
    Q_OBJECT

public:
    explicit frmMain(QWidget *parent = 0);
    ~frmMain();

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    Ui::frmMain *ui;

    //无边框拉伸类
    FramelessHelper *frameless;

    //导航按钮+图标集合
    QList<QChar> icons;
    QList<QAbstractButton *> btns;

    /// 帮助菜单
    QMenu *m_menu;
    QAction *m_helpAction;
    QAction *m_qaAction;
    QAction *m_aboutAction;
    QAction *m_versionAction;
    QAction *m_setAction;
    About *m_aboutUsDlg;

    void createAboutMenu();
private slots:
    void initForm();        //初始化界面数据
    void initNav();         //初始化导航窗体
    void initIcon();        //初始化按钮图标
    void buttonClicked();   //按钮单击事件

    void initText();        //初始化标题等
    void initAction();      //增加换肤菜单
    void doAction();        //执行菜单操作

private slots:
    void on_btnMenu_Min_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Close_clicked();

    void OnHelpActionTriggered();
    void OnVersionTriggered();
    void OnAboutUsTriggered();
signals:
    void setIndex(int index);
    void changeStyle();
};

#endif // FRMMAIN_H

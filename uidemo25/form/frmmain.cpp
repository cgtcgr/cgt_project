#include "frmmain.h"
#include "ui_frmmain.h"
#include "quiwidget.h"
#include "iconfont.h"
#include "appinit.h"
#include "framelesshelper.h"

#include "frmview.h"
#include "frmdata.h"
#include "frmconfig.h"
#include "QsLog.h"
#include "frmdatalog.h"

#include "task/taskmanager.h"
#include "task/periodtask.h"
#include "sql/sqlmanager.h"
#include "mount/steeringnet.h"
#include "userframe/versiondlg.h"
frmMain::frmMain(QWidget *parent) : QWidget(parent), ui(new Ui::frmMain)
{
    ui->setupUi(this);
    //initLog();
    this->initForm();
    createAboutMenu();
    this->initText();
    this->initNav();
    this->initIcon();
    this->initAction();
    //this->initServerice();
    QUIHelper::setFormInCenter(this);
}

frmMain::~frmMain()
{
    delete ui;
}

bool frmMain::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        //双击标题栏最大化
        if (watched == ui->widgetTop) {
            on_btnMenu_Max_clicked();
        }
    }

    return QWidget::eventFilter(watched, event);
}
void frmMain::initForm()
{
    ui->widgetBtn->setProperty("flag", "btnNavTop");
    ui->widgetMain->setProperty("form", true);
    ui->widgetTop->setProperty("form", "title");
    ui->widgetTop->setFixedHeight(80);
    ui->bottomWidget->setFixedHeight(25);

    this->setProperty("form", true);
    //this->setProperty("canMove", true);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    ui->widgetTop->installEventFilter(this);
    ui->labLogoBg->installEventFilter(this);

    //设置右上角菜单,图形字体
    IconHelper::Instance()->setIcon(ui->btnMenu_Min, QUIConfig::IconMin);
    IconHelper::Instance()->setIcon(ui->btnMenu_Max, QUIConfig::IconNormal);
    IconHelper::Instance()->setIcon(ui->btnMenu_Close, QUIConfig::IconClose);

    //无边框可拉伸类
    frameless = new FramelessHelper(this);
    //激活当前窗体
    frameless->activateOn(this);
    //设置窗体的标题栏高度
    frameless->setTitleHeight(ui->widgetTop->height());
    //设置窗体可移动
    frameless->setWidgetMovable(true);
    //设置窗体可缩放
    frameless->setWidgetResizable(true);

    //设置底部状态栏
    ui->bottomWidget->setVersion("V20210410");
    ui->bottomWidget->setCurrentUserName("admin");
    ui->bottomWidget->setCurrentUserType("管理员");
    ui->bottomWidget->setFontName(QUIConfig::FontName);
    int fontSize = (QUIConfig::FontSize > 12 ? QUIConfig::FontSize - 1 : QUIConfig::FontSize);
    ui->bottomWidget->setFontSize(fontSize);
    ui->bottomWidget->setLineFixedWidth(true);
    ui->bottomWidget->start();
}

void frmMain::initNav()
{
    //不同的样式边距和图标大小不一样,可以自行调整三个值看效果
    int topMargin, otherMargin;
    if (AppNav::NavMainLeft) {
        topMargin = ui->widgetMenu->height() + 6;
        otherMargin = 9;
    } else {
        topMargin = 3;
        otherMargin = 3;
    }

    ui->layoutNav->setContentsMargins(otherMargin, topMargin, otherMargin, otherMargin);

    frmView *view = new frmView;
    ui->stackedWidget->addWidget(view);
    qDebug()<<"frmMain::initNav view";

    frmData *data = new frmData;
    ui->stackedWidget->addWidget(data);
    qDebug()<<"frmMain::initNav frmData";

    frmConfig *config = new frmConfig;
    ui->stackedWidget->addWidget(config);
    qDebug()<<"frmMain::initNav frmConfig";

    connect(this, SIGNAL(setIndex(int)), view, SLOT(setIndex(int)));
    connect(this, SIGNAL(changeStyle()), this, SLOT(initIcon()));
    connect(this, SIGNAL(changeStyle()), data, SLOT(initIcon()));
    connect(this, SIGNAL(changeStyle()), config, SLOT(initIcon()));

    QList<QString> names, texts;
    names << "btnMainDevice" << "btnMainMap" << "btnMainData" << "btnData" << "btnConfig";
    texts << "视频监控" << "巡检控制"  << "巡检数据" << "系统设置";
    icons << 0xea00 << 0xe695 << 0xe60a << 0xe699;

    //根据设定实例化导航按钮对象
    for (int i = 0; i < texts.count(); i++) {
        QToolButton *btn = new QToolButton;
        if (AppNav::NavMainLeft) {
            btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            btn->setMinimumWidth(115);
        } else {
            btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
            btn->setMinimumWidth(80);
        }

        btn->setObjectName(names.at(i));
        btn->setText(texts.at(i));
        btn->setCheckable(true);
        btn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        connect(btn, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
        ui->layoutNav->addWidget(btn);
        btns << btn;
    }

    btns.at(0)->click();
}

void frmMain::initIcon()
{
    //如果logo图片不存在则隐藏标签,否则显示logo图片
    ui->labLogoBg->setAlignment(Qt::AlignCenter);
    QPixmap pixLogo(QUIHelper::appPath() + "/" + "bg.png");
    if (pixLogo.isNull()) {
        //ui->labLogoBg->setVisible(false);
        //设置图形字体作为logo
        int size = 60;
        ui->labLogoBg->setFixedWidth(size + 20);
        IconFont::Instance()->setIcon(ui->labLogoBg, 0xe6f2, size);
    } else {
        int size = 150;
        ui->labLogoBg->setFixedWidth(size + 20);
        pixLogo = pixLogo.scaled(size, size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->labLogoBg->setPixmap(pixLogo);
    }

    int count = btns.count();
    for (int i = 0; i < count; ++i) {
        AppNav::setIconNavMain(btns.at(i), icons.at(i));
    }
}

void frmMain::buttonClicked()
{
    QAbstractButton *btn = (QAbstractButton *)sender();
    if (btn == btns.at(0)) {
        emit setIndex(0);
        ui->stackedWidget->setCurrentIndex(0);
    } else if (btn == btns.at(1)) {
        emit setIndex(1);
        ui->stackedWidget->setCurrentIndex(0);
    } else if (btn == btns.at(2)) {
        ui->stackedWidget->setCurrentIndex(1);
    } else if (btn == btns.at(3)) {
        ui->stackedWidget->setCurrentIndex(2);
    }

    //取消其他按钮选中
    foreach (QAbstractButton *b, btns) {
        b->setChecked(b == btn);
    }
}

void frmMain::initText()
{
    //从配置文件读取logo名称
    ui->labLogoCn->setText("通用巡检监控平台");
    ui->labLogoEn->setText("General patrol monitor platform");
    this->setWindowTitle(ui->labLogoCn->text());

    //设置标题+版本+版权
    ui->bottomWidget->setTitle(ui->labLogoCn->text());
    ui->bottomWidget->setAuthor("XX智能设备");
}

void frmMain::initAction()
{
    this->setContextMenuPolicy(Qt::ActionsContextMenu);

    //添加换肤菜单
    QStringList styleNames, styleFiles;
    QUIHelper::getStyle(styleNames, styleFiles);
    for (int i = 0; i < styleNames.count(); i++) {
        QAction *action = new QAction(this);
        action->setText(styleNames.at(i));
        action->setData(styleFiles.at(i));
        connect(action, SIGNAL(triggered(bool)), this, SLOT(doAction()));
        this->addAction(action);
    }
}

void frmMain::doAction()
{
    QAction *action = (QAction *)sender();
    QString qssFile = action->data().toString();
    AppInit::Instance()->initStyle(qssFile);
    emit changeStyle();
}

void frmMain::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}

void frmMain::on_btnMenu_Max_clicked()
{
    static bool max = false;
    static QRect location = this->geometry();

    if (max) {
        this->setGeometry(location);
    } else {
        location = this->geometry();
        int screenIndex = QUIHelper::getScreenIndex();
        this->setGeometry(qApp->desktop()->availableGeometry(screenIndex));
    }

    IconHelper::Instance()->setIcon(ui->btnMenu_Max, max ? QUIConfig::IconNormal : QUIConfig::IconMax);
    this->setProperty("canMove", max);
    max = !max;
    frameless->setWidgetMovable(!max);
    frameless->setWidgetResizable(!max);
}

void frmMain::on_btnMenu_Close_clicked()
{
    this->close();
}
void frmMain::createAboutMenu()
{
    m_aboutUsDlg = new About();
    m_menu = new QMenu(this);

    m_aboutAction = m_menu->addAction(tr("关于我们"));
    connect(m_aboutAction,SIGNAL(triggered()),this,SLOT(OnAboutUsTriggered()));

    m_versionAction = m_menu->addAction(tr("版本信息"));
    connect(m_versionAction,SIGNAL(triggered()),this,SLOT(OnVersionTriggered()));

    m_helpAction = m_menu->addAction(tr("使用手册"));
    connect(m_helpAction,SIGNAL(triggered()),this,SLOT(OnHelpActionTriggered()));

    ui->btnMenu_Help->setMenu(m_menu);
    m_menu->setStyleSheet("QMenu {"
                          "    background-color: black; "
                          "    border: 1px solid white;"
                          "}"

                          "QMenu::item {"

                         "     background-color: transparent;"
                              "padding:8px 32px;"
                              "margin:0px 8px;"
                              "border-bottom:1px solid #DBDBDB;"
                          "}"

                          "QMenu::item:selected { "
                              "background-color: #2dabf9;"
                          "}");
}
void frmMain::OnHelpActionTriggered()
{
    //QString qtManulFile=qApp->applicationDirPath()+"/manul.pdf";
    //QDesktopServices::openUrl(QUrl::fromLocalFile(qtManulFile));
}

void frmMain::OnAboutUsTriggered()
{
    m_aboutUsDlg->setAttribute(Qt::WA_ShowModal, true);
    m_aboutUsDlg->show();
}

void frmMain::OnVersionTriggered()
{
    VersionDlg verDlg(this);
    verDlg.exec();
}

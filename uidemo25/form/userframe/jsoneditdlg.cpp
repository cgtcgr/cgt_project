#include "jsoneditdlg.h"
#include "ui_jsoneditdlg.h"
#include <QDebug>
#include <QMessageBox>
#include "json/json.h"
#include "configs/parameter/localconfigmanger.h"

JsonEditDlg::JsonEditDlg(QString strTitle, QWidget *parent) :
    QDialog(parent),
    m_strTitel(strTitle),
    m_modifyType(TYPE_WAYPOINTS),
    m_index(0),
    ui(new Ui::JsonEditDlg)
{
    ui->setupUi(this);
    resize(500,800);

    ui->plainTextEdit->document()->setModified(false);
    setWindowModified(false);

    setWindowTitle(m_strTitel+"[*]");
}

JsonEditDlg::~JsonEditDlg()
{
    delete ui;
}

void JsonEditDlg::UpdateJsonView(const QByteArray jsonByte)
{
    ui->plainTextEdit->document()->setModified(false);
    setWindowModified(false);
    ui->plainTextEdit->setPlainText(jsonByte);

    connect(ui->plainTextEdit->document(), &QTextDocument::contentsChanged,
            this, &JsonEditDlg::documentWasModified);

    //修改类型

    //索引
}

void JsonEditDlg::SetConfigParams(JsonEditDlg::MODIFY_TYPE type, int index)
{
    m_modifyType = type;
    m_index = index;
}

bool JsonEditDlg::maybeSave()
{
    if (!ui->plainTextEdit->document()->isModified())
        return true;
    const QMessageBox::StandardButton ret
        = QMessageBox::warning(this, tr("Application"),
                               tr("The document has been modified.\n"
                                  "Do you want to save your changes?"),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    switch (ret) {
    case QMessageBox::Save:
        ///TODO
        if(configSave())
            return true;
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}

bool JsonEditDlg::configSave()
{
    //校验json格式，格式出错则返回false,其他true
    QString strJson = ui->plainTextEdit->toPlainText();
    //解析
    Json::Value outValue;
    Json::Reader reader;

    if(!reader.parse(strJson.toStdString().c_str(),outValue))
    {
        QMessageBox::warning(this,"warnning","json format erro,please check!");
        return false;
    }
    else
    {
        Json::Value slicejson = Json::Value::null;
        LocalConfigManger::GetTaskSliceJson(slicejson);

        if(slicejson == Json::Value::null)
        {
            QMessageBox::warning(this,"warnning","loading json file fail!");
            return true;
        }

        switch (m_modifyType)
        {
        case TYPE_WAYPOINTS:
                slicejson[m_index]["WayPoints"] = outValue;
            break;
        case TYPE_ACTIONS:
                slicejson[m_index]["ActionGroups"] = outValue;
            break;
        case TYPE_SHELFPOINTS:
            ///TODO

            break;
        case TYPE_LASERPOINTS:
                slicejson[m_index]["LaserPoints"] = outValue;
            break;
        default:
            break;
        }

        LocalConfigManger::UpdateConfigJsonFile(TASKSLICECONFIG,slicejson);
    }

    return true;
}

void JsonEditDlg::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void JsonEditDlg::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_S  &&  event->modifiers() == Qt::ControlModifier)
    {
        if(configSave()) //保存成功,关闭的时候不需要再提示
        {
            setWindowModified(false);
            ui->plainTextEdit->document()->setModified(false);
        }
        else
        {

        }
    }
}

void JsonEditDlg::reject()
{
    close();
}

void JsonEditDlg::documentWasModified()
{
    setWindowModified(ui->plainTextEdit->document()->isModified());
}

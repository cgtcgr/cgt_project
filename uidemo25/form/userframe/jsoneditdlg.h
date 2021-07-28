#ifndef JSONEDITDLG_H
#define JSONEDITDLG_H

#include <QDialog>

namespace Ui {
class JsonEditDlg;
}

class JsonEditDlg : public QDialog
{
    Q_OBJECT

public:
    typedef enum tag_TYPE
    {
        TYPE_WAYPOINTS,
        TYPE_ACTIONS,
        TYPE_SHELFPOINTS,
        TYPE_LASERPOINTS
    }MODIFY_TYPE;

public:
    explicit JsonEditDlg(QString strTitle = "",QWidget *parent = 0);
    ~JsonEditDlg();

    void UpdateJsonView(const QByteArray jsonByte);

    ///
    /// \brief SetConfigParams
    /// \param type
    /// \param index
    ///
    void SetConfigParams(MODIFY_TYPE type,int index);

private:
    Ui::JsonEditDlg *ui;
    QString m_strTitel;
    MODIFY_TYPE m_modifyType;
    int m_index;

    bool maybeSave();
    bool configSave();

protected:
    void closeEvent(QCloseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void reject();

private slots:
    void documentWasModified();
};

#endif // JSONEDITDLG_H

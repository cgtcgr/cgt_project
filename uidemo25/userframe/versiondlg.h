#ifndef VERSIONDLG_H
#define VERSIONDLG_H

#include <QDialog>

namespace Ui {
class VersionDlg;
}

class VersionDlg : public QDialog
{
    Q_OBJECT

public:
    explicit VersionDlg(QWidget *parent = 0);
    ~VersionDlg();

private slots:
    void on_btn_Ok_clicked();

private:
    Ui::VersionDlg *ui;
};

#endif // VERSIONDLG_H

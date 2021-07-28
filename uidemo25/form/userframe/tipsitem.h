#ifndef TIPSITEM_H
#define TIPSITEM_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTimer>

namespace Ui {
class TipsItem;
}

class TipsItem : public QWidget
{
    Q_OBJECT

public:
    explicit TipsItem(QString tim,QString msg,QWidget *parent = 0);
    ~TipsItem();

public:
    void setContext(QString tim,QString msg);
    QString getId()
    {
        return m_id;
    }

private slots:
    void on_btn_close_clicked();
    void checkclose();

private:
    Ui::TipsItem *ui;

signals:
    void closeTips(QString idStr);

private:
    QString m_id;
    QTimer *m_timer;     ///< 3秒定时发送关闭消息
};

class tipsGroup : public QWidget
{
    Q_OBJECT

public:
    explicit tipsGroup(QWidget *parent = 0);
    ~tipsGroup();

public:
    void AddItems(TipsItem* item);

public slots:
    void DeleteItem(QString id);

private:
    QVector<TipsItem*> m_currentItem;
    QVBoxLayout *hlayout = nullptr;
    QMutex m_mutex;                 ///< 互斥信号量

private:
    void UpdateUI();
};

#endif // TIPSITEM_H

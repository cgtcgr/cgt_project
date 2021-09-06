#ifndef DATAPOINT_H
#define DATAPOINT_H

#include <QWidget>
#include "task/taskmanager.h"
#include <QtCharts/QChartView>//显示图表
#include <QtCharts/QLineSeries>//线系列
#include <QList>
#include "userframe/chartviewcgt.h"
namespace Ui {
class dataPoint;
}

QT_CHARTS_USE_NAMESPACE//QtChart名空间

class dataPoint : public QWidget
{
    Q_OBJECT

public:
    explicit dataPoint(QWidget *parent = nullptr);
    ~dataPoint();

public slots:
    void recvTaskListChange();
private slots:
    void on_pushButton_clicked();

    void SetSpeed(QString speed);
    //void on_pushButton_2_clicked();


    void on_pushButton_2_clicked();

private:
    bool updateTaskList(std::list<taskInfo> &tasklist);
    bool Slot_ExportData();
      void init();
          QLineSeries *getSeries(int flag);
private:
    Ui::dataPoint *ui;
    QString pointName;
    QList<QLineSeries *> m_series;
//    QChartView *chartView;
//    QChartView *chartView1;
    ChartViewCgt *chartView;
    ChartViewCgt *chartView1;
    bool m_tShow;
    bool m_maxShow;
    bool m_minShow;
};

#endif // DATAPOINT_H

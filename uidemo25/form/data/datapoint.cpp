#include "datapoint.h"
#include "ui_datapoint.h"
#include "task/periodtask.h"
#include <QDebug>
#include "sql/sqlmanager.h"
#include "xlsxdocument.h"
#include "xlsxformat.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include <QFileDialog>
#include <QGridLayout>
#include "userframe/frmessagebox.h"
#include "mount/commonconfig.h"
QT_CHARTS_USE_NAMESPACE//QtChart名空间

dataPoint::dataPoint(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dataPoint),
    m_tShow(false),
    m_maxShow(true),
    m_minShow(true)
{
    ui->setupUi(this);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);		//设置整行选择
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);		//设置只选择一行

    connect(PeriodTask::getInstance(),SIGNAL(sendTaskListChange()),this ,SLOT(recvTaskListChange()));
    std::list<taskInfo> task;
    PeriodTask::getInstance()->getTaskList(task);
    updateTaskList(task);
    //设置默认显示值的索引，从0开始
    ui->comboBox->setCurrentIndex(1);
    pointName = ui->comboBox->currentText();
    //关联信号和槽
    QObject::connect(ui->comboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(SetSpeed(QString)));
    init();
}

QLineSeries * dataPoint::getSeries(int flag)
{
    QLineSeries *series = new QLineSeries();
//设置线条名称
if(flag == 0)
{
    series->setName(QString("阴极钢棒"));
    series->setColor(QColor(255,0,255));
    series->setVisible(true);
    series->setPointLabelsVisible(true);
    series->setPointLabelsColor(QColor(255,255,255));
    series->setPointLabelsFont(QFont("微软雅黑"));
    series->setPointLabelsFormat("(@yPoint)");
    series->setPointsVisible(true);
}
else
{
    series->setName(QString("槽底"));
    //series->setName(QString("line " + QString::number(m_series.count())));
    //设置线条颜色，如果不设置会给默认颜色

    series->setColor(QColor(0,255,255));
    //series1->setBrush(QColor(255,0,255));
    //series1->setPen(QColor(255,0,255));
    //设置是否线条可视

    series->setVisible(false);
    //点标签是否可视

    series->setPointLabelsVisible(true);
    //点标签颜色

    series->setPointLabelsColor(QColor(255,255,255));
    //点标签字体

    series->setPointLabelsFont(QFont("微软雅黑"));
    //设置点标签显示格式  //(@xPoint,@yPoint)

    series->setPointLabelsFormat("(@yPoint)");
    //是否切割边缘点标签，默认为true
    //series->setPointLabelsClipping(false);
    //series->setPointLabelsClipping(true);
    //设置点标签是否可视

    series->setPointsVisible(true);
}
return series;
}

void dataPoint::init()
{
        //QList<QLineSeries *> m_series;
        QLineSeries *series1 = new QLineSeries();//实例化一个QLineSeries对象
        QLineSeries *series2 = new QLineSeries();//实例化一个QLineSeries对象
        m_series.append(series1);
        m_series.append(series2);
        //设置线条名称

        series1->setName(QString("cathode steelbar"));
        series2->setName(QString("cell bottom"));

        //设置线条颜色，如果不设置会给默认颜色
        //series1->setColor(QColor(255,0,255));
        //series2->setColor(QColor(0,255,255));
        //series1->setBrush(QColor(255,0,255));
        //series1->setPen(QColor(255,0,255));

        QPen pen;  // creates a default pen
       // pen.setStyle(Qt::DashDotLine);
        pen.setWidth(5);
        pen.setColor(QColor(255,0,255));
        pen.setCapStyle(Qt::RoundCap);
        pen.setJoinStyle(Qt::RoundJoin);
        series1->setPen(pen);

        pen.setColor(QColor(0,255,255));
        series2->setPen(pen);
        //设置是否线条可视
        series1->setVisible(true);
        series2->setVisible(true);
        //点标签是否可视
       // series1->setPointLabelsVisible(true);
        //series2->setPointLabelsVisible(true);
        //点标签颜色
        series1->setPointLabelsColor(QColor(255,255,255));
        series2->setPointLabelsColor(QColor(255,255,255));
        //点标签字体
        series1->setPointLabelsFont(QFont("微软雅黑"));
        series2->setPointLabelsFont(QFont("微软雅黑"));
        //设置点标签显示格式
        series1->setPointLabelsFormat("@yPoint");
        series2->setPointLabelsFormat("@yPoint");
        //是否切割边缘点标签，默认为true
        series1->setPointLabelsClipping(false);
        series2->setPointLabelsClipping(true);
        //设置点标签是否可视
        series1->setPointsVisible(true);
        series2->setPointsVisible(true);

        QVector<QPointF> ps1;
        QVector<QPointF> ps2;

        //添加坐标点

        ps1.push_back(QPointF(0, CommonConfig::GetInstance()->lineChartAYMin));
        ps1.push_back(QPointF(CommonConfig::GetInstance()->lineChartAXMax, CommonConfig::GetInstance()->lineChartAYMax));



        ps2.push_back(QPointF(0, CommonConfig::GetInstance()->lineChartBYMin));
        ps2.push_back(QPointF(CommonConfig::GetInstance()->lineChartBXMax, CommonConfig::GetInstance()->lineChartBYMax));

        m_series.at(0)->replace(ps1);
        m_series.at(1)->replace(ps2);


        QChart *chart = new QChart();
        chart->setTheme(QChart::ChartThemeBlueCerulean);//设置系统主题
        chart->setAnimationOptions(QChart::AllAnimations);//设置启用或禁用动画
        chart->setBackgroundBrush(QBrush(QColor(170,170,255)));//设置背景色,主题和背景二选一
        chart->setDropShadowEnabled(true);//是否背景阴影
        chart->setLocalizeNumbers(true);//数字是否本地化
        //chart->legend()->show();//legend是否显示，show和hide
        chart->addSeries(series1);//添加系列到QChart上
        chart->createDefaultAxes();//创建默认轴
        chart->setTitle(QString("cathode steelbar"));//设置标题
        chart->setTitleBrush(QBrush(QColor(255,170,255)));//设置标题Brush
        chart->setTitleFont(QFont("微软雅黑"));//设置标题字体

        chart->legend()->setVisible(true);
        chart->legend()->setAlignment(Qt::AlignBottom);//底部对齐
        //chart->legend()->detachFromChart();

        chart->legend()->setBackgroundVisible(true);//设置背景是否可视
        //chart->legend()->setAutoFillBackground(true);//设置背景自动填充
        //chart->legend()->setColor(QColor(255,128,128,128));//设置颜色
        //chart->legend()->setContentsMargins(10,10,10,10);//设置边距left,top,right,bottom
        chart->legend()->setLabelColor(QColor(255,128,255));//设置标签颜色
        chart->legend()->setVisible(false);//设置是否可视
          //chart->legend()->hide();
        chart->legend()->setMaximumHeight(40);
        chart->legend()->setMaximumWidth(150);
        //chart->legend()->setMaximumSize(10000);
        //chart->legend()->setGeometry(50,50,120,50);//设置几何位置x,y,w,h
        //chart->legend()->setBrush(QBrush(QColor(128,128,128,128)));
        //chart->legend()->setPen(QPen(QColor(192,192, 192,192)));
        chart->legend()->setBorderColor(QColor(255,255,170,185));//设置边框颜色

        QFont font = chart->legend()->font();
        font.setItalic(!font.italic());
       // chart->legend()->setFont(font);//设置字体为斜体
        font.setPointSizeF(8);
        chart->legend()->setFont(font);//设置字体大小
        chart->legend()->setFont(QFont("微软雅黑"));//设置字体类型

        QChart *chart1 = new QChart();
        chart1->setTheme(QChart::ChartThemeBlueCerulean);//设置系统主题
        chart1->setAnimationOptions(QChart::AllAnimations);//设置启用或禁用动画
        chart1->setBackgroundBrush(QBrush(QColor(170,170,255)));//设置背景色,主题和背景二选一
        chart1->setDropShadowEnabled(true);//是否背景阴影
        chart1->setLocalizeNumbers(true);//数字是否本地化
        //chart1->legend()->show();//legend是否显示，show和hide
        chart1->addSeries(series2);//添加系列到QChart上
        chart1->createDefaultAxes();//创建默认轴
        chart1->setTitle(QString("cell bottom"));//设置标题
        chart1->setTitleBrush(QBrush(QColor(255,170,255)));//设置标题Brush
        chart1->setTitleFont(QFont("微软雅黑"));//设置标题字体

        chart1->legend()->setVisible(true);
        chart1->legend()->setAlignment(Qt::AlignBottom);//底部对齐
        //chart1->legend()->detachFromChart();
        chart1->legend()->setBackgroundVisible(true);//设置背景是否可视
        //chart1->legend()->setAutoFillBackground(true);//设置背景自动填充
        //chart1->legend()->setColor(QColor(255,128,128,128));//设置颜色
        //chart1->legend()->setContentsMargins(10,10,10,10);//设置边距left,top,right,bottom
        chart1->legend()->setLabelColor(QColor(255,128,255));//设置标签颜色
        chart1->legend()->setVisible(false);//设置是否可视
        //chart1->legend()->setMaximumHeight(50);
        //chart1->legend()->setMaximumWidth(120);
        //chart1->legend()->setMaximumSize(10000);
        //chart1->legend()->setGeometry(50,50,120,50);//设置几何位置x,y,w,h
        //chart1->legend()->setBrush(QBrush(QColor(128,128,128,128)));
        //chart1->legend()->setPen(QPen(QColor(192,192, 192,192)));
        chart1->legend()->setBorderColor(QColor(255,255,170,185));//设置边框颜色
        chart1->legend()->setFont(font);//设置字体大小
        chart1->legend()->setFont(QFont("微软雅黑"));//设置字体类型

        chartView = new ChartViewCgt();
        chartView->setChart(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        QGridLayout * gridLayout = new QGridLayout(ui->widget);
        gridLayout->addWidget(chartView,0,0);

        chartView1 = new ChartViewCgt();
        chartView1->setChart(chart1);
        chartView1->setRenderHint(QPainter::Antialiasing);
        QGridLayout * gridLayout1 = new QGridLayout(ui->widget_3);
        gridLayout1->addWidget(chartView1,0,0);
}

void dataPoint::SetSpeed(QString speed)
{
    qDebug()<<speed;
    pointName = speed;
}

void dataPoint::recvTaskListChange()
{
    qDebug()<<"dataPoint::recvTaskListChange";
    std::list<taskInfo> task;
    PeriodTask::getInstance()->getTaskList(task);
    updateTaskList(task);
}

dataPoint::~dataPoint()
{
    delete ui;
}

bool dataPoint::updateTaskList(std::list<taskInfo>& tasklist)
{
    auto it = tasklist.begin();
    for(; it != tasklist.end();it ++)
    {
        ui->comboBox->addItem(it->pointName);
    }
    return true;
}

void dataPoint::on_pushButton_clicked()
{
//    frmessagebox::Instance()->setMessage("111",0);
//    frmessagebox::Instance()->show();
    ui->tableWidget->clearContents();
    QString startTime = ui->dateTimeEdit->text();
    QString endTime = ui->dateTimeEdit_2->text();
    QLOG_INFO()<<"dataPoint search"<<startTime<<endTime<<pointName;
    QStringList value;
    sqlManager::GetInstance()->getValuesWhereTime("taskResult",value,startTime,endTime,pointName);

    m_series.at(0)->clear();
    m_series.at(1)->clear();
    QVector<QPointF> s1;
    QVector<QPointF> s2;
    if(!value.empty() && value.size()%7 == 0)
    {
        ui->tableWidget->setRowCount(value.size()/7);
        for(int i = 0; i<value.size()/7 ;i++)
        {
            QTableWidgetItem *name1= new QTableWidgetItem(value.at(i*7+2));
            ui->tableWidget->setItem(i,0,name1);
            QTableWidgetItem *name2=new QTableWidgetItem(value.at(i*7+1));
            ui->tableWidget->setItem(i,1,name2);
            QString max = QString::number((double)(value.at(i*7+4).toInt())/1000.0, 'f', 1);
            //m_series.at(0)->append(i, (double)(value.at(i*7+4).toInt())/1000.0);
            s1.push_back(QPointF(i, (value.at(i*7+4).toInt())/1000));
            QTableWidgetItem *name3= new QTableWidgetItem(max);
            ui->tableWidget->setItem(i,2,name3);
            QString min = QString::number((double)(value.at(i*7+5).toInt())/1000.0, 'f', 1);
           // m_series.at(1)->append(i, (double)(value.at(i*7+5).toInt())/1000.0);
            s2.push_back(QPointF(i, (value.at(i*7+5).toInt())/1000));
            QTableWidgetItem *name4=new QTableWidgetItem(min);
            ui->tableWidget->setItem(i,3,name4);
        }
        ui->tableWidget->scrollToBottom ();
        ui->tableWidget->show();
    }
    m_series.at(0)->replace(s1);
    m_series.at(1)->replace(s2);
}

bool dataPoint::Slot_ExportData()
{
    bool bRet = false;

    QString filename = "";
    filename += QDateTime::currentDateTime().toString("MM-dd-hh-mm");
    QString filepath = QFileDialog::getSaveFileName(this, tr("Save as..."),
        filename, tr("EXCEL files (*.xls);;HTML-Files (*.txt);;"));
    qDebug()<<"Slot_ExportDataq";
    if (filepath != "")
    {
        int row = ui->tableWidget->rowCount();
        int col = ui->tableWidget->columnCount();

        QXlsx::Document xlsx;
        QXlsx::Format format1;
       // format1.setFontColor(QColor(Qt::red));/*文字为红色*/
        format1.setPatternBackgroundColor(QColor(255,220,220));
        format1.setHorizontalAlignment(QXlsx::Format::AlignHCenter);/*横向居中*/
        //format1.setBorderStyle(QXlsx::Format::BorderThin);/*边框样式*/

        QXlsx::Format format2;
        format2.setFontColor(QColor(Qt::red));/*文字为红色*/
        format2.setPatternBackgroundColor(QColor(255,220,220));
        format2.setHorizontalAlignment(QXlsx::Format::AlignHCenter);/*横向居中*/
        format2.setBorderStyle(QXlsx::Format::BorderThin);/*边框样式*/
        //qDebug()<<"Slot_ExportDataq2";
        xlsx.setColumnWidth(1,100,35);
        xlsx.setRowHeight(1,100,25);
        for (int i = 0; i < col; i++)
        {
             QString str = ui->tableWidget->horizontalHeaderItem(i)->text();
             xlsx.write(1,i+1, str, format2);
        }
        //qDebug()<<"Slot_ExportDataq3"<<row<<col;
        int current_row = 0;
        for (int i = 0; i < row; i++)
        {
             for (int j = 0; j < col; j++)
             {
                  //qDebug()<<"Slot_ExportDataq000"<<i<<j<<ui->tableWidget->item(i, j)->text();
                QString rowStr;
                if(ui->tableWidget->item(i,j)!=nullptr)
                {
                    rowStr = ui->tableWidget->item(i, j)->text();
                }
               // qDebug()<<"Slot_ExportDataq111"<<i<<j<<rowStr;
                xlsx.write(i+2,j+1, rowStr, format1);
                //qDebug()<<"Slot_ExportDataq222"<<i<<j<<rowStr;
                current_row = i+2;
             }
        }
        current_row+=3;
       // qDebug()<<"Slot_ExportDataq4";
//        xlsx.write(current_row,1, type, format1);
//        QXlsx::CellRange range(current_row,1,current_row,3);
//        xlsx.mergeCells(range,format1);

//        xlsx.write(++current_row,1, serial, format1);
//        QXlsx::CellRange range1(current_row,1,current_row,3);
//        xlsx.mergeCells(range1,format1);

//        xlsx.write(++current_row,1, softVersion, format1);
//        QXlsx::CellRange range2(current_row,1,current_row,3);
//        xlsx.mergeCells(range2,format1);

        xlsx.write(++current_row,1, QString(QObject::tr("生成时间 :")) +QDateTime::currentDateTime().toString("MM-dd hh:mm"), format1);
        QXlsx::CellRange range3(current_row,1,current_row,3);
        xlsx.mergeCells(range3,format1);


//        QXlsx::Chart *pieChart = xlsx.insertChart(4, 5, QSize(300, 300));/*在3行5列的位置插入一个图标*/
//        pieChart->setChartType(QXlsx::Chart::CT_Pie);/*插入一个饼形图*/
//        pieChart->addSeries(QXlsx::CellRange("G1:G3"));/*饼形图添加数据*/

          bRet = xlsx.saveAs(filepath);
          if(bRet)
          {
            QLOG_INFO()<<"Slot_ExportDataq4 success";
          }
          else {
              QLOG_INFO()<<"Slot_ExportDataq4 fail";
          }
    }
    return bRet;
}


void dataPoint::on_pushButton_2_clicked()
{
    Slot_ExportData();
}

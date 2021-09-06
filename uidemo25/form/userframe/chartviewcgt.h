#ifndef CHARTVIEWCGT_H
#define CHARTVIEWCGT_H

#include <QObject>
#include <QWidget>
#include <QChartView>

QT_CHARTS_USE_NAMESPACE

class ChartViewCgt: public QChartView
{
    Q_OBJECT
public:
    ChartViewCgt();
    void saveAxisRange();

    protected:
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);
        void wheelEvent(QWheelEvent *event);
        //void keyPressEvent(QKeyEvent *event);
        //void keyReleaseEvent(QKeyEvent *event); 这里两句是原博主保留的 我这里注释掉了

    //以下3个为Qt Android下Qchart的缩放（单指触点时默认为鼠标点击，所以移动功能可正常使用）
       // bool event(QEvent *event) override;             //使用手势实现缩放
       // bool gestureEvent(QGestureEvent *event);
       // void pinchTriggered(QPinchGesture *gesture);     这三个为Android的操作，我也注释掉了

    private:
        QPoint m_lastPoint;
        bool m_isPress = false;  //这里我把平移的判定初始化成了false，解决鼠标一移进QchartView就跟随移动的情况
        bool m_ctrlPress;
        bool m_alreadySaveRange;
        double m_xMin, m_xMax, m_yMin, m_yMax;
        QGraphicsSimpleTextItem* m_coordItem;
};

#endif // CHARTVIEWCGT_H

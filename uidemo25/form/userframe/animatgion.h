#ifndef ANIMATGION_H
#define ANIMATGION_H

#include <QObject>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QWidget>

#define ANIMATION_COVER

class animatgion
{

public:

    static void wigShow(QWidget *wig, int direct, int flag =0, QWidget *tmpWig=nullptr,int timeVal=500)
    {
        int w = wig->size().width();
        int h = wig->size().height();

        switch (flag)
        {
            case 0:
            {
                int startx = (direct > 0)? -w : w;
                QPropertyAnimation *animal1= new QPropertyAnimation(wig,"pos");
                animal1->setDuration(timeVal);
                animal1->setStartValue(QPoint(startx,0));
                animal1->setEndValue(QPoint(0,0));
                animal1->setEasingCurve(QEasingCurve::OutCubic);
                animal1->start();
            }
                break;
            case 1:
            {
                int startx = (direct > 0)? -w : w;
                QPropertyAnimation *animal1= new QPropertyAnimation(wig,"pos");
                animal1->setDuration(timeVal);
                animal1->setStartValue(QPoint(startx,0));
                animal1->setEndValue(QPoint(0,0));
                animal1->setEasingCurve(QEasingCurve::OutCubic);

                QPropertyAnimation *animal2= new QPropertyAnimation(tmpWig,"pos");
                animal2->setDuration(timeVal);
                animal2->setStartValue(QPoint(0,0));
                animal2->setEndValue(QPoint(-startx,0));
                animal2->setEasingCurve(QEasingCurve::OutCubic);

                QParallelAnimationGroup *group = new QParallelAnimationGroup;
                group->addAnimation(animal1);
                group->addAnimation(animal2);
                group->start();
            }
                break;
            case 2:
            {
                if(direct > 0){//从左上角慢慢变大出现
                    QPropertyAnimation *animal1= new QPropertyAnimation(wig,"size");
                    animal1->setDuration(timeVal);
                    animal1->setStartValue(QSize(0,0));
                    animal1->setEndValue(QSize(w,h));
                    animal1->setEasingCurve(QEasingCurve::OutCubic);
                    animal1->start();
                }else{  //右下角慢慢变大出现
                    QPropertyAnimation *animal1= new QPropertyAnimation(wig,"geometry");
                    animal1->setDuration(timeVal);
                    animal1->setStartValue(QRect(w,h, 0, 0));
                    animal1->setEndValue(QRect(0,0, w, h));
                    animal1->setEasingCurve(QEasingCurve::OutCubic);
                    animal1->start();
                }
            }
                break;
            case 3:
            {
                if(direct > 0){//从左上角慢慢变大出现
                    QPropertyAnimation *animal1= new QPropertyAnimation(wig,"size");
                    animal1->setDuration(timeVal);
                    animal1->setStartValue(QSize(0,0));
                    animal1->setEndValue(QSize(w,h));
                    animal1->setEasingCurve(QEasingCurve::OutCubic);
                    animal1->start();
                }else{  //右下角慢慢变大出现
                    QPropertyAnimation *animal1= new QPropertyAnimation(wig,"geometry");
                    animal1->setDuration(timeVal);
                    animal1->setStartValue(QRect(w,h, 0, 0));
                    animal1->setEndValue(QRect(0,0, w, h));
                    animal1->setEasingCurve(QEasingCurve::OutCubic);
                    animal1->start();
                }
            }
                break;
            case 4:
            {
                if(direct > 0){//从左上角慢慢变大出现
                    QPropertyAnimation *animal1= new QPropertyAnimation(wig,"size");
                    animal1->setDuration(timeVal);
                    animal1->setStartValue(QSize(0,0));
                    animal1->setEndValue(QSize(w,h));
                    animal1->setEasingCurve(QEasingCurve::OutCubic);
                    wig->raise();
                    animal1->start();
                }
            }
            break;
            case 5:
            {
                QPropertyAnimation *animal1= new QPropertyAnimation(wig,"geometry");
                animal1->setDuration(timeVal);
                animal1->setStartValue(QRect(w/2,h/2, 0, 0));
                animal1->setEndValue(QRect(0,0, w, h));
                animal1->start();
            }
            break;
            case 6:
            {
                QPropertyAnimation *animal1= new QPropertyAnimation(wig,"geometry");
                animal1->setDuration(timeVal);
                animal1->setStartValue(QRect(w/2,h/2, 0, 0));
                animal1->setEndValue(QRect(0,0, w, h));
                animal1->setEasingCurve(QEasingCurve::OutCubic);
                animal1->start();
            }
            break;
            case 7:
            {
                if(!tmpWig){return;}
                QPropertyAnimation *animal1= new QPropertyAnimation(wig,"geometry");
                animal1->setDuration(timeVal);
                (direct > 0)? animal1->setStartValue(QRect(0,0, 0, 0)):animal1->setStartValue(QRect(w,h, 0, 0));
                (direct > 0)? animal1->setEndValue(QRect(0,0, w, h)):animal1->setEndValue(QRect(0,0, w, h));

                QPropertyAnimation *animal2= new QPropertyAnimation(tmpWig,"geometry");
                animal2->setDuration(timeVal);
                (direct > 0)? animal2->setStartValue(QRect(0,0, w, h)):animal2->setStartValue(QRect(0,0, w, h));
                (direct > 0)? animal2->setEndValue(QRect(w,h, 0, 0)):animal2->setEndValue(QRect(0,0, 0, 0));

                QParallelAnimationGroup *group = new QParallelAnimationGroup;
                group->addAnimation(animal1);
                group->addAnimation(animal2);
                group->start();
            }
            break;
            default:
                break;
        };
    }
    static void wigMove(QWidget *pWidget)
    {
        int nRange = 1;
        int nX = pWidget->x();
            int nY = pWidget->y();
            QPropertyAnimation *pAnimation = new QPropertyAnimation(pWidget,"geometry");
            pAnimation->setEasingCurve(QEasingCurve::InOutSine);
            pAnimation->setDuration(300);
            pAnimation->setStartValue(QRect(QPoint(nX,nY),pWidget->size()));

            int nShakeCount = 20; //抖动次数
            double nStep = 1.0/nShakeCount;
            for(int i = 1; i < nShakeCount; i++){
                nRange = i&1 ? -nRange : nRange;
                pAnimation->setKeyValueAt(nStep*i,QRect(QPoint(nX + nRange,nY),pWidget->size()));
            }

            pAnimation->setEndValue(QRect(QPoint(nX,nY),pWidget->size()));
            pAnimation->start(QAbstractAnimation::DeleteWhenStopped);

    }
};

#endif // ANIMATGION_H

#ifndef TEMPMANAGER_H
#define TEMPMANAGER_H

#include <QObject>

class tempManager
{
public:
    static tempManager* getInstance();
    tempManager();
    int m_maxT;
    int m_minT;
public slots:
    void getTem(int &max, int &min)
    {
        max = m_maxT;
        min = m_minT;
    }
    void setTem(int &max, int &min)
    {
       m_maxT =  max ;
       m_minT =  min ;
    }
};

#endif // TEMPMANAGER_H

#ifndef TEMPMANAGER_H
#define TEMPMANAGER_H

#include <QObject>

class tempManager
{
public:
    static tempManager* getInstance();
    tempManager();
    int m_max0T;
    int m_min0T;
    int m_max1T;
    int m_min1T;
    void settem(int id, int max, int min);

    void gettem(int &maxL, int &maxR);
};

#endif // TEMPMANAGER_H

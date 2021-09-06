#include "tempmanager.h"


tempManager *tempManager::getInstance()
{
    static tempManager instance;
    return  &instance;
}

tempManager::tempManager():
    m_max0T(0),
    m_min0T(0),
    m_max1T(0),
    m_min1T(0)
{

}
void tempManager::settem(int id , int max ,int min)
{
    if(id ==0)
    {
        m_max0T = max;m_min0T = min;
    }
    else if(id == 1)
    {
        m_max1T = max;m_min1T = min;
    }
}

void tempManager::gettem(int &maxL ,int &maxR)
{
    maxL = m_max0T;
    maxR = m_max1T;
}

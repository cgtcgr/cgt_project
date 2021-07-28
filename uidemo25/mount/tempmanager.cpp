#include "tempmanager.h"


tempManager *tempManager::getInstance()
{
    static tempManager* instance;
    return  instance;
}

tempManager::tempManager():
    m_maxT(0),
    m_minT(0)
{

}

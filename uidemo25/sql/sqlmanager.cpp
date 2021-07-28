#include "sqlmanager.h"


sqlManager *sqlManager::GetInstance()
{
    static sqlManager instance;
    return &instance;
}

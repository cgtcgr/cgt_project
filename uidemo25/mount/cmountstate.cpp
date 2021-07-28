#include "cmountstate.h"

CMountState *CMountState::GetInstance()
{
    static CMountState instance;
    return &instance;
}

CMountState::CMountState():m_autoFlag(true)
{

}


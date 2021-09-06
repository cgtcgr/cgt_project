#ifndef CMOUNTSTATE_H
#define CMOUNTSTATE_H

#include <QObject>
#include "subform/mountinfoshow.h"
class CMountState:public QObject
{
    Q_OBJECT
public:
    static CMountState *GetInstance();
    bool getAutoState(){return m_autoFlag;}
    bool setAutoState(bool flag)
    {
        m_autoFlag = flag;
        emit sendMountState(flag);
        return true;
    }

private:
    CMountState();
signals:
    void sendMountState(bool);
private:
    bool m_autoFlag;
};

#endif // CMOUNTSTATE_H

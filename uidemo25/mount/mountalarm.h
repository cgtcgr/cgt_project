#ifndef MOUNTALARM_H
#define MOUNTALARM_H
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>

class mountAlarm:public QObject
{
    Q_OBJECT
public:
    static mountAlarm *GetInstance();
    void sendData(QString str);
    void setLowPower(bool flag, int power);
    void setMotorFault(bool flag);
public slots:
    void newconnected();
    void disconnected();
    void recvData();
private slots:
    void sendPowerAlarm();
    void sendMotorAlarm();
private:
    mountAlarm();
    QTcpServer * ser;
    QTcpSocket *cli;
    bool m_connect;
    QString powerStr;
    QString motorStr;
    QTimer  * powerAlarm;
    QTimer  * motorAlarm;
    int sendTimeOut;
private:
    void saveFile(QString str);
};

#endif // MOUNTALARM_H

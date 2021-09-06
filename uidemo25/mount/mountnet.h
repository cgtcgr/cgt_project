#ifndef MOUNTNET_H
#define MOUNTNET_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QMutex>
#include <condition_variable>
#include "xsocketclient.h"
#include "smtstyle.h"

#define MOUNTPOSNEAR 5
#define SWINGPOSNEAR 5
class MountNet : public QObject
{
    Q_OBJECT
public:
    struct mountData
    {
        int horData;//水平数据
        int verData;//垂直数据
        int TM;//温度
        int DM;//湿度
        int H2S;
        int CO;
        int PV;//烟雾
        int CH4;
        int RAN;//噪音
        int power;//电量
        bool operator =(QVector<int> &data)
        {
             horData = data.at(0);
             verData = data.at(1);
             TM = data.at(2);
             DM = data.at(3);
             H2S = data.at(4);
             CO = data.at(5);
             PV = data.at(6);
             CH4 = data.at(7);
             RAN = data.at(8);
             power = data.at(9);
             return true;
        }
    };
    static MountNet *GetInstance();
    bool init();
    bool setIpPort(QString ip, int p);

    bool sendCmd(QString cmd, int distance);
    bool sendMoveMount(bool flag);
    bool sendMoveSwingA(bool flag);
    bool sendMoveSwingB(bool flag);
    bool sendMoveStop();
    bool resetmountPos()
    {
        PackGo0(0);
        return true;
    }
    bool setRoom(QString room);
    bool sendMovePsotion(int pos, int flag, int posbak=0);
    bool sendMoveSpeed(int speed);
    int getMountPos(){return m_mountPos;}
    int getSwingAPos(){return m_swingPosA;}
    int getSwingBPos(){return m_swingPosB;}

    void PackJG(int sta);
    bool sendSwingMoveSpeed(int speed);
public slots:
    void readyExcute();
    void OnDataReady();
private:
    MountNet();
    MountNet(QString ip, int port);
    bool disconnect();
    bool parseData(QByteArray &buffer);
    void PackStop(int endmsg=0);
    void PackStart(unsigned char dirX, int X, unsigned char dirY, int Y, unsigned char dirZ, int Z);
    void Unpack(unsigned char *buff);
    void UnpackNow(unsigned char *buff);
    void PackGo0(int endmsg);
private slots:
    void ReadData(QByteArray buffer);
    void ReadError(QAbstractSocket::SocketError);
    bool sendHeartBeat();
signals:
    void mountPos(int);
    void sendMMXY(int,int ,int);//更新显示坐标
    void sendMountState(int,int ,int,uchar,uchar,short);//更新显示坐标
private:
    QTcpSocket * m_tcpSocket;
    QTimer * m_timer;
    QString m_ip;
    int m_port;
    bool m_connect;
    mountData m_data;
    std::condition_variable m_cond;
    std::mutex control_mutex_;
    XSocketClient socketClient;
    int m_mountSpeed;
    int m_swingSpeed;

    int m_mountPos;
    int m_swingPosA;
    int m_swingPosB;

    int m_mountMostLimit;
    int m_swingMostLimit;


    void onSet(int endMsg);
};

#endif // MOUNTNET_H

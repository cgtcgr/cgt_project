#ifndef STEERINGNET_H
#define STEERINGNET_H

//内部不保证线程安全，请注意

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QMutex>
#include <condition_variable>
#include "xsocketclient.h"
#include "QsLog.h"

typedef		char			s8;
typedef		unsigned char	u8;
typedef		unsigned short	u16;
typedef		short			s16;
typedef		unsigned long	u32;
typedef		long			s32;

class steeringNet : public QObject
{
    Q_OBJECT
public:
    static steeringNet *GetInstance(int c);
    bool init();
    bool setIpPort(QString ip, int p);
    bool WritePos(u8 ID, u16 Position, u16 Time, u16 Speed = 0);//普通写位置指令!!!!!
    bool readWord(u8 ID, u8 MemAddr);//读2个字节
    int ReadPos(u8 ID);//读位置
    int ReadMaxAngle(u8 ID);
    int ReadMinAngle(u8 ID);
    int Ping(u8 ID);//Ping指令
    bool Reset(u8 ID);//复位舵机参数为默认值

public:
    u8	Level;//舵机返回等级
    u8	End;//处理器大小端结构

    bool MoveFrontStep();
    bool MoveBackStep();
private:
    steeringNet(int c);
    bool disconnect();
    bool parseData(QByteArray &buffer);

    void writeBuf(u8 ID, u8 MemAddr, u8 *nDat, u8 nLen, u8 Fun);
    int writePos(u8 ID, u16 Position, u16 Time, u16 Speed, u8 Fun);
    void Host2SCS(u8 *DataL, u8* DataH, int Data);//1个16位数拆分为2个8位数
    int	SCS2Host(u8 DataL, u8 DataH);//2个8位数组合为1个16位数
private slots:
    void ReadData(QByteArray buffer);
    void ReadError(QAbstractSocket::SocketError);

private:
    int m_channel;// 0 ,1
    QTcpSocket * m_tcpClient;
    QTimer * m_timer;
    QString m_ip;
    int port;
    bool m_connect;
    std::condition_variable m_cond;
    std::mutex control_mutex_;
    XSocketClient socketClient;
    u16 m_currentPos;
    u16 m_minAngle;
    u16 m_maxAngle;
    u8 m_devState;
    int m_readState;//0 init 1 devstate 2 m_minAngle 3 m_maxAngle 4 m_currentPos


    #define		B_1M		0
    #define		B_0_5M		1
    #define		B_250K		2
    #define		B_128K		3
    #define		B_115200	4
    #define		B_76800		5
    #define		B_57600		6
    #define		B_38400		7

    //register Address
    #define P_VERSION_L 3 //软件版本
    #define P_VERSION_H 4
    #define P_ID 5
    #define P_BAUD_RATE 6
    #define P_RETURN_DELAY_TIME 7
    #define P_RETURN_LEVEL 8
    #define P_MIN_ANGLE_LIMIT_L 9 //最小角度限制
    #define P_MIN_ANGLE_LIMIT_H 10
    #define P_MAX_ANGLE_LIMIT_L 11//最大角度限制
    #define P_MAX_ANGLE_LIMIT_H 12
    #define P_LIMIT_TEMPERATURE 13
    #define P_MAX_LIMIT_VOLTAGE 14
    #define P_MIN_LIMIT_VOLTAGE 15
    #define P_MAX_TORQUE_L 16
    #define P_MAX_TORQUE_H 17
    #define P_ALARM_LED 19
    #define P_ALARM_SHUTDOWN 20
    #define P_COMPLIANCE_P 21
    #define P_PUNCH_L 24
    #define P_PUNCH_H 25
    #define P_CW_DEAD 26
    #define P_CCW_DEAD 27

    #define P_TORQUE_ENABLE 40
    #define P_LED 41
    #define P_GOAL_POSITION_L 42 //目标位置（L）
    #define P_GOAL_POSITION_H 43 //
    #define P_GOAL_TIME_L 44//运行时间（L）
    #define P_GOAL_TIME_H 45
    #define P_GOAL_SPEED_L 46//运行速度（L）
    #define P_GOAL_SPEED_H 47
    #define P_LOCK 48//锁标志

    #define P_PRESENT_POSITION_L 56//当前位置（L）
    #define P_PRESENT_POSITION_H 57
    #define P_PRESENT_SPEED_L 58//当前速度（L）
    #define P_PRESENT_SPEED_H 59
    #define P_PRESENT_LOAD_L 60
    #define P_PRESENT_LOAD_H 61
    #define P_PRESENT_VOLTAGE 62
    #define P_PRESENT_TEMPERATURE 63
    #define P_REGISTERED_INSTRUCTION 64
    #define P_MOVING 66 //移动标志

    //Instruction:
    #define INST_PING 0x01
    #define INST_READ 0x02
    #define INST_WRITE 0x03
    #define INST_REG_WRITE 0x04
    #define INST_ACTION 0x05
    #define INST_RESET 0x06
    #define INST_SYNC_WRITE 0x83
};



#endif // STEERINGNET_H

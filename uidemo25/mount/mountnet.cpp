#include "mountnet.h"
#include "QsLog.h"
#include <windows.h>
MountNet *MountNet::GetInstance()
{
    static MountNet instance;
    return &instance;
}

MountNet::MountNet():
m_timer(new QTimer()),
m_ip("192.168.1.1"),
m_port(8888),
m_connect(false),
m_mountSpeed(10),
m_swingSpeed(10)
{

}

MountNet::MountNet(QString ip, int p):
m_ip(ip),
m_port(p),
m_connect(false),
m_mountSpeed(10),
m_swingSpeed(10)
{
    m_timer = new QTimer();
}

bool MountNet::setIpPort(QString ip, int p)
{
    m_ip = ip;
    m_port = p;
    return true;
}

bool MountNet::setRoom(QString room)
{
    return true;
}

//外部保证线程安全
bool MountNet::init()
{
    socketClient.ConnectTo(m_ip,m_port);
    connect(&socketClient, SIGNAL(signalOnReceiveData(QByteArray)), this, SLOT(ReadData(QByteArray)));

    if(socketClient.IsConnected())  // 连接成功则进入if{}
    {
        QLOG_INFO()<<"connect success";
        m_connect = true;
    }
    return true;
}

bool MountNet::disconnect()
{
    m_connect = false;
    return socketClient.disconnect();
}


//slots
void MountNet::ReadData(QByteArray buffer)
{
    if(buffer.isEmpty())
    {
        QLOG_INFO()<<"recv data error";
    }
    QLOG_INFO()<<"recv data" << buffer;

    unsigned char* buff=(unsigned char*)buffer.data();
    int num=buffer.size();
    num=num<COM_MAX?num:COM_MAX;
    unsigned char buffRead[COM_MAX]={0};
    for(int i=0;i<num;i++)//保存本次读取数据
    {
        buffRead[i]=buff[i];
    }
    if (COM_STX != buffRead[COM_ADD_STX])
    {//判断第一个字节是否是0xFE
        return;
    }
    Unpack(buffRead);//读取串口缓冲区的所有数据给临时变量temp
    return;

}
void MountNet::Unpack(unsigned char* buff)//解包
{
//    if (CrcIsOK(buff))
    // {
       // nAgainTime=0;//从发次数清零
       // this->setOkRun(false);//只能通信一次标志
        int nReadEndMsg = buff[COM_ADD_DAT + buff[COM_ADD_LEN] - 1];//读取结束标志
        switch (buff[COM_ADD_MSG])
        {
//        case COM_MSG_XY:this->UnpackXY(buff);break;
//        case COM_MSG_RUN:this->UnpackRun(buff);break;
//        case COM_MSG_VACUUM:this->UnpackVacuum(buff);break;
//        case COM_MSG_RUN_LED :this->UnpackRunLed(buff);break;
          case COM_MSG_NOW:this->UnpackNow(buff);break;
//        case COM_MSG_LED:this->UnpackLED(buff);break;
//        case COM_MSG_CCDLIGHT:this->UnpackCCDLight(buff);break;
//        case COM_MSG_TEST:this->UnpackTest(buff);break;
//        case COM_MSG_IAP:this->UnpackIAP(buff);break;
//        case COM_MSG_GO0:UnpackGO0(buff);break;//重新发送一次数据 保证这里不重发
//        case COM_MSG_SET:this->UnpackSet();break;
//        case COM_MSG_KEY:this->sendRunKey();break;//启动/暂停
//        case COM_MSG_STOP:this->sendRunStop();break;//停止
//        case COM_MSG_TESTFLY:this->sendRunRun();break;//启动
//        case COM_MSG_SCURVE:this->UnpackSCurve(buff);break;
        }
        buff[COM_ADD_STX]=0;
   // }
}
void MountNet::UnpackNow(unsigned char *buff)
{
    int X,Y,Y2;
    X=(buff[COM_ADD_DAT+1]&0X7F)<<24;//读取当前SMT上的XY坐标
    X|=buff[COM_ADD_DAT+2]<<16;
    X|=buff[COM_ADD_DAT+3]<<8;
    X|=buff[COM_ADD_DAT+4];
    if(buff[COM_ADD_DAT+1])X=0-X;//负数

    Y=(buff[COM_ADD_DAT+5]&0X7F)<<24;
    Y|=buff[COM_ADD_DAT+6]<<16;
    Y|=buff[COM_ADD_DAT+7]<<8;
    Y|=buff[COM_ADD_DAT+8];
    if(buff[COM_ADD_DAT+5])Y=0-Y;//负数

    Y2=(buff[COM_ADD_DAT+9]&0X7F)<<24;
    Y2|=buff[COM_ADD_DAT+10]<<16;
    Y2|=buff[COM_ADD_DAT+11]<<8;
    Y2|=buff[COM_ADD_DAT+12];
    if(buff[COM_ADD_DAT+9])Y2=0-Y2;//负数
    m_mountPos = X;
    m_swingPosA = Y;
    m_swingPosB = Y2;
    sendMMXY(X,Y,Y2);

}
void MountNet::ReadError(QAbstractSocket::SocketError)
{
    //socketClient.GetSocket->disconnect();
}

bool MountNet::sendHeartBeat()
{
    return true;
}

bool MountNet::sendMoveMount(bool flag)
{
    if(flag)
    {
        PackStart(0,100000 , 0 ,0, 0 ,0);}
    else
    {
        PackStart(1, 100000, 0, 0, 0 ,0);
    }
    return true;
}

bool MountNet::sendMoveSwingA(bool flag)
{
    if(flag)
    {
        PackStart(0,0 , 0 ,100000, 0 ,0);}
    else
    {
        PackStart(0, 0, 1, 100000, 0 ,0);
    }
    return true;
}
bool MountNet::sendMoveSwingB(bool flag)
{
    if(flag)
    {
        PackStart(0,0 , 0 ,0, 0 ,100000);}
    else
    {
        PackStart(0, 0, 0, 0, 1 ,100000);
    }
    return true;
}



bool MountNet::sendMoveStop()
{
    PackStop();
    return true;
}

bool MountNet::sendMovePsotion(int pos , int flag)//flag 0 mount 1 swingA 2 swingB
{
    int tryTimes = 6;
    if(flag == 0)
    {
        if(m_mountPos == pos)
        {
            return true;
        }
        unsigned char dir = (m_mountPos < pos) ? 1 : 0;
        PackStart(dir, abs(m_mountPos-pos), 0, 0, 0, 0);
        do
        {
            if(abs(m_mountPos-pos) <= MOUNTPOSNEAR)
            {
                return true;
            }
            Sleep(1000);
        }while(  tryTimes--);
    }
    else if(flag == 1)
    {
        if(m_swingPosA == pos)
        {
            return true;
        }
        unsigned char dir = (m_swingPosA < pos) ? 1 : 0;
        PackStart(0, 0, dir, abs(m_swingPosA-pos), 0, 0);

        do
        {
            if(abs(m_swingPosA-pos) <= SWINGPOSNEAR)
            {
                return true;
            }
            Sleep(1000);
        }while(  tryTimes--);
    }
    else if(flag == 2)
    {
        if(m_swingPosB == pos)
        {
            return true;
        }
        unsigned char dir = (m_swingPosB < pos) ? 1 : 0;
        PackStart(0, 0, dir, abs(m_swingPosB-pos), 0, 0);

        do
        {
            if(abs(m_swingPosB-pos) <= SWINGPOSNEAR)
            {
                return true;
            }
            Sleep(1000);
        }while(  tryTimes--);
    }
    return false;
}

bool MountNet::sendMoveSpeed(int speed)
{
    m_mountSpeed = speed;
    m_swingSpeed = speed;
    //下发给下位机
    return true;
}

void MountNet::PackStop(int endmsg)
{

    QByteArray buffSend;//用来存储报头与校验位   数据或命令存储在相应的处理函数中
    buffSend.resize(COM_LEN_RUN+COM_MIN);
    buffSend[COM_ADD_STX]=COM_STX;
    buffSend[COM_ADD_LEN]=COM_LEN_RUN;//数据长度
    buffSend[COM_ADD_MSG]=COM_MSG_STOP;//数据类型
    buffSend[COM_ADD_MSG_END]=endmsg;
    socketClient.SendData(buffSend);
}

void MountNet::PackStart(unsigned char dirX,int X,unsigned char dirY, int Y, unsigned char dirZ, int Z)
{
    QByteArray buffSend;//用来存储报头与校验位
    buffSend.resize(COM_LEN_RUN+COM_MIN);
    buffSend[COM_ADD_STX]=COM_STX;
    buffSend[COM_ADD_LEN]=COM_LEN_RUN;//数据长度
    buffSend[COM_ADD_MSG]=COM_MSG_XY;//数据类型
    buffSend[COM_ADD_DAT]=0;
    buffSend[COM_ADD_DAT+1]=(X>>24)&0x7f;
    buffSend[COM_ADD_DAT+2]=X>>16;
    buffSend[COM_ADD_DAT+3]=X>>8;
    buffSend[COM_ADD_DAT+4]=X;
    buffSend[COM_ADD_DAT+1]= buffSend[COM_ADD_DAT+1]|dirX<<7;
    buffSend[COM_ADD_DAT+5]=(Y>>24)&0x7f;
    buffSend[COM_ADD_DAT+6]=Y>>16;
    buffSend[COM_ADD_DAT+7]=Y>>8;
    buffSend[COM_ADD_DAT+8]=Y;
    buffSend[COM_ADD_DAT+5]=buffSend[COM_ADD_DAT+5]|dirY<<7;

    buffSend[COM_ADD_DAT+9]=(Z>>24)&0x7f;
    buffSend[COM_ADD_DAT+10]=Z>>16;
    buffSend[COM_ADD_DAT+11]=Z>>8;
    buffSend[COM_ADD_DAT+12]=Z;
    buffSend[COM_ADD_DAT+9]=buffSend[COM_ADD_DAT+9]|dirZ<<7;
    buffSend[COM_ADD_DAT+13]=100;

  //  buffSend[COM_ADD_DAT+16]|=testRun<<7;//此处添加ccd 和 是否模拟运行
    buffSend[COM_ADD_DAT+16] =0;//运行灯
    buffSend[COM_ADD_DAT+16] =0;//报警
    buffSend[COM_ADD_DAT+17] =0;//自动较正时清除对应的XYZA坐标
    buffSend[COM_ADD_DAT+17] =0;//控制是否启用飞达供气
    buffSend[COM_ADD_DAT+18]=(unsigned char)m_swingSpeed;//真空检测延时
    buffSend[COM_ADD_DAT+19]=(unsigned char)m_mountSpeed;//整机速度的百分比 默认开机是100
    buffSend[COM_ADD_MSG_END]=COM_MSG_STOP;
    socketClient.SendData(buffSend);
}


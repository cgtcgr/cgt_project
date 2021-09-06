#include "mountnet.h"
#include "QsLog.h"
#include <windows.h>
#include "commonconfig.h"

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
m_mountSpeed(CommonConfig::GetInstance()->mountSpeed),
m_swingSpeed(CommonConfig::GetInstance()->swingSpeed)
{

}

MountNet::MountNet(QString ip, int p):
m_ip(ip),
m_port(p),
m_connect(false),
m_mountSpeed(CommonConfig::GetInstance()->mountSpeed),
m_swingSpeed(CommonConfig::GetInstance()->swingSpeed)
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
    QLOG_INFO()<<"MountNet init"<<m_ip<<m_port;
    bool ret = false;
    socketClient.ConnectTo("192.168.1.240",58088);
    connect(&socketClient, SIGNAL(signalOnReceiveData(QByteArray)), this, SLOT(ReadData(QByteArray)));
    connect(m_timer, SIGNAL(timeout()), this, SLOT(readyExcute()));

    //if(socketClient.IsConnected())
    {
        QLOG_INFO()<<"mount connect success";
       // qDebug()<<"connect success";
        m_connect = true;
        ret = true;
        m_timer->start(1000);
    }

    return ret;
}

bool MountNet::disconnect()
{
    m_connect = false;
    return socketClient.disconnect();
}

void MountNet::readyExcute()
{
    //qDebug()<<"MountNet::readyExcute()";
    static int i = 0;
    i++;
    if(i>100)
    {
        m_timer->stop();
    }

    onSet(0);
}
void MountNet::OnDataReady()
{
//      QByteArray by = m_tcpSocket->readAll();
//      ReadData(by);
}
//slots
void MountNet::ReadData(QByteArray buffer)
{
    if(buffer.isEmpty())
    {
        QLOG_INFO()<<"recv data error";
    }
    //QLOG_INFO()<<"recv data1" << buffer;

    unsigned char* buff=(unsigned char*)buffer.data();
    int num=buffer.size();
    num=num<COM_MAX?num:COM_MAX;
    unsigned char buffRead[COM_MAX]={0};
    for(int i=0;i<num;i++)//保存本次读取数据
    {
        buffRead[i]=buff[i];
    }
    //QLOG_INFO()<<"recv data2";
    if (COM_STX != buffRead[COM_ADD_STX])
    {//判断第一个字节是否是0xFE
        return;
    }
     //QLOG_INFO()<<"recv data3";
    Unpack(buffRead);//读取串口缓冲区的所有数据给临时变量temp
    return;

}
void MountNet::Unpack(unsigned char* buff)//解包
{
    if(CommonConfig::GetInstance()->MountDebug)
    {
        QLOG_INFO()<<"MountNet::Unpack"<< buff;
    }

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
          case COM_MSG_POWEROUT:break;
//        case COM_MSG_CCDLIGHT:this->UnpackCCDLight(buff);break;
//        case COM_MSG_TEST:this->UnpackTest(buff);break;
//        case COM_MSG_IAP:this->UnpackIAP(buff);break;
//        case COM_MSG_GO0:UnpackGO0(buff);break;//重新发送一次数据 保证这里不重发
          case COM_MSG_SET:
            {
                QLOG_INFO()<<"recv set data stop timer";
                m_timer->stop();
            }
            break;
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
    //QLOG_INFO()<<"UnpackNow recv data4"<< buff;
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
    uchar nErrerMotor=buff[COM_ADD_DAT+13];//电机报警
    //电池电量
    uchar powerRate = buff[COM_ADD_DAT+14];
    short A=buff[COM_ADD_DAT+15]<<8;
    A|=buff[COM_ADD_DAT+16];
    if(CommonConfig::GetInstance()->MountDebug)
    {
        QLOG_INFO()<<"UnpackNow recv data"<< X<<Y<<Y2<<nErrerMotor<<powerRate<<A;
    }
    emit sendMountState(X,Y,Y2,nErrerMotor,powerRate,A);
    //mountInfoShow::GetInstance()->setInfo(X,Y,Y2,nErrerMotor,powerRate,A);

}

void MountNet::ReadError(QAbstractSocket::SocketError)
{
}

bool MountNet::sendHeartBeat()
{
    return true;
}

bool MountNet::sendMoveMount(bool flag)
{
    if(flag)
    {

        PackStart(0, CommonConfig::GetInstance()->mountMoveLimits-m_mountPos , 0 ,0, 0 ,0);
    }
    else
    {
        PackStart(1, m_mountPos, 0, 0, 0 ,0);
    }
    return true;
}

bool MountNet::sendMoveSwingA(bool flag)
{
    if(flag)
    {
        PackStart(0,0 , 0 ,CommonConfig::GetInstance()->swingMoveLimits - m_swingPosA, 0 ,0);
    }
    else
    {
        PackStart(0, 0, 1, m_swingPosA, 0 ,0);
    }
    return true;
}
bool MountNet::sendMoveSwingB(bool flag)
{
    if(flag)
    {
        PackStart(0,0, 0 ,0, 0 ,CommonConfig::GetInstance()->swingMoveLimits - m_swingPosB);}
    else
    {
        PackStart(0, 0, 0, 0, 1 ,m_swingPosB);
    }
    return true;
}



bool MountNet::sendMoveStop()
{
    PackStop();
    return true;
}

bool MountNet::sendMovePsotion(int pos , int flag, int posbak)//flag 0 mount 1 swingA 2 swingB
{
    QLOG_INFO()<<"MountNet::sendMovePsotion"<<pos<<flag;
    int tryTimes = CommonConfig::GetInstance()->taskTimeout;
    if(flag == 0)
    {
        if(m_mountPos == pos)
        {
            return true;
        }
        unsigned char dir = (m_mountPos < pos) ? 0 : 1;
         QLOG_INFO()<<"MountNet::sendMovePsotion real pos"<<m_mountPos<<m_swingPosA<<m_swingPosB;
        PackStart(dir, abs(m_mountPos-pos), 0, 0, 0, 0);
        do
        {
            QLOG_INFO()<<"MountNet::sendMovePsotion real pos"<<m_mountPos<<m_swingPosA<<m_swingPosB;
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
        unsigned char dir = (m_swingPosA < pos) ? 0 : 1;
        PackStart(0, 0, dir, abs(m_swingPosA-pos), 0, 0);
        QLOG_INFO()<<"MountNet::sendMovePsotion real pos"<<m_mountPos<<m_swingPosA<<m_swingPosB;
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
        unsigned char dir = (m_swingPosB < pos) ? 0 : 1;
        PackStart(0, 0, 0, 0, dir, abs(m_swingPosB-pos));
        QLOG_INFO()<<"MountNet::sendMovePsotion real pos"<<m_mountPos<<m_swingPosA<<m_swingPosB;
        do
        {
            if(abs(m_swingPosB-pos) <= SWINGPOSNEAR)
            {
                return true;
            }
            Sleep(1000);
        }while(  tryTimes--);
    }
    else if(flag == 3)
    {
        if((m_swingPosA == pos) && (m_swingPosB == posbak))
        {
            return true;
        }
        unsigned char dirA = (m_swingPosA < pos) ? 0 : 1;
        unsigned char dirB = (m_swingPosB < posbak) ? 0 : 1;
        PackStart(0, 0, dirA, abs(m_swingPosA-pos), dirB, abs(m_swingPosB-posbak));
        QLOG_INFO()<<"MountNet::sendMovePsotion real pos"<<m_mountPos<<m_swingPosA<<m_swingPosB;
        do
        {
            if((abs(m_swingPosA-pos) <= SWINGPOSNEAR) &&(abs(m_swingPosB-posbak) <= SWINGPOSNEAR))
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
    qDebug()<<"MountNet::sendMoveSpeed"<<speed;
    m_mountSpeed = speed;
    //m_swingSpeed = speed;
    //下发给下位机
    return true;
}
bool MountNet::sendSwingMoveSpeed(int speed)
{
    qDebug()<<"MountNet::sendSwingMoveSpeed"<<speed;
    //m_mountSpeed = speed;
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
    QLOG_INFO()<<"MountNet::PackStart"<<dirX<<X<<dirY<<Y<<dirZ<<Z;
    QByteArray buffSend;//用来存储报头与校验位
    buffSend.resize(COM_LEN_RUN+COM_MIN+8);
    buffSend[COM_ADD_STX]=COM_STX;
    buffSend[COM_ADD_LEN]=COM_LEN_RUN;//数据长度
    buffSend[COM_ADD_MSG]=COM_MSG_XY;//数据类型
    //buffSend[COM_ADD_RAN]=0;
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

    buffSend[COM_ADD_DAT+13]=10;
    buffSend[COM_ADD_DAT+14]=0;
    buffSend[COM_ADD_DAT+15]=0;
    buffSend[COM_ADD_DAT+16] =10;//运行灯
    buffSend[COM_ADD_DAT+16] =10;//报警
    buffSend[COM_ADD_DAT+17] =10;//自动较正时清除对应的XYZA坐标
    buffSend[COM_ADD_DAT+17] =10;//控制是否启用飞达供气
    buffSend[COM_ADD_DAT+18]=(unsigned char)m_swingSpeed;//真空检测延时
    buffSend[COM_ADD_DAT+19]=(unsigned char)m_mountSpeed;//整机速度的百分比 默认开机是100
    buffSend[COM_ADD_DAT+20]=10;
    buffSend[COM_ADD_DAT+21]=10;
    buffSend[COM_ADD_DAT+22]=10;
    buffSend[COM_ADD_DAT+23]=10;
    buffSend[COM_ADD_DAT+24]=10;
    buffSend[COM_ADD_MSG_END]=COM_MSG_STOP;
    socketClient.SendData(buffSend);
}

void MountNet::PackGo0(int endmsg)
{
    QLOG_INFO()<<"MountNet::PackGo0";
    QByteArray buffSend;//用来存储报头与校验位
    buffSend.resize(COM_LEN_RUN+COM_MIN+8);
    buffSend[COM_ADD_STX]=COM_STX;
    buffSend[COM_ADD_LEN]=COM_LEN_RUN;//数据长度
    buffSend[COM_ADD_MSG]=COM_MSG_GO0;//数据类型
    buffSend[COM_ADD_MSG_END]=endmsg;
    socketClient.SendData(buffSend);
}
void MountNet::onSet(int endMsg)//上电参数设置
{
    QLOG_INFO()<<"MountNet::onSet";
    QByteArray buffSend;//用来存储报头与校验位
    buffSend.resize(COM_LEN_RUN+COM_MIN+8);
        //第一步打包数据
    buffSend[COM_ADD_DAT+1]=4;//添加吸嘴数量 低4位
    buffSend[COM_ADD_DAT+2]=m_mountSpeed;//添加吸嘴数量 低4位
    buffSend[COM_ADD_DAT+3]=m_swingSpeed;//bit1 添加是否使用飞达跳盖检查信号  =1使用
    buffSend[COM_ADD_DAT+4]=CommonConfig::GetInstance()->lowerPoer;//电池电量

    buffSend[COM_ADD_DAT+6]=CommonConfig::GetInstance()->mountMotorRate/2;//进出板延时
    buffSend[COM_ADD_DAT+7]=CommonConfig::GetInstance()->mountMotorAccTime/2  ;//
    buffSend[COM_ADD_DAT+8]=CommonConfig::GetInstance()->swingMotorRate/2;//
    buffSend[COM_ADD_DAT+9]=CommonConfig::GetInstance()->swingMotorAccTime/2;//

    int nx=CommonConfig::GetInstance()->mountResetPos;
    buffSend[COM_ADD_DAT+10]=nx>>16 ;//
    buffSend[COM_ADD_DAT+11]=nx>>8 ;//
    buffSend[COM_ADD_DAT+12]=nx ;//

    nx=CommonConfig::GetInstance()->swingResetPosL;
    buffSend[COM_ADD_DAT+13]=nx>>16 ;//
    buffSend[COM_ADD_DAT+14]=nx>>8 ;//
    buffSend[COM_ADD_DAT+15]=nx ;//

    nx=CommonConfig::GetInstance()->swingResetPosR;
    buffSend[COM_ADD_DAT+16]=nx>>16 ;//
    buffSend[COM_ADD_DAT+17]=nx>>8 ;//
    buffSend[COM_ADD_DAT+18]=nx ;//

    //第二步添加包标志
        buffSend[COM_ADD_LEN]=COM_LEN_RUN;//数据长度
        buffSend[COM_ADD_MSG]=COM_MSG_SET;//数据类型
        buffSend[COM_ADD_MSG_END]=endMsg;
       socketClient.SendData(buffSend);
}

void MountNet::PackJG(int sta)
{
    QLOG_INFO()<<"MountNet::PackJG";
    QByteArray buffSend;//用来存储报头与校验位
    buffSend.resize(COM_LEN_RUN+COM_MIN+8);
    buffSend[COM_ADD_STX]=COM_STX;
    buffSend[COM_ADD_MSG_END]=0;
    buffSend[COM_ADD_MSG]=COM_MSG_EN;
    buffSend[COM_ADD_DAT]=sta;//关闭
    socketClient.SendData(buffSend);
}
//        buffSend[COM_ADD_DAT+1]=4;//添加吸嘴数量 低4位
//        buffSend[COM_ADD_DAT+2]=mS.VAll;//添加吸嘴数量 低4位
//        buffSend[COM_ADD_DAT+3]=mS.VAll2;//bit1 添加是否使用飞达跳盖检查信号  =1使用
//int nKHzXY（代表频率15khz）,nMsXY（加速时间100s）;//XY最高频率
    //int VAll（底盘速度30%）,VAll2（摇臂速度10%）;//整机速度
//        buffSend[COM_ADD_DAT+6]=mS.nKHzXY/2 ;//进出板延时
//        buffSend[COM_ADD_DAT+7]=mS.nMsXY/2  ;//
//        buffSend[COM_ADD_DAT+8]=mS.nKHzXY2/2;//
//        buffSend[COM_ADD_DAT+9]=mS.nMsXY2/2 ;//
/*
 double fXp;//1mm脉冲数
 int nKHzXY,nMsXY;//XY最高频率
 int nKHzXY2,nMsXY2;//XY最高频率
 double pul2mm(int n,int sign=SMT_X1MM);//坐标转换
 int mm2pul(double f,int sign=SMT_X1MM);//坐标转换
 int VAll,VAll2;//整机速度
 int nDistancemm,nAngle;//小车移动的距离和升降角度
 int SX,SY,SY2;
*/

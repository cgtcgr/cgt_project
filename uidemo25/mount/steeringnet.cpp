//#include "steeringnet.h"
//#include "QsLog.h"

//#define stepPos 50;
//steeringNet *steeringNet::GetInstance(int c)
//{
//    if(c == 0)
//    {
//        static steeringNet instance0(c);
//        return &instance0;
//    }
//    else if(c == 1)
//    {
//        static steeringNet instance1(c);
//        return &instance1;
//    }

//}


//steeringNet::steeringNet(int c):
//m_currentPos(400),
//m_minAngle(400),
//m_maxAngle(3000),
//m_devState(0),
//m_readState(0),
//m_channel(c),
//m_connect(false),
//m_tcpClient(new QTcpSocket()),
//m_timer(new QTimer())
//{

//}

//bool steeringNet::setIpPort(QString ip, int p)
//{
//    m_ip = ip;
//    port = p;
//    return true;
//}
////外部保证线程安全
//bool steeringNet::init()
//{
//    socketClient.ConnectTo(m_ip,port);
//    connect(&socketClient, SIGNAL(signalOnReceiveData(QByteArray)), this, SLOT(ReadData(QByteArray)));

//    if (socketClient.IsConnected())  // 连接成功则进入if{}
//    {
//        QLOG_INFO()<<"connect success";

//        m_connect = true;
//        ReadPos(0);
//        ReadMaxAngle(0);
//        ReadMinAngle(0);
//    }
//    return true;
//}

//bool steeringNet::disconnect()
//{
//    m_connect = false;
//    return socketClient.disconnect();
//}


////slots
//void steeringNet::ReadData(QByteArray buffer)
//{
//    if(buffer.isEmpty())
//    {
//        QLOG_INFO()<<"recv data error";
//    }
//    QLOG_INFO()<<"steeringNet recv data" << buffer;
//    if(m_readState == 1)
//    {
//        if(buffer.size() == 6)
//         {
//            m_devState = buffer.at(4);
//        }
//    }
//    else if(m_readState == 1)
//    {
//        if(buffer.size() == 8)
//         {
//            m_devState = buffer.at(4);
//            m_minAngle = SCS2Host(buffer.at(5),buffer.at(6));
//        }
//    }
//    else if(m_readState == 2)
//    {
//        if(buffer.size() == 8)
//         {
//            m_devState = buffer.at(4);
//            m_maxAngle = SCS2Host(buffer.at(5),buffer.at(6));
//        }
//    }
//    else if(m_readState == 2)
//    {
//        if(buffer.size() == 8)
//         {
//            m_devState = buffer.at(4);
//            m_currentPos = SCS2Host(buffer.at(5),buffer.at(6));
//        }
//    }
//    m_cond.notify_all();
//}

//void steeringNet::ReadError(QAbstractSocket::SocketError)
//{
//    socketClient.GetSocket()->disconnect();
//}

//void steeringNet::writeBuf(u8 ID, u8 MemAddr, u8 *nDat, u8 nLen, u8 Fun)
//{
//    u8 msgLen = 2;
//    QByteArray bBuf;
//    u8 CheckSum = 0;
//    bBuf.resize(5);
//    bBuf[0] = 0xff;
//    bBuf[1] = 0xff;
//    bBuf[2] = ID;
//    bBuf[4] = Fun;
//    if(nDat){
//        msgLen += nLen + 1;
//        bBuf.resize(6);
//        bBuf[3] = msgLen;
//        bBuf[5] = MemAddr;
//        for(int i = 1 ;i<= nLen; i++)
//        {
//           bBuf.push_back(nDat[i-1]);
//        }

//    }else{
//        bBuf[3] = msgLen;
//    }
//    CheckSum = ID + msgLen + Fun + MemAddr;
//    u8 i = 0;
//    if(nDat){
//        for(i=0; i<nLen; i++){
//            CheckSum += nDat[i];
//        }
//    }
//    bBuf.push_back(~CheckSum);
//    QLOG_INFO()<<"steeringNet::writeBuf"<<m_channel<<bBuf.size()<<bBuf.toHex();


//    socketClient.SendData(bBuf);
//}

////写位置指令
////舵机ID，Position位置，执行时间Time，执行速度Speed  写数据不关心返回值


//bool steeringNet::WritePos(u8 ID, u16 Position, u16 Time, u16 Speed)
//{
//    QLOG_INFO()<<"steeringNet::WritePos";
//    u8 buf[6];
//    Host2SCS(buf+0, buf+1, Position);
//    Host2SCS(buf+2, buf+3, Time);
//    Host2SCS(buf+4, buf+5, Speed);
//    writeBuf(ID, P_GOAL_POSITION_L, buf, 6, INST_WRITE);
//    {
////        std::unique_lock<std::mutex> lock(control_mutex_);
////        if (m_cond.wait_for(lock, std::chrono::seconds(10)) == std::cv_status::timeout)
////        {
////            QLOG_INFO()<<"sendmsg timeout";
////            return false;
////        }
////        else
////        {
////            QLOG_INFO()<<"sendmsg success";
////            return true;
////        }
//    }
//    return true;
//}

////读指令
////舵机ID，MemAddr内存表地址，返回数据nData，数据长度nLen
////读2字节，超时返回-1
//bool steeringNet::readWord(u8 ID, u8 MemAddr)
//{
//    u8 nDat[2];
//    u16 wDat;
//    writeBuf(ID, MemAddr, nDat, 2, INST_READ);
//    std::unique_lock<std::mutex> lock(control_mutex_);
//    if (m_cond.wait_for(lock, std::chrono::seconds(5)) == std::cv_status::timeout)
//    {
//        QLOG_INFO()<<"sendmsg timeout";
//        return false;
//    }
//    else
//    {
//        QLOG_INFO()<<"sendmsg success";
//        return true;
//    }
//    return false;
//}

////读位置，超时返回-1
//int steeringNet::ReadPos(u8 ID)
//{
//    m_readState = 4;
//    if(readWord(ID, P_PRESENT_POSITION_L))
//    {
//        QLOG_INFO()<<"steeringNet m_currentPos:"<<m_currentPos;
//        return m_currentPos;
//    }
//    else
//    {
//        return 0;
//    }
//}

////读maxangle，超时返回-1
//int steeringNet::ReadMaxAngle(u8 ID)
//{
//    m_readState = 3;
//    if(readWord(ID, P_MAX_ANGLE_LIMIT_L))
//    {
//        QLOG_INFO()<<"steeringNet m_maxAngle:"<<m_maxAngle;
//        return m_maxAngle;
//    }
//    else
//    {
//        return 0;
//    }
//}
////读minangle，超时返回-1
//int steeringNet::ReadMinAngle(u8 ID)
//{
//    m_readState = 2;
//    if(readWord(ID, P_MIN_ANGLE_LIMIT_L))
//    {
//        QLOG_INFO()<<"steeringNet m_minAngle:"<<m_minAngle;
//        return m_minAngle;
//    }
//    else
//    {
//        return 0;
//    }
//}


////Ping指令，返回舵机ID，超时返回-1
//int steeringNet::Ping(u8 ID)
//{
//    m_readState = 1;
//    u8 bBuf[6];
//    writeBuf(ID, 0, NULL, 0, INST_PING);
//    std::unique_lock<std::mutex> lock(control_mutex_);
//    if (m_cond.wait_for(lock, std::chrono::seconds(5)) == std::cv_status::timeout)
//    {
//        QLOG_INFO()<<"sendmsg timeout";
//        return -1;
//    }
//    else
//    {
//        QLOG_INFO()<<"sendmsg success";
//        return m_devState;
//    }
//    return -1;
//}

////复位舵机参数为默认值
//bool steeringNet::Reset(u8 ID)
//{
//    writeBuf(ID, 0, NULL, 0, INST_RESET);
//    return true;
//}

////复位舵机参数为默认值
//bool steeringNet::MoveFrontStep()
//{
//    QLOG_INFO()<<"steeringNet MoveFrontStep";
//    int rightPos = m_currentPos+ stepPos;
//    if(rightPos>3000)
//    {
//        rightPos = 3000;
//    }
//    WritePos(0, rightPos, 0, 1000);
//    return true;
//}

////复位舵机参数为默认值
//bool steeringNet::MoveBackStep()
//{

//    int leftPos = m_currentPos+ stepPos;
//    if(leftPos<3000)
//    {
//        leftPos = 400;
//    }WritePos(0, leftPos, 0, 1000);
//    return true;
//}

////DataL为低位，DataH为高位
//void steeringNet::Host2SCS(u8 *DataL, u8* DataH, int Data)
//{
//    if(End){
//        *DataL = (Data>>8);
//        *DataH = (Data&0xff);
//    }else{
//        *DataH = (Data>>8);
//        *DataL = (Data&0xff);
//    }
//}

////2个8位数组合为1个16位数
////DataL为低位，DataH为高位
//int steeringNet::SCS2Host(u8 DataL, u8 DataH)
//{
//    int Data;
//    if(End){
//        Data = DataL;
//        Data<<=8;
//        Data |= DataH;
//    }else{
//        Data = DataH;
//        Data<<=8;
//        Data |= DataL;
//    }
//    return Data;
//}


#include "xsocketclient.h"
#include "QsLog.h"
XSocketClient::XSocketClient()
{
}

XSocketClient::~XSocketClient()
{
    if(nullptr != m_pClientSocket)
    {
        m_pClientSocket->deleteLater();
    }
}

void XSocketClient::SetAcceptSocket(QTcpSocket *pSocket)
{
    m_pClientSocket = pSocket;
    InitClientSocket(m_pClientSocket);
}

void XSocketClient::ConnectTo(QString ip, quint16 port)
{
    if(nullptr == m_pClientSocket)
    {
        m_pClientSocket = new QTcpSocket(this);
    }
    InitClientSocket(m_pClientSocket);
    m_pClientSocket->connectToHost(QHostAddress(ip), port);
}

bool XSocketClient::IsConnected()
{
    bool bre = false;
    if(nullptr != m_pClientSocket)
    {
        bre = (m_pClientSocket->state() == QTcpSocket::ConnectedState);
    }
    return bre;
}

bool XSocketClient::SendData(QByteArray by)
{
    bool bre = false;
    if(nullptr != m_pClientSocket)
    {
        bre = (by.size() == m_pClientSocket->write(by));
    }
   // QLOG_INFO() << "XSocketClient::SendData"<<by.toHex();
    return bre;
}

QTcpSocket *XSocketClient::GetSocket()
{
    return m_pClientSocket;
}

void XSocketClient::OnDataReady()
{
    QTcpSocket *pSocket = qobject_cast<QTcpSocket*>(sender());
    if(nullptr != pSocket)
    {
        QByteArray by = pSocket->readAll();
        //QLOG_INFO()<<"XSocketClient::OnDataReady"<<by.toHex();
        emit signalOnReceiveData(by);
        OnData(by);
    }
}

void XSocketClient::OnConnected()
{
    //QTcpSocket *pSocket = qobject_cast<QTcpSocket*>(sender());
}

void XSocketClient::OnDisconnected()
{
    //QTcpSocket *pSocket = qobject_cast<QTcpSocket*>(sender());
}

void XSocketClient::InitClientSocket(QTcpSocket *pSocket)
{
    if(nullptr != pSocket)
    {
        connect(pSocket, SIGNAL(readyRead()), this, SLOT(OnDataReady()));
        connect(pSocket, SIGNAL(connected()), this, SLOT(OnConnected()));
        connect(pSocket, SIGNAL(disconnected()), this, SLOT(OnDisconnected()));

        connect(pSocket, SIGNAL(connected()), this, SIGNAL(signalOnConnected()));
        connect(pSocket, SIGNAL(disconnected()), this, SIGNAL(signalOnDisconnected()));
    }
}

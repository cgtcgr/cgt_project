#ifndef XSOCKETCLIENT_H
#define XSOCKETCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>

class XSocketClient : public QObject
{
    Q_OBJECT
public:
    XSocketClient();
    ~XSocketClient();

    ///做为服务端接受连接时，设置接受的socket
    void SetAcceptSocket(QTcpSocket *pSocket);

    ///做为客户端主动连接时使用
    void ConnectTo(QString ip, quint16 port);


    bool IsConnected();

    bool SendData(QByteArray by);
    QTcpSocket *GetSocket();

signals:
    void signalOnReceiveData(QByteArray);
    void signalOnConnected();
    void signalOnDisconnected();

protected slots:
    virtual void OnData(QByteArray by){}
    virtual void OnConnected();
    virtual void OnDisconnected();

    void OnDataReady();

private:
    void InitClientSocket(QTcpSocket *pSocket);

private:
    QTcpSocket *m_pClientSocket = nullptr;

};

#endif // XSOCKETCLIENT_H


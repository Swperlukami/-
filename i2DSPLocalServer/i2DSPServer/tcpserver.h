#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QByteArray>
#include <QTcpServer>
#include <QHash>

class TcpServer : public QTcpServer
{
public:
    TcpServer(QObject *parent = 0);
    bool startTcpServer(QString Host , unsigned int Port);
    bool stopTcpServer();
    bool sendClientData(qintptr SocketDescriptor , QByteArray SendData);

signals:
    void clientReceiveData(qintptr SocketDescriptor , QByteArray ReveiveData);

private:
    QString msHost;
    int miPort;

    QHash<qintptr , QTcpSocket*>mhClientList;


};

#endif // TCPSERVER_H

#ifndef TCPSOCKET_H
#define TCPSOCKET_H
#include <QTcpSocket>
#include <QAbstractSocket>
#include "public.h"

class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpSocket(qintptr SocketDescriptor);
    bool writeSocketData(QByteArray Data , quint32 len);
signals:
    void socketDisconnected(qintptr SocketDescriptor);
private:
    qintptr mSocketDescriptor;
    QTimer mtLTRegisterTimer;
    bool mLTRegisterFlag;
    QTimer mtCheckLTOnlineTimer;
    quint32 mLTLive;

    void clientDisconnected();
    void receiveData();

    void processData(const QByteArray cmd);


    QString mstrLTISN;
    QString mstrLTIP;

};

#endif // TCPSOCKET_H

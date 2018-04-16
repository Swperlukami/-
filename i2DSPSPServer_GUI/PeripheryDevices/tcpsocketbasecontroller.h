#ifndef TCPSOCKETBASECONTROLLER_H
#define TCPSOCKETBASECONTROLLER_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include "SysDataBase/database.h"

class TcpSocketBaseController : public QObject
{
    Q_OBJECT
public:
    explicit TcpSocketBaseController(QObject *parent = nullptr);
    ~TcpSocketBaseController();
signals:
    void deviceConneted();
    void deviceDisconnted();
public:
    virtual void processData(QByteArray data) = 0;
    void setTcpSocket(QTcpSocket *pTcopSocket);
    void sendTcpSocketData(QByteArray data , int len);
    QTcpSocket *m_pTcpSocket ;
    int m_SocketLive;

    void enableKeepLiveTimer(bool isEnable);
    void setKeepLiveInfo(QByteArray keepLiveBuffer , int keepLiveInterval , int keepLiveTimes);
public slots:

private:
    QTimer m_KeepLiveTimer;
    QByteArray m_KeepLiveBuffer;
    int m_KeepLiveTimerInterval;
    int m_keepLiveTimes ;
private slots:
    void readData();
};

#endif // TCPSOCKETBASECONTROLLER_H

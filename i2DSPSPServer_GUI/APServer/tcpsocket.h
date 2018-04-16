#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include "ap_protocol.h"
#include "SysProrocol/server_ui_protocol.h"
#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QByteArray>
#include <QHostAddress>
//#define FLINE __FILE__<<"Line:"<<__LINE__


class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpSocket(qintptr SocketDescriptor);

    void setAPInput(int InputNo);
    void setAPEmgMode(bool isEmgMode);
    void setAPVoice(unsigned char voice);
    void setAPSelfTest();


    void setKeepLiveEnable(bool keepLiveEnable);
    QString m_strClientIP;
    int m_inputNo ;
    int m_CurrentInputNo;
    qintptr mSocketDescriptor;


    void sendUIAPState(const APState spstate);

signals:
    void socketDisconnected(qintptr SocketDescriptor);

    void apEvent(QByteArray event);                                              // 6 online 7 offline


private:
    QTimer mtCheckLTOnlineTimer;
    QTimer mtSendAPKeepLiveTimer;
    quint32 m_intClinetLive;

    void clientDisconnected();
    void receiveData();

    void processData(const QByteArray cmd);

private slots:
    void sendAPKeepLiveEvent();
    bool writeSocketData(QByteArray Data , quint32 len);


};

#endif // TCPSOCKET_H

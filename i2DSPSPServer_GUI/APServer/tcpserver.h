#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTimer>
#include <QTcpSocket>
#define FLINE __FILE__<<"Line:"<<__LINE__



class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpSocket(qintptr SocketDescriptor);
    bool writeSocketData(QByteArray Data , quint32 len);

    void setAPInput(int InputNo, bool isBroadcast);
    void setKeepLiveEnable(bool keepLiveEnable);
    QString mstrLTIP;
    int m_inputNo ;
    int m_CurrentInputNo;
    qintptr mSocketDescriptor;

signals:
    void socketDisconnected(qintptr SocketDescriptor);

    void apEvent(QString APIp ,  unsigned char APEvent);             // 6 online 7 offline
    void resAPSPState(QString APIp , QString SPId ,int SPNo, int SPState);

private:
    QTimer mtCheckLTOnlineTimer;
    QTimer mtSendAPKeepLiveTimer;
    quint32 mLTLive;

    void clientDisconnected();
    void receiveData();

    void processData(const QByteArray cmd);

};

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0);
    bool startTcpServer(QString Host , quint16 Port);
    bool stopTcpServer();


    void incomingConnection(qintptr socketDescriptor);
public slots:
    void disConneted(qintptr socketDescriptor);

    void setAllApBroadcastMode(bool isBroadcast);
    void synAPInput(QString APIp , int InputNo, bool isEmg = false);

    void apSeltTest();
signals:
    void serverAPEvent(QString APIp , unsigned char event);


private:
    TcpSocket *mpTcpSocket;
    QHash<qintptr , TcpSocket*>mClientSocketDescriptorList;

};

#endif // TCPSERVER_H

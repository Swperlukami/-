#include "tcpsocketbasecontroller.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

TcpSocketBaseController::TcpSocketBaseController(QObject *parent) : QObject(parent)
{
    m_pTcpSocket = NULL;
    m_KeepLiveTimerInterval= 0;
    m_keepLiveTimes = 0;
    m_SocketLive = 0;

    connect(&m_KeepLiveTimer , &QTimer::timeout , this , [=](){
        if(m_SocketLive <= 0 && m_pTcpSocket != NULL){
            qDebug()<<FLINE<<m_pTcpSocket->peerAddress()<<" Time Out Close Socket" ;
            m_pTcpSocket->close();
            m_pTcpSocket->deleteLater();
            m_pTcpSocket = NULL ;
        }
        else
            sendTcpSocketData(m_KeepLiveBuffer , m_KeepLiveBuffer.length());

        m_SocketLive -= 1 ;
    });

    m_KeepLiveTimer.stop();
}
void TcpSocketBaseController::setTcpSocket(QTcpSocket *pTcopSocket)
{
    if(m_pTcpSocket){
        m_pTcpSocket->close();
        m_pTcpSocket->deleteLater();
        m_pTcpSocket = NULL ;
    }

    m_pTcpSocket = pTcopSocket;
    m_pTcpSocket->setSocketOption(QAbstractSocket::LowDelayOption , 1);
    m_pTcpSocket->setSocketOption(QAbstractSocket::KeepAliveOption , 1);

    int enableKeeppAlive = 1;
    int fd = m_pTcpSocket->socketDescriptor();
    setsockopt(fd , SOL_SOCKET , SO_KEEPALIVE , &enableKeeppAlive , sizeof(enableKeeppAlive));

    int maxIdel = 6 ;
    setsockopt(fd , IPPROTO_TCP , TCP_KEEPIDLE , &maxIdel , sizeof(maxIdel));

    int count = 3;
    setsockopt(fd , SOL_TCP , TCP_KEEPCNT , &count , sizeof(count));

    int interval = 2;
    setsockopt(fd , SOL_TCP , TCP_KEEPINTVL , &interval , sizeof(interval));

    connect(m_pTcpSocket , &QTcpSocket::readyRead , this , &TcpSocketBaseController::readData);
    connect(m_pTcpSocket , &QTcpSocket::disconnected , this , &TcpSocketBaseController::deviceDisconnted);
    emit deviceConneted();
    m_SocketLive = m_keepLiveTimes ;

}
TcpSocketBaseController::~TcpSocketBaseController()
{
    if(m_pTcpSocket){
        m_pTcpSocket->close();
        m_pTcpSocket->deleteLater();
    }
    m_pTcpSocket = NULL ;

}
void TcpSocketBaseController::setKeepLiveInfo(QByteArray keepLiveBuffer , int keepLiveInterval , int keepLiveTimes)
{
    m_KeepLiveBuffer = keepLiveBuffer;
    m_KeepLiveTimerInterval= keepLiveInterval ;
    m_keepLiveTimes = keepLiveTimes ;
}
void TcpSocketBaseController::enableKeepLiveTimer(bool isEnable)
{
    if(isEnable){
        m_KeepLiveTimer.start(m_KeepLiveTimerInterval*1000);
    }
    else{
        m_KeepLiveTimer.stop();
    }
}
void TcpSocketBaseController::readData()
{
    while(m_pTcpSocket->bytesAvailable()){
        m_SocketLive = m_keepLiveTimes ;
        processData(m_pTcpSocket->readAll());
    }
}

void TcpSocketBaseController::sendTcpSocketData(QByteArray data, int len)
{
    if(m_pTcpSocket != NULL && m_pTcpSocket->state() == QTcpSocket::ConnectedState){
        if(m_pTcpSocket->write(data.data() , len) != len){
            qDebug()<<FLINE<<"Send Tcp Socket Data Len Error";
        }
        else{
            qDebug()<<FLINE<<"Send Tcp Socket Data Len OK"<<m_pTcpSocket->peerAddress().toString();
        }
    }
    else{
        qDebug()<<FLINE<<"Tcp Socket Disconnted";
    }
}

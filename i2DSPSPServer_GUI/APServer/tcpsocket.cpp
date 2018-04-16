#include "tcpsocket.h"
#include "SysProrocol/server_ui_protocol.h"
#include <QThread>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

TcpSocket::TcpSocket(qintptr SocketDescriptor)
    :mSocketDescriptor(SocketDescriptor)
{
    if(this->setSocketDescriptor(SocketDescriptor)){
        qDebug()<<FLINE<<this->peerAddress()<<"socket:"<<SocketDescriptor<<"Connected";
    }
    else{
        qDebug()<<FLINE<<"Create "<<this->peerAddress()<<"Failed";
        return;
    }
    m_inputNo = 0 ;
    m_CurrentInputNo = 6 ;
    m_strClientIP = "";

    connect(this , &QTcpSocket::readyRead , this , &TcpSocket::receiveData);
    connect(this , &QTcpSocket::disconnected , this , &TcpSocket::clientDisconnected);
    connect(this, static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),
            [=](QAbstractSocket::SocketError socketError){ qDebug()<<socketError;});

    m_strClientIP = this->peerAddress().toString();

    //检查AP存活生命值
    m_intClinetLive = 2;
    mtCheckLTOnlineTimer.setInterval(16*1000);
    connect(&mtCheckLTOnlineTimer , &QTimer::timeout , [this](){
        if(m_intClinetLive <= 0){
            clientDisconnected();
                qDebug()<<FLINE<<"AP >> ip"<<m_strClientIP<<" 15s Keep Live Packget Time Out Close Socket";
                return;
        }
        m_intClinetLive--;
    });
    setKeepLiveEnable(false);

    //向功放发送保活事件
//    sendAPKeepLiveEvent();
//    mtSendAPKeepLiveTimer.setInterval(5*1000);


    this->setSocketOption(QAbstractSocket::LowDelayOption , 1);
    this->setSocketOption(QAbstractSocket::KeepAliveOption , 1);

    int enableKeeppAlive = 1;
    int fd = this->socketDescriptor();
    setsockopt(fd , SOL_SOCKET , SO_KEEPALIVE , &enableKeeppAlive , sizeof(enableKeeppAlive));

    int maxIdel = 6 ;
    setsockopt(fd , IPPROTO_TCP , TCP_KEEPIDLE , &maxIdel , sizeof(maxIdel));

    int count = 3;
    setsockopt(fd , SOL_TCP , TCP_KEEPCNT , &count , sizeof(count));

    int interval = 2;
    setsockopt(fd , SOL_TCP , TCP_KEEPINTVL , &interval , sizeof(interval));
}
void TcpSocket::setKeepLiveEnable(bool keepLiveEnable)
{
    if(keepLiveEnable){
        if(!mtCheckLTOnlineTimer.isActive()){
            mtCheckLTOnlineTimer.start();
        }
    }
    else{
        if(!mtCheckLTOnlineTimer.isActive()){
            mtCheckLTOnlineTimer.stop();
        }
    }
}

bool TcpSocket::writeSocketData(QByteArray Data, quint32 len)
{
    if(this->state() == QTcpSocket::ConnectedState){
        if(len !=  this->write(Data.data() , len)){
            qDebug()<<FLINE<<"Write Socket Data Error";
            return false;
        }
        this->waitForBytesWritten();
        return true;
    }
    else
        return false ;
}
void TcpSocket::receiveData()
{
    while (this->bytesAvailable()) {
        processData(this->readAll());
    }
}
void TcpSocket::clientDisconnected()
{
    m_intClinetLive = 0;
    qDebug()<<FLINE<<"Socket Client Disconnected:"<<this->peerAddress().toString();
    sendUIAPState(APOffline);
    emit socketDisconnected(mSocketDescriptor);
}


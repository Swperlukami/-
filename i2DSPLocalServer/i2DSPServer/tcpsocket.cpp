#include "tcpsocket.h"
#include "senduidata.h"

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

    connect(this , &QTcpSocket::readyRead , this , &TcpSocket::receiveData);
    connect(this , &QTcpSocket::disconnected , this , &TcpSocket::clientDisconnected);
    connect(this, static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),
            [=](QAbstractSocket::SocketError socketError){ qDebug()<<socketError;});

    mstrLTIP = this->peerAddress().toString();

    mLTRegisterFlag = false;
    connect(&mtLTRegisterTimer , &QTimer::timeout ,[this](){
        if(!mLTRegisterFlag && this->state() == QTcpSocket::ConnectedState){
            qDebug()<<FLINE<<"LT >> ip:"<<mstrLTIP<<" Wait LT 4S Register Time Out Close Socket";
            this->close();
        }
    });
    mtLTRegisterTimer.setSingleShot(true);
    mtLTRegisterTimer.start(4000);

    mLTLive = 2;
    connect(&mtCheckLTOnlineTimer , &QTimer::timeout , [this](){
        if(mLTRegisterFlag && mLTLive <= 0){
            this->close();
            qDebug()<<"LT >> ip"<<mstrLTIP<<" isn:"<<mstrLTISN<<" 15s Keep Live Packget Time Out Close Socket";
            return;
        }
        mLTLive--;
    });
    mtCheckLTOnlineTimer.start(16000);

}

bool TcpSocket::writeSocketData(QByteArray Data, quint32 len)
{
    if(this->state() == QTcpSocket::ConnectedState){
        if(len !=  this->write(Data)){
            qDebug()<<FLINE<<"Write Socket Data Error";
            return false;
        }
        this->waitForBytesWritten();
    }
    return true;
}
void TcpSocket::receiveData()
{
    while (this->bytesAvailable()) {
        processData(this->readAll());
    }
}
void TcpSocket::clientDisconnected()
{
    if(mLTRegisterFlag){
        mLTRegisterFlag = false;
        mLTLive = 0;
        qDebug()<<FLINE<<"LT Disconnected ip:"<<mstrLTIP<<" isn:"<<mstrLTISN;

        QString  strQuery = QString("update lt set socket=0 , online=0 where ltid='%1' ").arg(mstrLTISN);
        if(!exec_database(strQuery)){
            qDebug()<<FLINE<<mstrLTIP<<"Offline database exec Error";
            return;
        }
        strQuery = QString("update sensor set online=0 , emglv=0 where ltid='%1' ").arg(mstrLTISN);
        if(!exec_database(strQuery)){
            qDebug()<<FLINE<<mstrLTIP<<"Offline database exec Error";
            return;
        }

        lt_on_off_line_(mstrLTISN , false);
    }
    qDebug()<<FLINE<<"Client Disconnected:"<<this->peerAddress().toString();
    emit socketDisconnected(mSocketDescriptor);
}

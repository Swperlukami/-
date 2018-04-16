#include "aptcpserver.h"
#include <QThread>

TcpServer::TcpServer(QObject *parent) : QTcpServer(parent)
{
    mpTcpSocket = NULL;
}
bool TcpServer::startTcpServer(QString Host, quint16 Port)
{
    if(this->isListening())
        this->close();

    if(this->listen(QHostAddress(Host) , Port)){
        qDebug()<<FLINE<<"Server Listen Host:"<<Host<<" Port:"<<Port<<" Successed";
        return true;
    }
    qDebug()<<FLINE<<"Server Listen Host:"<<Host<<" Port:"<<Port<<" Failed";
    return false;

}
bool TcpServer::stopTcpServer()
{
    if(this->isListening()){
        this->close();
        foreach (QTcpSocket* Socket, mClientSocketDescriptorList.values()) {
            Socket->close();
            Socket->deleteLater();
        }
        mClientSocketDescriptorList.clear();

        qDebug()<<FLINE<<"Server Closed";
        return true;
    }
    qDebug()<<FLINE<<"Server Closed Failed";
    return false;
}
void TcpServer::incomingConnection(qintptr socketDescriptor)
{
    mpTcpSocket = new TcpSocket(socketDescriptor);
    if(mpTcpSocket == NULL){
        qDebug()<<FLINE<<"Create New TcpSocket Error";
        return;
    }

    qDebug()<<FLINE<<"New AP"<<socketDescriptor;
    connect(mpTcpSocket , &TcpSocket::socketDisconnected , this , &TcpServer::disConneted);
    connect(mpTcpSocket , &TcpSocket::apEvent , this , &TcpServer::serverAPEvent , Qt::QueuedConnection);

    foreach (TcpSocket *pSocket, mClientSocketDescriptorList.values()) {
        if(pSocket->m_strClientIP == mpTcpSocket->m_strClientIP){
            if(mClientSocketDescriptorList.remove(pSocket->socketDescriptor())){
                qDebug()<<FLINE<<"Remove Already Exist AP Socket"<<pSocket->m_strClientIP<<pSocket->socketDescriptor();
                pSocket->abort();
                pSocket->deleteLater();
            }
        }
    }

    mClientSocketDescriptorList.insert(mpTcpSocket->socketDescriptor()  , mpTcpSocket);

    mpTcpSocket->sendUIAPState(APOnline);           //AP Online
    mpTcpSocket = NULL;

}
void TcpServer::disConneted(qintptr socketDescriptor)
{
    TcpSocket *pDisconneted = NULL;
    pDisconneted = mClientSocketDescriptorList.value(socketDescriptor);
    if(pDisconneted != NULL) {
           if(mClientSocketDescriptorList.remove(pDisconneted->mSocketDescriptor) >0 ){
               qDebug()<<FLINE<<"Client Ip:"<<pDisconneted->m_strClientIP<<" socket"<<socketDescriptor<<" Removed";
//               pDisconneted->close();
               pDisconneted->abort();
               pDisconneted->deleteLater();

               qDebug()<<FLINE<<"Delete Socket "<<socketDescriptor;
           }
    }
}


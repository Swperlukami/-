#include "i2dspserver.h"



i2DSPServer::i2DSPServer(QObject *parent) : QTcpServer(parent)
{
    mpTcpSocket = NULL;


}
bool i2DSPServer::startTcpServer(QString Host, quint16 Port)
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
bool i2DSPServer::stopTcpServer()
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
void i2DSPServer::incomingConnection(qintptr socketDescriptor)
{
    mpTcpSocket = new TcpSocket(socketDescriptor);
    if(mpTcpSocket == NULL){
        qDebug()<<FLINE<<"Create New TcpSocket Error";
        return;
    }

    connect(mpTcpSocket , &TcpSocket::socketDisconnected , this , &i2DSPServer::disConneted);

    mClientSocketDescriptorList.insert(mpTcpSocket->socketDescriptor()  , mpTcpSocket);

    mpTcpSocket = NULL;
}
void i2DSPServer::disConneted(qintptr socketDescriptor)
{
    TcpSocket *pDisconneted = NULL;
    pDisconneted = mClientSocketDescriptorList.value(socketDescriptor);
    if(pDisconneted != NULL) {
            mClientSocketDescriptorList.remove(pDisconneted->socketDescriptor());
            pDisconneted->close();
            pDisconneted->deleteLater();
    }
}


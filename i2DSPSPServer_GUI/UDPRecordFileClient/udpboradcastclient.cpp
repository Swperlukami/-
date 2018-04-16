#include "udpboradcastclient.h"
#include <QTimer>

UDPBoradcastClient::UDPBoradcastClient(QObject *parent) : QObject(parent)
{
    m_BroadcastUdpSocket.setParent(this);
    connect(&m_BroadcastUdpSocket , &QUdpSocket::readyRead , this , &UDPBoradcastClient::recordBroadcastFile);
}
bool UDPBoradcastClient::bindBroadcastPort(int port)
{
    if(m_BroadcastUdpSocket.bind(QHostAddress::AnyIPv4 ,  port , QAbstractSocket::ShareAddress)){
//        qDebug()<<m_BroadcastUdpSocket.state();
//        qDebug()<<QNetworkInterface::allInterfaces();
//        qDebug()<<"Net:"<<QNetworkInterface::allInterfaces().at(1).flags();
//        m_BroadcastUdpSocket.setSocketOption(QAbstractSocket::MulticastLoopbackOption , 1);
        if(m_BroadcastUdpSocket.joinMulticastGroup(QHostAddress("224.1.1.1") , QNetworkInterface::allInterfaces().at(1)))
            return true;
        else{
            qDebug()<<m_BroadcastUdpSocket.error();
            return false;
        }
    }
    else
        return false;
}
void UDPBoradcastClient::startAndStopBroadcast(bool isStart)
{
//    if(m_BroadcastUdpSocket.joinMulticastGroup(QHostAddress("224.1.1.1")))
//        qDebug()<<"UDP Join OK";
//    else{
//        qDebug()<<m_BroadcastUdpSocket.errorString();
//    }

    static QTime recordTime;

    if(isStart){
        if(m_RecordFile.isOpen()){
            m_RecordFile.close();
        }
        if(m_BroadcastUdpSocket.joinMulticastGroup(QHostAddress("224.1.1.1") , QNetworkInterface::allInterfaces().at(1)))
            qDebug()<<"Join OK";
        m_RecordFile.setFileName("./Audio/sysBroadcastRecordFile/"+QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss")+".pcm");
        if(!m_RecordFile.open(QFile::Truncate | QFile::WriteOnly)){
            qDebug()<<"Broadcast record file open error";
        }
        recordTime.start();
    }
    else{
        m_RecordFile.close();
        if(m_BroadcastUdpSocket.leaveMulticastGroup(QHostAddress("224.1.1.1") , QNetworkInterface::allInterfaces().at(1)))
            qDebug()<<"Leave OK";

        if(recordTime.elapsed() < 2000)
            m_RecordFile.remove();
    }
}
void UDPBoradcastClient::recordBroadcastFile()
{
    while (m_BroadcastUdpSocket.hasPendingDatagrams()) {
        QNetworkDatagram udpData = m_BroadcastUdpSocket.receiveDatagram(1026);
//        qDebug()<<"Aduio"<<(unsigned char)udpData.data().at(1)<<" Len:"<<udpData.data().length();
//        qDebug()<<"Data :"<<udpData.data();
        if((unsigned char)udpData.data().at(1) == 5){
//            qDebug()<<"UDP Aduio Data"<<(unsigned char)udpData.data().at(1);
//            qDebug()<<"Data :"<<udpData.data();

        }
        if(m_RecordFile.isOpen()){
            if((unsigned char)udpData.data().at(1) == 7){
//                udpData.data().replace(0,1, &input);
//                qDebug()<<"Recording Audio Data :"<<udpData.data().at(1)<<"len:"<<udpData.data().length();

                if(m_RecordFile.write(udpData.data().mid(2)) != (udpData.data().length()-2))
                    qDebug()<<"Record UDP Data Error";
//                else
//                    qDebug()<<"Recording UDP Data ";

            }
        }
    }
}

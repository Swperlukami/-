#include "powermodeserver.h"
#include "PeripheryDevices/periphery_protocol.h"


PowerModeServer::PowerModeServer(QObject *parent) : QObject(parent)
{
    connect(&m_PowerModeServer , &QTcpServer::newConnection , this , &PowerModeServer::powerModelClientConnect);

    m_pMatrixContrller = new MatrixController(this);
    m_pPowerModelConroller = new PowerModelController(this);
    m_pNetMicController = new NetMicController(this);
    m_pListenSPKController = new LisenlingSPKController(this);
}
bool PowerModeServer::startServer(QString hostIp, int port)
{
    if(hostIp.isEmpty()){
        qDebug()<<FLINE<<"Power Model Server IP Error";
        return false;
    }
    if(m_PowerModeServer.listen(QHostAddress(hostIp) , port)){
        qDebug()<<FLINE<<"Power Mode Server"<<hostIp<<" Port:"<<port<<" Listen OK";
        return true;
    }
    else{
        qDebug()<<FLINE<<"Power Mode Server Listen Error";
        return false;
    }
}
bool PowerModeServer::stopServer()
{
    if(m_PowerModeServer.isListening())
        m_PowerModeServer.close();
}

void PowerModeServer::powerModelClientConnect()
{
    QTcpSocket *tmp = NULL;
    tmp = m_PowerModeServer.nextPendingConnection();
    if(tmp->peerAddress().toString() == "192.168.2.60"){
        qDebug()<<FLINE<<tmp->peerAddress()<<" Matrix Client Connected";
        m_pMatrixContrller->setTcpSocket(tmp);
    }


    if(tmp->peerAddress().toString() == "192.168.2.52"){
        qDebug()<<FLINE<<tmp->peerAddress()<<" Net Mic Client Connected";
        m_pNetMicController->setTcpSocket(tmp);
    }
    else if(tmp->peerAddress().toString() == "192.168.2.57"){
        qDebug()<<FLINE<<tmp->peerAddress()<<"Power Mode Client Connected";
        m_pPowerModelConroller->setTcpSocket(tmp);
    }

}



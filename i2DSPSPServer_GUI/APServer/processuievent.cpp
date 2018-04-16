#include "aptcpserver.h"

void TcpServer::processUISetAPEvent(QByteArray UIEvent)
{

}

void TcpServer::setAPEmgMode(QString APIp ,  bool isEmgMode)
{
    QList<TcpSocket*>APClients = mClientSocketDescriptorList.values();
    foreach (TcpSocket *pClient, APClients) {
        if(pClient->m_strClientIP == APIp){
            pClient->setAPEmgMode(isEmgMode);
            return;
        }
    }
}

void TcpServer::setAPInputNo(QString APIp, int InputNo)
{
    QList<TcpSocket*>APClients = mClientSocketDescriptorList.values();

    foreach (TcpSocket *pClient, APClients) {
        if(pClient->m_strClientIP == APIp){
            pClient->setAPInput(InputNo);
            return;
        }
    }
}
void TcpServer::apSeltTest()
{
    QList<TcpSocket*>APClients = mClientSocketDescriptorList.values();
    foreach (TcpSocket *pClient, APClients) {
        pClient->setAPSelfTest();
    }
}

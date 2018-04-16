#include "netmiccontroller.h"

NetMicController::NetMicController(QObject *parent) : TcpSocketBaseController(parent)
{
    connect(this , &NetMicController::deviceConneted , this , &NetMicController::netMicConnected);
    connect(this , &NetMicController::deviceDisconnted , this , &NetMicController::netMicDisConnected);

}
void NetMicController::netMicConnected()
{
    qDebug()<<FLINE<<"Net Mic Online";


    setKeepLiveInfo("" , 15 , 3);
    enableKeepLiveTimer(true);

    emit netMicEvents("Online" , m_pTcpSocket->peerAddress().toString());
}
void NetMicController::netMicDisConnected()
{
    qDebug()<<FLINE<<"Net Mic Offline";
    emit netMicEvents("Offline");
}
void NetMicController::userSetNetMicEvent(QString cmdType , int voice)
{
    QByteArray buffer(20 , 0);
    PACK_POWER_MODEL *pNetMicPack = (PACK_POWER_MODEL*)buffer.data();


    if(cmdType == "Broadcast"){
        sendNetMicData(true , false , false ,false);
    }
    else if(cmdType == "StopBroadcast"){
        sendNetMicData(false , false , false ,false);
    }
    else if(cmdType == "Volume"){
    }
    else if(cmdType == "SelfTest"){
        pNetMicPack->cmd = CMD_DEV_SELF_TEST ;
        sendTcpSocketData(buffer , sizeof(PACK_POWER_MODEL));

    }
}
void NetMicController::sendNetMicData(bool isBro, bool isEmg, bool isDeviceError, bool isVoice)
{
    qDebug()<<FLINE<<"Set NetMic System State  Bro:"<<isBro;

    QByteArray buffer(20 , 0);
    PACK_POWER_MODEL *pLightState = (PACK_POWER_MODEL*)buffer.data();

    pLightState->cmd = CMD_MC_STATE ;

    unsigned char tmp = 0x00 ;

    if(isDeviceError)
        tmp = tmp | 0x01 ;

    if(isEmg)
        tmp = tmp | 0x02 ;

    if(isBro)
        tmp = tmp | 0x04 ;

    if(isVoice)
        tmp = tmp | 0x08 ;

    qDebug()<<FLINE<<"Set Net Mic Code:"<<tmp;
    pLightState->stateCode = tmp ;
    sendTcpSocketData(buffer , sizeof(PACK_POWER_MODEL));
}

void NetMicController::processData(QByteArray data)
{

    if(data.length() < sizeof(PACK_POWER_MODEL)){
        qDebug()<<FLINE<<"Net Mic Data Len Not Enough";
        return;
    }

    PACK_POWER_MODEL *pPowerState = (PACK_POWER_MODEL*)data.data();
    switch (pPowerState->cmd) {
    case CMD_MIC_KEEP_LIVE:
    {
        qDebug()<<FLINE<<"Net Mic Data Kepp Live"<<m_SocketLive;
        QByteArray FeetBackPacket(sizeof(PACK_POWER_MODEL) ,0);
        PACK_POWER_MODEL *pFeetBack = (PACK_POWER_MODEL*)FeetBackPacket.data();
        pFeetBack->cmd = CMD_MIC_KEEP_LIVE;

        sendTcpSocketData(FeetBackPacket , sizeof(PACK_POWER_MODEL));

    }
        break;
    case CMD_BTN_BROADCAST:
    {
        qDebug()<<FLINE<<"Button Broadcast";
        emit netMicEvents("Broadcast");

    }
        break;
    case CMD_BTN_STOP_BROADCAST:
    {
        qDebug()<<FLINE<<"Button Stop Broadcast";
        emit netMicEvents("StopBroadcast");

    }
        break;
    }
}

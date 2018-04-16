#include "powermodelcontroller.h"

PowerModelController::PowerModelController(QObject *parent) : TcpSocketBaseController(parent)
{
    connect(this , &PowerModelController::deviceConneted , this , &PowerModelController::powerConnected);
    connect(this , &PowerModelController::deviceDisconnted , this , &PowerModelController::powerDisconnected);

    isSysBro = false ;
    isSysEmg = false ;
    isSysDevError = false ;
    isSysVoice = false ;
}
void PowerModelController::powerConnected()
{
    qDebug()<<FLINE<<"Power Model Online";

//    QByteArray buffer(sizeof(PACK_POWER_MODEL), 0);
//    PACK_POWER_MODEL *pPacket = (PACK_POWER_MODEL*)buffer.data();
//    pPacket->cmd = CMD_MIC_KEEP_LIVE;

//    setKeepLiveInfo(buffer , 5 , 3);
//    enableKeepLiveTimer(false);

    emit powerModelEvents("Online" , m_pTcpSocket->peerAddress().toString());
}
void PowerModelController::powerDisconnected()
{
    qDebug()<<FLINE<<"Power Model Offline";
    emit powerModelEvents("Offline");
}

void PowerModelController::userSetPowerModelEvent(bool isBro, bool isEmg)
{
    isSysBro = isBro ;
    isSysEmg = isEmg ;
    sendPowerModelData(isSysBro , isSysEmg , isSysDevError , false);

}
void PowerModelController::sysSetPowerModelEVent(bool devError, bool isVoice)
{
    isSysDevError = devError ;
    isSysVoice =  isVoice ;

    isSysVoice = false ;
    sendPowerModelData(isSysBro , isSysEmg , isSysDevError , isSysVoice);

}
void PowerModelController::sysSetPowerModelSelfTest()
{
    QByteArray buffer(20 , 0);
    PACK_POWER_MODEL *pSlefTest = (PACK_POWER_MODEL*)buffer.data();
    pSlefTest->cmd = CMD_DEV_SELF_TEST;
    sendTcpSocketData(buffer , sizeof(PACK_POWER_MODEL));

}
void PowerModelController::sendPowerModelData(bool isBro, bool isEmg, bool isDeviceError, bool isVoice)
{
    qDebug()<<FLINE<<"Set PowerModel System State IsDeiceError:"<<isDeviceError<<" Bro:"<<isBro<<" Emg:"<<isEmg<<" Voice"<<isVoice;

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

    qDebug()<<FLINE<<"Set Power Mode Code:"<<tmp;
    pLightState->stateCode = tmp ;
    sendTcpSocketData(buffer , sizeof(PACK_POWER_MODEL));
}
void PowerModelController::processData(QByteArray data)
{
    if(data.length() < sizeof(PACK_POWER_MODEL)){
        qDebug()<<FLINE<<"Power Model Data Len Not Enough";
        return;
    }

    PACK_POWER_MODEL *pPowerState = (PACK_POWER_MODEL*)data.data();

    switch (pPowerState->cmd) {
    case CMD_POWER_STATE:
    {
        QString strErrorType = "";
        static unsigned char homePowerErrorFlag = 0 ;
        static unsigned char standbyPowerErrorFlag = 0 ;

        unsigned char homepower = 0xF0  ,  standbypower = 0x0F;

        qDebug()<<FLINE<<"Power Code:"<<pPowerState->stateCode;
        switch (pPowerState->stateCode&homepower) {
        case 0x10:
            strErrorType = "正常";
            break ;
        case 0x20:
            strErrorType = "故障";
            break;
        case 0x40:
            strErrorType = "欠压";
            break;
        }
        qDebug()<<FLINE<<"Home Power State: "<<strErrorType<<(pPowerState->stateCode&homepower)<<homePowerErrorFlag;
        if((pPowerState->stateCode&homepower) != homePowerErrorFlag)
            emit powerModelEvents("HomePower" , strErrorType);
        homePowerErrorFlag = pPowerState->stateCode&homepower;

        switch (pPowerState->stateCode&standbypower) {
        case 0x01:
            strErrorType = "正常";
            break;
        case 0x02:
            strErrorType = "故障";
            break;
        case 0x04:
            strErrorType = "欠压";
            pPowerState->stateCode = 0x02;

            break;
        }
        qDebug()<<FLINE<<"Standby Power State: "<<strErrorType;
        if((pPowerState->stateCode&standbypower) != standbyPowerErrorFlag)
            emit powerModelEvents("StandbyPower" , strErrorType);
        standbyPowerErrorFlag = pPowerState->stateCode&standbypower;
    }
        break;
    case CMD_DEV_SELF_TEST:
    {
        qDebug()<<FLINE<<"Button SelfTest";
        emit powerModelEvents("SelfTest");
    }
        break;
    case CMD_BTN_EMG:
    {
        qDebug()<<FLINE<<"Button Emg";
        isSysEmg = true ;
        emit powerModelEvents("Emg");

    }
        break;
    case CMD_BTN_CANCEL_EMG:
    {
        qDebug()<<FLINE<<"Button Cancel Emg";
        isSysEmg = false ;
        emit powerModelEvents("CancelEmg");
    }
        break;
    }
}

#include "lisenlingspkcontroller.h"
#include "APServer/aptcpserver.h"


extern TcpServer *g_pAPServer;
extern QString g_ListenSPKIP;


LisenlingSPKController::LisenlingSPKController(QObject *parent) : TcpSocketBaseController(parent)
{
    connect(this , &LisenlingSPKController::deviceConneted , this , &LisenlingSPKController::listenSPKConnected);
    connect(this , &LisenlingSPKController::deviceDisconnted , this , &LisenlingSPKController::listenSPKDisconnected);

    m_pListenSPKTestSound = new QSoundEffect(this);
    m_pListenSPKTestSound->setSource(QUrl::fromLocalFile("./Audio/sysAudioFiles/VoiceTest.wav"));
    m_pListenSPKTestSound->setLoopCount(1);
    m_pListenSPKTestSound->setVolume(0.4);
//    m_pListenSPKTestSound->play();
}
void LisenlingSPKController::listenSPKConnected()
{
    qDebug()<<FLINE<<"Listen SPK Online";
    emit listenSPKEvents("Online"  , g_ListenSPKIP);
}
void LisenlingSPKController::listenSPKDisconnected()
{
    qDebug()<<FLINE<<"Listen SPK Offline";
    emit listenSPKEvents("Offline");
}
void LisenlingSPKController::userSetListenSPKEvent(QString cmdType, int vol)
{
    if(cmdType == "Enable"){
        g_pAPServer->setAPInputNo(g_ListenSPKIP , 6);
    }
    else if(cmdType == "Disable"){
        g_pAPServer->setAPInputNo(g_ListenSPKIP , 0);
    }
    else if(cmdType == "Voice"){

    }
    else if(cmdType == "SelfTest"){
        g_pAPServer->setAPInputNo(g_ListenSPKIP , 6);
        m_pListenSPKTestSound->play();
    }
}
void LisenlingSPKController::processData(QByteArray data)
{
    qDebug()<<FLINE<<data;
}

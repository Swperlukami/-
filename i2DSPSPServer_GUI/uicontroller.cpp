#include "uicontroller.h"
#include "SysProrocol/mc_uiprotocol.h"
#include "APServer/aptcpserver.h"
#include "USC4000/rs485.h"
#include "PeripheryDevices/powermodeserver.h"

extern TcpServer *g_pAPServer;
//extern MartixServer *g_pMartixServer;
extern PowerModeServer *g_pPowerSerialPort;
extern RS485 *g_pUSC4000SerialPort;


UIController::UIController(QObject *parent) : QObject(parent)
{

    m_EventControllers.pChannelMusicController = new ChannelMusicEvent;
    m_EventControllers.pAudioFileManageController = new AudioFilesManageController;
    m_EventControllers.pSysBroadcastController = new SysBroadcastController;

//    mp_SDController = new SDController;
    mp_SPController = new SPController;
    mp_APController = new APController;

    engine.rootContext()->setContextProperty("ChannelMusicController" , m_EventControllers.pChannelMusicController);
    engine.rootContext()->setContextProperty("AduioFileManageController" , m_EventControllers.pAudioFileManageController);
    engine.rootContext()->setContextProperty("SysBroadcastController" , m_EventControllers.pSysBroadcastController);


//    engine.rootContext()->setContextProperty("SDController" , mp_SDController);
    engine.rootContext()->setContextProperty("SPController" , mp_SPController);
    engine.rootContext()->setContextProperty("APController" , mp_APController);
    engine.rootContext()->setContextProperty("USCController" , g_pUSC4000SerialPort);


    engine.rootContext()->setContextProperty("CPMatrixController" , g_pPowerSerialPort->m_pMatrixContrller);
    engine.rootContext()->setContextProperty("CPPowerModelController" , g_pPowerSerialPort->m_pPowerModelConroller);
    engine.rootContext()->setContextProperty("CPNetMicController" , g_pPowerSerialPort->m_pNetMicController);
    engine.rootContext()->setContextProperty("CPListenSPKController" , g_pPowerSerialPort->m_pListenSPKController);



    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return;

//    mp_SDController->setSDQmlViewRootItem(engine.rootObjects().at(0));
    mp_APController->initAPDevices();

    mp_SPController->setSPQmlViewRootItem(engine.rootObjects().at(0));

    //功放事件回复
    connect(g_pAPServer , &TcpServer::serverAPEvent ,  mp_APController , &APController::gotServerAPEvent , Qt::QueuedConnection);

    //电源外围设备事件
//    connect(g_pPowerSerialPort , &PowerModeServer::emitPowerModeEvent , mp_APController , &APController::processEventToQmlUI);

    //USC4000 485 Event
//    connect(g_pUSC4000SerialPort , &RS485::emitRS485Event , mp_APController , &APController::processEventToQmlUI);

    //界面操作功放、电源管理 事件
//    connect(mp_APController , &APController::emitEventToBackstage , g_pAPServer , &TcpServer::processUISetAPEvent);
//    connect(mp_APController , &APController::emitEventToBackstage , g_pPowerSerialPort , &PowerModeServer::processUISetPowerModelEvent);


    //同步设置功放的输入设备编号
//    connect(mp_APController , &APController::synAPInputNo , g_pAPServer , &TcpServer::setAPInputNo);
    //设置功放进入紧急广播\停止广播-设置输入设备编号，还原原来输入编号
//    connect(mp_APController , &APController::setSysIsEmg , g_pAPServer , &TcpServer::setAllApBroadcastMode);

    //电源状态检测回复
//    connect(g_pPowerSerialPort , &PowerModeServer::powerEvent , mp_APController , &APController::setPowerModelState);
    //按键事件  自检、广播、停止广播
//    connect(g_pPowerSerialPort , &PowerModeServer::btnEvent , mp_APController , &APController::setPowerMdelBtnEvent);
//    设置外部状态灯显示 广播、紧急、设备故障 三个灯
//    connect(mp_APController , &APController::setLightOfDevBroEmg , g_pPowerSerialPort , &PowerModeServer::serialSetLightOfDevBroEmg);

    //USC4000 485 Event
//    connect(g_pUSC4000SerialPort , &RS485::rs485Event , mp_APController , &APController::setRS485EventToUI);

    //Self Test   电源检测模块自检   功放自检
//    connect(mp_APController , &APController::informPowerModeSelfTest , g_pPowerSerialPort , &PowerModeServer::powerModeSeltTest);
//    connect(mp_APController , &APController::informAMPSelfTest , g_pAPServer , &TcpServer::apSeltTest);

//    qDebug()<<engine.rootObjects().at(0)->findChild<QObject*>("objsdbuildview");




    mpGuiSHM = new QueueShareMemory(QSharedMemory::ReadWrite , QSharedMemory::ReadOnly);
    connect(&mt_ReadSHMTimer , &QTimer::timeout , this ,&UIController::readSHMTimerEvent);
    mt_ReadSHMTimer.start(200);

//    mpGuiSHM->writeSHMData(QByteArray("I am GUI 1"));
//    mpGuiSHM->writeSHMData(QByteArray("I am GUI 2"));
//    mpGuiSHM->writeSHMData(QByteArray("I am GUI 3"));
//    mpGuiSHM->writeSHMData(QByteArray("I am GUI 4"));
}
void UIController::readSHMTimerEvent()
{
    GUI_SHM_PACKET_HEADER *pHeader = NULL;
    if(mpGuiSHM->getSHMData(mq_SHMData)){
        foreach (QByteArray data, mq_SHMData) {
            pHeader = (GUI_SHM_PACKET_HEADER*)(data.data());
            if(data.length() < pHeader->packetsize){
                qDebug()<<FLINE<<"Got SHM Data Size Not Enough Packet Size:"<<pHeader->packetsize<<" Data Len:"<<data.data();
                return;
            }
//            mp_SDController->processData(data);
            mp_SPController->processData(data);
        }
    }
}

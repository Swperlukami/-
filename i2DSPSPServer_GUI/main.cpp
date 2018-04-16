#include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include "APServer/aptcpserver.h"
#include "PeripheryDevices/powermodeserver.h"
#include "USC4000/rs485.h"
#include "SysDataBase/database.h"
#include "SysDataBase/emgbroadcastdatabase.h"
#include "SysConfig/getsysconfig.h"
#include "uicontroller.h"


#define AP_SERVER_PORT         6020
#define POWER_MODEL_PORT       6030
#define MARTIX_SERVER_PORT     6040
#define BROADCAST_UDP_PORT     5000

QString g_ServerIp = "192.168.2.50";
QString g_PowerModeComName;
QString g_RS485ComName;
QString g_ListenSPKIP = "192.168.2.59";
unsigned char g_RS485Id;


TcpServer *g_pAPServer = NULL;
PowerModeServer *g_pPowerSerialPort = NULL ;
RS485 *g_pUSC4000SerialPort = NULL ;
UIController *g_pUIController = NULL;

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);

    if(!get_mc_info())
        exit(0);

    if(!connect_database())
        exit(0);
    if(!connect_broadcast_database())
        exit(0);

    TcpServer APServer;
    g_pAPServer = &APServer;

    PowerModeServer PowerModel;
    g_pPowerSerialPort = &PowerModel ;

    RS485 USC4000RS485("");
    g_pUSC4000SerialPort = &USC4000RS485 ;
    USC4000RS485.setSerialPortFormat(QSerialPort::Baud4800 , QSerialPort::Data8 , QSerialPort::MarkParity , QSerialPort::OneStop);
//    USC4000RS485.setSerialPortName(g_RS485ComName.simplified());



    UIController UiController;
    g_pUIController = &UiController;
    UiController.m_EventControllers.pChannelMusicController->getChannelMusicInfo();
    UiController.m_EventControllers.pAudioFileManageController->initSystemAudoFiles();

    if(!APServer.startTcpServer(g_ServerIp , AP_SERVER_PORT))
        exit(0);

    if(!PowerModel.startServer(g_ServerIp , POWER_MODEL_PORT)){
        exit(0);
    }

//    PowerModel.m_pPowerModelConroller->powerModelEvents("SelfTest");

    return app.exec();
}

#include "apcontroller.h"
#include <QApplication>
#include "SysDataBase/database.h"
#include "APServer/aptcpserver.h"
#include "PeripheryDevices/powermodeserver.h"


extern QString g_ListenSPKIP;
extern TcpServer *g_pAPServer;
extern PowerModeServer *g_pPowerSerialPort;


APController::APController(QObject *parent) : QObject(parent)
{

}
void APController::initAPDevices()
{
    QString apip , martixip , addr , apname ;
    int inputno ,  apstate ;

    QString strQuery = QString("select * from apinfo");
    QSqlQuery query;
    if(!query_database(strQuery , query))
        return;

    while (query.next()) {
        apip = query.value("apid").toString();
        martixip = query.value("martixip").toString();
        inputno = query.value("inputno").toInt();
        apstate = query.value("apstate").toInt();
        addr = query.value("addr").toString();
        apname = query.value("apname").toString();

        emit initAPInfo(apip , martixip , inputno , apstate , addr , apname);
    }
}
void APController::gotServerAPEvent(QByteArray apEventBuffer)
{
    UI_PACKET_HEAD *pUIPackHead = (UI_PACKET_HEAD*)apEventBuffer.data();
    UI_AP_STATE *pUIPackAPState = (UI_AP_STATE*)(apEventBuffer.data() + sizeof(UI_PACKET_HEAD));


    if(pUIPackHead->Flag != CMD_AP_STATE)
        return;

    QString APIp = QString(QLatin1String(pUIPackHead->APIp));
    APState apState = pUIPackAPState->spstate ;

    if(APIp == g_ListenSPKIP){
        if(apState == APOK || apState == APOnline){
            g_pPowerSerialPort->m_pListenSPKController->listenSPKConnected();
//            g_pPowerSerialPort->m_pListenSPKController->userSetListenSPKEvent("Disable");
            g_pPowerSerialPort->m_pListenSPKController->userSetListenSPKEvent("Enable");
        }
        else if(apState == APOffline){
            g_pPowerSerialPort->m_pListenSPKController->listenSPKDisconnected();
        }
        return;
    }

    QString apEventStr = "" ;

    switch (apState) {
    case APOK: apEventStr = "OK" ;
    case APShort: apEventStr = "Short" ;
    case APOPen: apEventStr = "Open" ;
    case APVoerDrive: apEventStr = "OverDrive" ;
    case APProtect: apEventStr = "Protect" ;
        emit apEvents(apEventStr , APIp);
        break;
    case APOnline:
    {
        apEventStr = "Online" ;
        emit apEvents("Online" , APIp);

//        emit synAPInputNo(APIp , 7);
    }
        break;
    case APOffline:
        emit apEvents("Offline" , APIp);
        apEventStr = "Offline" ;

        sys_device_info_record("AP:"+APIp+" Offline");

        break;
    }

    qDebug()<<FLINE<<"APContoller Got AP :"<<APIp <<"Event:"<<apEventStr;

}
void APController::userSetAPEvents(QString cmdType, QVariant parmas)
{
    qDebug()<<FLINE<<"User Set AP Event:"<<cmdType<<" "<<parmas;

    if(cmdType == "Emg"){
        qDebug()<<FLINE<<"Emg AP:"<<parmas.toString();
        g_pAPServer->setAPEmgMode(parmas.toString() , true);
    }
    else if(cmdType == "CancelEmg"){
        qDebug()<<FLINE<<"Cancel Emg AP:"<<parmas.toString();
        g_pAPServer->setAPEmgMode(parmas.toString() , false);
    }
    else if(cmdType == "SynInput"){
        g_pAPServer->setAPInputNo(parmas.toString().split('#').at(0) , parmas.toString().split('#').at(1).toInt());
    }
    else if(cmdType == "Volunme"){

    }
    else if(cmdType == "SelfTest"){
        g_pAPServer->apSeltTest();
    }
}
void APController::sys_device_info_record(QString msg)
{
    if(msg.isEmpty())
        return;

    QDir sysRecord(QDir::current().path()+"/sysRecord");
    if(!sysRecord.exists()){
        if(!sysRecord.mkdir(QDir::current().path()+"/sysRecord")){
            qDebug()<<"mkdir sysRecord failed";
        }
    }

    QString strRecordMsg = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")+"  "+msg+"\n";


    QString strFileName = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    QFile sysRecordFile("./sysRecord/"+strFileName);

    if(sysRecordFile.open(QFile::WriteOnly | QFile::Append)){
      if(strRecordMsg.length() != sysRecordFile.write(strRecordMsg.toLatin1())){
           qDebug()<<" write record file error";
      }
    }
    sysRecordFile.close();
}



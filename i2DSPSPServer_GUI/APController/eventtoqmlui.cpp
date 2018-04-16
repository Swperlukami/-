#include "apcontroller.h"
#include "SysProrocol/server_ui_protocol.h"
#include "SysDataBase/database.h"

extern QString g_ListenSPKIP;

void APController::processEventToQmlUI(QByteArray eventToUI)
{

}

void APController::setRS485EventToUI(QString event)
{
    if(event.contains("Fire")){
        event = event.mid(event.indexOf("#")+1);
        emit rs485EventToUI(event);
    }
    else if(event == "Reset"){
        emit rs485EventToUI(event);
    }
    else if(event == "SelfTest"){
        emit rs485EventToUI(event);
    }
}
void APController::setAPUiEvent(QString APIp, unsigned char apState)
{
    if(APIp == g_ListenSPKIP)
        return;

    switch (apState) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        emit apStateEvent(APIp , apState);
        break;
    case 6:
    {
        emit apStateEvent(APIp , 1);         //Online Syn Input No
        QString strQury = QString("select inputno from apinfo where apid = '%1' ").arg(APIp);
        QSqlQuery query;
        if(!query_database(strQury , query))
            return;
        query.first();

        sys_device_info_record("AP:"+APIp+" Online");

        QThread::msleep(50);
        if(sysIsEmg){
            emit synAPInputNo(APIp , 6);
            emit setAPEmg(APIp , true);
        }
        else if((sysIsBro || sysIsPalyRecordFile )&& APIp == g_ListenSPKIP)
            emit synAPInputNo(APIp , 6);
        else{
            emit synAPInputNo(APIp , 5);
//            emit synAPInputNo(APIp , query.value("inputno").toInt());

        }
//        emit synAPInputNo(APIp , 7);
    }
        break;
    case 7:
        emit apStateEvent(APIp , 0);
        sys_device_info_record("AP:"+APIp+" Offline");

        break;
    default:
        break;
    }
}

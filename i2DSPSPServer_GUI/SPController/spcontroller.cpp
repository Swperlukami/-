#include "spcontroller.h"
#include "SysDataBase/database.h"
#include "SysProrocol/mc_uiprotocol.h"

SPController::SPController(QObject *parent) : QObject(parent)
{
    mp_SPBuildingMapModel = NULL;
}
void SPController::getFloorDevice(int buildtype, QVariant buildindex)
{
    if(mp_SPBuildingMapModel == NULL){
        qDebug()<<FLINE<<"Error SP Building Model is NULL";
    }
    qDebug()<<FLINE<<"SP Usr Clicked "<<buildtype<<buildindex;
//    mp_SPBuildingMapModel->userClickedDistrickItem(buildtype , buildindex);
}
void SPController::setSPQmlViewRootItem(const QObject *rootqmlview)
{
//    mp_SPBuildingMapModel = new BuildingMapModel(rootqmlview->findChild<QObject*>("objspbuildview"));
    initSPDevices();
//    getLocalAudioFile();
}
void SPController::initSPDevices()
{
    QString strQuery = QString("select ltid,online,addr from lt");
    QSqlQuery query;
    if(!query_database(strQuery,query))
        return;
    QString ltisn, ltstate , ltaddr;
    while (query.next()) {
        ltisn = query.value("ltid").toString();
        int isltonline = query.value("online").toUInt();
        if(isltonline >0)
            ltstate = "green";
        else
            ltstate = "grey";

        ltaddr = query.value("addr").toString();

         emit spInit(ltisn , ltisn , 0 , ltstate , ltaddr , "" , 0);
    }


    strQuery = QString("select id,ltid,port,emglv,online,addr,apid,apspno from sensor");
    query.clear();
    if(!query_database(strQuery,query))
        return;
    QString spisn, spstate , spaddr , apid;
    int spport , spisonline , spisemg , apspno;
    while (query.next()) {
        ltisn = query.value("ltid").toString();
        spisn = query.value("id").toString();
        spport = query.value("port").toUInt();
        spisemg = query.value("emglv").toUInt();
        spisonline = query.value("online").toUInt();
        spaddr = query.value("addr").toString();
        apid = query.value("apid").toString();
        apspno = query.value("apspno").toUInt();

        if(spisonline >0)
            spstate = "green";
        else
            spstate = "grey";

        if(spisemg)
            spstate = "yellow";

        emit spInit(ltisn , spisn , spport , spstate , spaddr ,apid , apspno);

    }
//    emit spEvent("LTK000000056" , "SPK000000003" , 3 , "grey" ,"192.168.2.53" , 3);

}
void SPController::processData(QByteArray data)
{
    GUI_SHM_PACKET_HEADER *pGUIPacketHead = (GUI_SHM_PACKET_HEADER*)(data.data());
    GUI_SHM_LT_INFO *pLTInfo = (GUI_SHM_LT_INFO*)(data.data()+sizeof(GUI_SHM_PACKET_HEADER));
    GUI_SHM_SUB_DEV_INFO *pSPInfo = (GUI_SHM_SUB_DEV_INFO*)(data.data()+sizeof(GUI_SHM_PACKET_HEADER));

    if (pGUIPacketHead->devtype == DEVICE_SPEAKER) {
        QString ltisn = QString(pSPInfo->ltisn);
        QString spisn = QString(pSPInfo->subdevisn);
        int spno = pSPInfo->subdevno;
        QString spevent = "" , spstate = "green";
        switch (pSPInfo->cmd) {
        case CMD_GUI_DEVICE_ON:
            spevent = "Online";
            spstate = "green";
            break;
        case CMD_GUI_DEVICE_OFF:
            spevent = "Offline";
            spstate = "grey";
            break;
        case CMD_START_ALARM:
            spevent = "Emg";
            spstate = "yellow";
            break;
        case CMD_CANCEL_ALARM:
            spevent = "CancelEmg";
            spstate = "green";
            break;
        }

        QString apid = "" ; int apspno = 0 ;
        QString strQuery = QString("select apid , apspno from sensor where id='%1' ").arg(spisn);
        QSqlQuery query;
        if(!query_database(strQuery , query))
            return;
        query.first();
        apid = query.value("apid").toString();
        apspno = query.value("apspno").toUInt();
        qDebug()<<FLINE<<"SPK <<<<<<<<<<<<<<<<<<<<<<<<"<<ltisn<<spisn<<apid<<spno<<apspno;
       emit spEvent(ltisn , spisn , spno ,  spstate , apid , apspno);
    }
    else if(pGUIPacketHead->devtype == DEVICE_LT){
        QString ltisn = QString(pLTInfo->ltisn);
        QString ltevent = "" , ltstate = "";
        switch (pLTInfo->cmd) {
        case CMD_GUI_DEVICE_ON:
            ltevent = "Online";
            ltstate = "green";
            break;
        case CMD_GUI_DEVICE_OFF:
            ltevent = "Offline";
            ltstate = "grey";
            break;
        }
        qDebug()<<FLINE<<"LT <<<<<<<<<<<<<<<<<<<<<<<<<<<<<"<<ltisn<<ltevent;
        emit spEvent(ltisn , ltisn , 0 ,  ltstate , "" , 0);
    }
    else {
        return;
    }
}
void SPController::resAPSPStateEvent(QString APIp, QString SPId, int SPNo, int SPState)
{
    QString spState = "grey";
    if(SPState == CMD_GUI_DEVICE_ON)
        spState = "green" ;
    else if(SPState == CMD_START_ALARM)
        spState = "yellow" ;

    qDebug()<<"AP :"<<APIp<<" SPID :"<<SPId<<" SPNo :"<<SPNo<<" State: "<<SPState;
    emit spEvent(APIp , SPId , SPNo , spState , APIp , SPNo);
}

QString SPController::getSPDetailInfo(QString ltid, int spno)
{
    QString spAddr="" , ltAddr="" , apAddr="" , emgtime="" , strstate = "" , apip = "";
    int online = 0  , emglv = 0 ;
    QString strQuery = QString("select online ,addr , emglv , emgtime , apid from sensor where ltid='%1' and port=%2 ").arg(ltid).arg(spno);
    QSqlQuery query;
    if(query_database(strQuery , query)){
        query.first();
        online = query.value("online").toInt();
        spAddr =  query.value("addr").toString();
        emglv = query.value("emglv").toInt();
        emgtime = query.value("emgtime").toString();
        apip = query.value("apid").toString();

        if(online <= 0)
            strstate = "喇叭掉线";

        if(emglv > 0 )
            strstate = "喇叭断开";
        else
            emgtime = "";

    }

    strQuery.clear(); query.clear();
    strQuery = QString("select addr from apinfo  where apid='%1' ").arg(apip);
    if(query_database(strQuery , query)){
        query.first();
        apAddr = query.value("addr").toString();
    }

    qDebug()<<FLINE<<emgtime+"#"+strstate+"#"+apAddr+" "+ltAddr+" "+spAddr;
    apAddr = "";
    return emgtime+"#"+strstate+"#"+apAddr+""+ltAddr+" "+spAddr;

    qDebug()<<FLINE<<ltid<<spno;
}

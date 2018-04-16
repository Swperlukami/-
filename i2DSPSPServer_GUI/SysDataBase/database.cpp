#include "database.h"
#include "SysProrocol/mc_uiprotocol.h"
#include <QFile>
QSqlDatabase g_MCDataBase;

bool connect_database()
{
    g_MCDataBase = QSqlDatabase::addDatabase("QSQLITE" , "mcconnection");
    g_MCDataBase.setDatabaseName("/home/i2dsp-chengdu/AdrianProgram/build-EmgBraodcast-Desktop_Qt_5_9_1_GCC_64bit-Debug/i2DSPLocalServer/i2dsp.db");
    if(g_MCDataBase.open()){
        qDebug()<<FLINE<<g_MCDataBase.databaseName()<<" Open Successed";
        return true;
    }
    qDebug()<<FLINE<<g_MCDataBase.databaseName()<<" Open Failed";
    return false;
}
bool exec_database(QString statement)
{
    if(statement.isEmpty())
        return false;
    QSqlQuery  query(g_MCDataBase);
    if(!query.exec(statement)){
        qDebug()<<FLINE<<"Sql Query Error ??"<<statement<<" Error:"<<query.lastError();
        return false;
    }
    return true;
}
bool query_database(QString statement, QSqlQuery &query)
{
    if(statement.isEmpty() || &query == NULL)
        return false;
    QSqlQuery  Query(g_MCDataBase);
    if(!Query.exec(statement)){
        if(!Query.isSelect() || !Query.isValid()){
            qDebug()<<FLINE<<"Sql Query ??"<<statement<<" Error:"<<Query.lastError();
            query.clear();
            return false;
        }
    }
    query = Query;
    return true;
}
bool get_floor_devices(int devtype, int buildno, int floorno , QSqlQuery &query)
{
    QString strQuery;
    if(devtype == DEVICE_LT){
         strQuery = QString("selcet ltid,xpos,ypos,online from lt where buildingnum=%1 and floornum=%2")
                .arg(buildno).arg(floorno);
    }
    else if(devtype == DEVICE_SD100){
         strQuery = QString("selcet id,ltid,port,xpos,ypos,online,emglv,power from sensor where buildingnum=%1 and floornum=%2")
                .arg(buildno).arg(floorno);
    }
    else if(devtype == DEVICE_SPEAKER){
         strQuery = QString("selcet id,ltid,port,xpos,ypos,online,emglv,power from sensor where buildingnum=%1 and floornum=%2")
                .arg(buildno).arg(floorno);
    }
    QSqlQuery myquery(g_MCDataBase);
    if(!myquery.exec(strQuery)){
        query.clear();
        return false;
    }
    else{
        query = myquery;
        return true;
    }
}
void init_test_database()
{
    //lt
    QSqlQuery ltQuery(g_MCDataBase);
    int ltno = 56;
    QString sdid = "SPK0000000"; int sdno = 1;
    QString ltid = "LTK0000000" ; QString mcid = "MC0000000001"; int buildnum = 9; int floor = 1; int xpos = 10; int ypos = 10;
    QString addr = "层";
    for(int i = 0 ; i < 1 ; i++){
        if(i == 1){
            floor = 2;
        }
        ltid = "LTK0000000"+QString::number(ltno);
        QString strQuery = QString("insert into lt(ltid,mcid,buildingnum,floornum,xpos,ypos,addr,online,apip) values('%1','%2',%3,%4,%5,%6,'%7',%8 ,'%9' )")
                .arg(ltid).arg(mcid).arg(buildnum).arg(floor)
                .arg(xpos).arg(ypos).arg(QString::number(i+1)+addr).arg(0)
                .arg("192.168.2.53");

        if(!ltQuery.exec(strQuery)){
            qDebug()<<"Insert test lt error";
        }

        ltno++;
        xpos+=30;
        ypos+=30;


        for(int j = 1 ; j < 20 ; j++){
            QString strsdQuery = QString("insert into sensor(id,ltid,port,buildingnum,floornum,xpos,ypos,addr,type,emglv,online,power,emgno,emgtime) "
                                         "values('%1','%2',%3,%4,%5,%6,%7,'%8',%9,%10,%11,%12,%13,'%14')")
                    .arg(sdid+QString().sprintf("%02d",sdno)).arg(ltid).arg(j).arg(buildnum).arg(floor)
                    .arg(xpos).arg(ypos).arg(QString::number(j)+"教室")
                    .arg(3).arg(1).arg(0).arg(0).arg(0).arg("2017-11-11 11:11:11");

            sdno++;
            xpos+=30;
            ypos+=30;

            if(!ltQuery.exec(strsdQuery)){
                qDebug()<<"Insert test sd error";
            }
        }
        xpos=30;
        ypos=30;

    }

    QString apid = "192.168.2." , martixip = "192.168.2.51" , apaddr = "教学楼" , apname = "教学楼一区";
    int inputno = 0 , state = 0;
    for(int remoteapcnt = 53 ; remoteapcnt < 56 ; remoteapcnt++){
        QString strQuery = QString("insert into apinfo(apid,martixip,inputno,apstate,addr,apname) values('%1' , '%2' , %3 , %4 , '%5' , '%6' )")
                .arg(apid+QString::number(remoteapcnt)).arg(martixip).arg(inputno).arg(state).arg(apaddr+QString::number(remoteapcnt)+"区").arg(apname);

        if(!ltQuery.exec(strQuery)){
            qDebug()<<"Insert apinfo error";
        }
    }


}

void delete_database()
{
    QString strQuery = "delete from lt";
    QSqlQuery query(g_MCDataBase);
    if(!query.exec(strQuery))
        qDebug()<<"database clear error";
    strQuery = "delete from sensor";
    if(!query.exec(strQuery))
        qDebug()<<"database clear error";
    strQuery = "delete from apinfo";
    if(!query.exec(strQuery))
        qDebug()<<"database clear error";
}











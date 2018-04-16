#include "emgbroadcastdatabase.h"
#include "SysProrocol/mc_uiprotocol.h"
#include <QFile>

QSqlDatabase g_EmgBroadcastDataBase;


bool connect_broadcast_database(){
    g_EmgBroadcastDataBase = QSqlDatabase::addDatabase("QSQLITE" , "emgconnection");
    g_EmgBroadcastDataBase.setDatabaseName("EmgBrodcastSQlite.db");
    if(g_EmgBroadcastDataBase.open()){
        qDebug()<<FLINE<<g_EmgBroadcastDataBase.databaseName()<<" Open Successed";
        return true;
    }
    qDebug()<<FLINE<<g_EmgBroadcastDataBase.databaseName()<<" Open Failed";
    return false;
}
bool exec_broadcast_database(QString statement){
    if(statement.isEmpty())
        return false;
    QSqlQuery  query(g_EmgBroadcastDataBase);
    if(!query.exec(statement)){
        qDebug()<<FLINE<<"Sql Query Error ??"<<statement<<" Error:"<<query.lastError();
        return false;
    }
    return true;
}
bool query_broadcast_database(QString statement, QSqlQuery &query){
    if(statement.isEmpty() || &query == NULL)
        return false;
    QSqlQuery  Query(g_EmgBroadcastDataBase);
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

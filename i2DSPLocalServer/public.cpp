#include "public.h"
#include "mc_protocol.h"

QSqlDatabase g_MCDataBase;
void get_mc_info()
{

}
bool connect_database()
{
    g_MCDataBase = QSqlDatabase::addDatabase("QSQLITE" , "mcconnection");
    g_MCDataBase.setDatabaseName("i2dsp.db");
    if(g_MCDataBase.open()){
        qDebug()<<FLINE<<g_MCDataBase.databaseName()<<" Open Successed";
        return true;
    }
    qDebug()<<FLINE<<g_MCDataBase.databaseName()<<" Open Failed";
    return false;

}
bool init_database()
{
    QString strQuery = QString("update lt set emglv=0 , socket = 0 , online = 0");
    if(!exec_database(strQuery)){
        qDebug()<<FLINE<<"Init DataBase lt Table Error";
        return false;
    }
    strQuery = QString("update sensor set emglv=0 , online = 0 , state = 0 , power = 0 , emgno = 0 , emgtime = NULL");
    if(!exec_database(strQuery)){
        qDebug()<<FLINE<<"Init DataBase sensor Table Error";
        return false;
    }
    qDebug()<<FLINE<<"Init DataBase OK";
    return true;


}
bool query_database(QString statement, QSqlQuery &query)
{
    if(statement.isEmpty() || &query == NULL)
        return false;
    QSqlQuery  Query(g_MCDataBase);
    if(!Query.exec(statement)){
        qDebug()<<FLINE<<"Sql Query ??"<<statement<<" Error:"<<query.lastError();
        query.clear();
        return false;
    }
    query = Query;
    return true;


}
bool exec_database(QString statement)
{
    if(statement.isEmpty())
        return false;
    QSqlQuery  query(g_MCDataBase);
    if(!query.exec(statement)){
        qDebug()<<FLINE<<"Sql Query Error ??"<<statement<<" Error:"<<query.lastError();
        sys_device_info_record(statement);
        return false;
    }
    return true;
}
int get_device_emglv(QString Ltid, int Port)
{
    if(Ltid.isEmpty() || Port <= 0)
        return -1;
    QString strQuery = QString("select emglv from sensor where ltid='%1' and port=%2")
            .arg(Ltid).arg(Port);
    QSqlQuery query;
    if(!query_database(strQuery , query)){
        qDebug()<<FLINE<<"Get Device emglv ltid: "<<Ltid<<" port:"<<Port<<" Error";
        return -1;
    }
    query.first();
    if(query.isValid())
        return query.value("emglv").toUInt();
    else
        return -1;

}
QString get_device_isn_by_ltid_and_port(QString Ltid, int Port)
{
    QString strQuery = QString("select id from sensor where ltid='%1' and port=%2").arg(Ltid).arg(Port);
    QSqlQuery query;
    if(!query_database(strQuery , query))
        return "";
    query.first();
    if(query.isValid())
        return query.value(0).toString();
    else{
        qDebug()<<FLINE<<"Got ltid:"<<Ltid<<"Port:"<<Port<<"Error";
        return "";
    }
}

void sys_device_event_sql_reord(QString id, int devType, int eventType)
{
    QSqlQuery query(g_MCDataBase);
    query.prepare("insert into record values(?,?,?,?)"); QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    query.bindValue(0,id); query.bindValue(1,time);query.bindValue(2,devType);query.bindValue(3,eventType);
    if(!query.exec())
        qDebug()<<FLINE<<"Recrod Device Event Error";
}

void sys_device_info_record(QString msg)
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

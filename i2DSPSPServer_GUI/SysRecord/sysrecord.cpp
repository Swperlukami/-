#include "sysrecord.h"

SysRecord::SysRecord(QObject *parent) : QObject(parent)
{

}
void SysRecord::systemLogRcord(QString msg)
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

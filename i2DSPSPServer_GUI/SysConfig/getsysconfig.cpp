#include <QFile>
#include <QString>
#include "getsysconfig.h"
#include "SysProrocol/server_ui_protocol.h"


extern QString g_ServerIp;
extern QString g_PowerModeComName;
extern QString g_RS485ComName;
extern QString g_ListenSPKIP;
extern unsigned char g_RS485Id;

bool get_mc_info()
{
    QFile ServerConfigFile("./Config/config.ini");
    if(ServerConfigFile.open(QFile::ReadOnly)){
        while (!ServerConfigFile.atEnd()) {
            QString strTmp = ServerConfigFile.readLine();
            if(strTmp.contains("server ip")){
                g_ServerIp = strTmp.mid(strTmp.indexOf(":")+1);
            }
            else if(strTmp.contains("power com")){
                g_PowerModeComName = strTmp.mid(strTmp.indexOf(":")+1);
            }
            else if(strTmp.contains("rs485 com")){
                g_RS485ComName = strTmp.mid(strTmp.indexOf(":")+1);
            }
            else if(strTmp.contains("spk ip")){
                g_ListenSPKIP = strTmp.mid(strTmp.indexOf(":")+1).simplified();
            }
            else if(strTmp.contains("rs485 id")){
                g_RS485Id = strTmp.mid(strTmp.indexOf(":")+1).toUInt();
            }
        }
        if(!g_ServerIp.isEmpty()){
            qDebug()<<FLINE<<"Got Server IP";
            return true;
        }
        else{
            qDebug()<<FLINE<<"Got Server IP Error";
            return false;
        }
    }
    else{
        qDebug()<<FLINE<<"Got Server IP Error";
        return false;
    }

}

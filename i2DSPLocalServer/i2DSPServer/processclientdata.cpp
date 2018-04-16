#include "tcpsocket.h"
#include "mc_protocol.h"
#include "senduidata.h"

void TcpSocket::processData(const QByteArray cmd)
{
    NETPACKET_HEADER *pHeader;
    TCP_PACK_REGISTER *pRegister;
    DEVICE_SD_INFO  *pSDInfo;

    TCP_PACK_CMD *pCmd;
    TCP_TER_EMG_INFO *pEmgInfo;

    pHeader = (NETPACKET_HEADER*)cmd.data();
    pRegister = (TCP_PACK_REGISTER*)(cmd.data() + sizeof(NETPACKET_HEADER));
    pSDInfo = (DEVICE_SD_INFO*)(cmd.data() + sizeof(NETPACKET_HEADER) + sizeof(TCP_PACK_REGISTER));
    pCmd = (TCP_PACK_CMD*)(cmd.data() + sizeof(NETPACKET_HEADER));
    pEmgInfo = (TCP_TER_EMG_INFO*)(cmd.data() + sizeof(NETPACKET_HEADER) + sizeof(TCP_PACK_CMD));


    QString strQuery;
    QSqlQuery query;
    int deviceType = 0;
    QString strDeviceType;
    QString strDeviceEvent;


    mstrLTISN = QString(QLatin1String(pHeader->DeviceId));
    int SDNo = -1;
    QString strSDISN;

    switch (pHeader->Flag) {
    case NETPACKET_HEADER_REGISTER_FLAG:      //LT / SD100 的注册包  SD100的掉线包
        if(pRegister->RegisterMode == REGISTER_ONLY){
            mLTRegisterFlag = true;
            deviceType = DEVICE_LT ; SDNo = 0;strDeviceEvent = "Online";
            //Send Data UI
            if(deviceType == DEVICE_LT && SDNo == 0)
                lt_on_off_line_(mstrLTISN , true);

            strQuery = QString("update lt set type=%1 , socket=%2 , online=1 where ltid='%3' ")
                    .arg(deviceType).arg(mSocketDescriptor).arg(mstrLTISN);
        }
        else if(pRegister->RegisterMode == REGISTER_ADD){
            SDNo = pSDInfo->DeviceNO ; strDeviceEvent = "Online";
            qDebug()<<FLINE<<"SubDev:"<<mstrLTISN<<"Port:"<<SDNo<<" Online";
            strQuery = QString("update sensor set type=%1 , online=1 where ltid='%2' and port=%3 ")
                    .arg(deviceType).arg(mstrLTISN).arg(SDNo);
        }
        else if(pRegister->RegisterMode == REGISTER_DROP){
            SDNo = pSDInfo->DeviceNO ; strDeviceEvent = "Offline";
            strQuery = QString("update sensor set type=%1 , online=0 , emglv=0 , emgtime=NULL where ltid='%2' and port=%3 ")
                    .arg(0).arg(mstrLTISN).arg(SDNo);
        }

        if(pRegister->RegisterType == DEVICE_SD100)
            deviceType = DEVICE_SD100;
        else if (pRegister->RegisterType == DEVICE_SPEAKER)
            deviceType = DEVICE_SPEAKER;
        if(pRegister->RegisterType == DEVICE_SD100 || pRegister->RegisterType == DEVICE_SPEAKER){
            strSDISN = get_device_isn_by_ltid_and_port(mstrLTISN , SDNo);
            sys_device_event_sql_reord(strSDISN , deviceType , pRegister->RegisterMode);


        }

        //Send Data UI
        sub_dev_event(strSDISN,mstrLTISN,deviceType,SDNo,pRegister->RegisterMode);


        break;
    case NETPACKET_HEADER_COMMAND_FLAG:      //SD100 报警/取消报警
    {
        SDNo = pEmgInfo->LtPort;
        if(pEmgInfo->DeviceType == DEVICE_SD100)
            deviceType = DEVICE_SD100;
        else if(pEmgInfo->DeviceType == DEVICE_SPEAKER)
            deviceType = DEVICE_SPEAKER;

        strSDISN = get_device_isn_by_ltid_and_port(mstrLTISN , SDNo);
        sys_device_event_sql_reord(strSDISN , deviceType , pCmd->Cmd);


        if(pCmd->Cmd == CMD_START_ALARM){
            strDeviceEvent = "Emg";  static QString strFirstEmgTime;
            int emglv = get_device_emglv(mstrLTISN , SDNo);
            if(emglv < 0)
                return;
            emglv++;

            if(deviceType == DEVICE_SD100 && emglv == SD100_EMG_TRIG_TIMES){
                strFirstEmgTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
                //Send Data UI
                sub_dev_event(strSDISN,mstrLTISN,deviceType,SDNo,pCmd->Cmd);

            }
            else if(deviceType == DEVICE_SPEAKER && emglv == SPEAKER_EMG_TRIG_TIMES){
                strFirstEmgTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
                //Send Data UI
                sub_dev_event(strSDISN,mstrLTISN,deviceType,SDNo,pCmd->Cmd);
            }

            strQuery = QString("update sensor set emglv=%1 , emgtime='%2' where ltid='%3' and port=%4 ")
                    .arg(emglv).arg(strFirstEmgTime).arg(mstrLTISN).arg(SDNo);
        }
        else if(pCmd->Cmd == CMD_CANCEL_ALARM){
            strDeviceEvent = "Cancel Emg";
            //Send Data UI
            sub_dev_event(strSDISN,mstrLTISN,deviceType,SDNo,pCmd->Cmd);

            strQuery = QString("update sensor set emglv=0 , emgno=0, emgtime=NULL where ltid='%1' and port=%2 ")
                    .arg(mstrLTISN).arg(SDNo);
        }
        else if(pCmd->Cmd == CMD_LOW_POWER_ALARM){
            strDeviceEvent = "Low Power Emg";
            //Send Data UI
            sub_dev_event(strSDISN,mstrLTISN,deviceType,SDNo,pCmd->Cmd);

            strQuery = QString("update sensor set power=1  where ltid='%1' and port=%2 ")
                    .arg(mstrLTISN).arg(SDNo);
        }
        else if(pCmd->Cmd == CMD_CANCEL_LOW_POWER_ALARM){
            strDeviceEvent = "Cancel Low Power Emg";
            //Send Data UI
            sub_dev_event(strSDISN,mstrLTISN,deviceType,SDNo,pCmd->Cmd);

            strQuery = QString("update sensor set power=0  where ltid='%1' and port=%2 ")
                    .arg(mstrLTISN).arg(SDNo);
        }
        else if (pCmd->Cmd == CMD_ACTIVE_NITIFY) {
            strDeviceEvent = "Active Nofify";
        }
    }
        break;
    case NETPACKET_HEADER_LT_DETECT_FLAG:    //LT探测保活
        mLTLive = 2; SDNo = 0;
//        qDebug()<<FLINE<<"Keep live Packet::"<<mstrLTISN<<"Live:"<<mLTLive;
        break;
    case NETPACKET_HEADER_DROP_FLAG:         //LT掉线，此处服务器检测，不使用。原MC采用线程事件处理，不区分掉线事件
        break;
    default:
        break;
    }

    QString strSDNo = QString::number(SDNo) ;
    switch (deviceType) {
    case DEVICE_LT:strDeviceType = "LT"; break;
    case DEVICE_SD100:strDeviceType = "SD"; break;
    case DEVICE_SPEAKER:strDeviceType = "SP"; break;
    default: strDeviceType = "UN"; break;
    }

    QString strRecord;

    if(SDNo < 0 || SDNo > MAX_LT_PORT_CNT){
        strRecord = "SDNo Error ??"+strSDNo+" lt isn:"+mstrLTISN;
        sys_device_info_record(strRecord);
        qDebug()<<FLINE<<strRecord;
        return;
    }

    if(!strQuery.isEmpty()){
        if(!exec_database(strQuery)){
            qDebug()<<FLINE<<strDeviceType+">> lt ip: "+mstrLTIP+" isn: "+mstrLTISN+" Port: "+strSDNo+" "+
                      strDeviceEvent<<" Update DataBase Error";
            return;
        }
    }

    if(pHeader->Flag != NETPACKET_HEADER_LT_DETECT_FLAG){
        strRecord = strDeviceType+">> lt ip: "+mstrLTIP+" isn: "+mstrLTISN+" Port: "+strSDNo+" "+strDeviceEvent;
        sys_device_info_record(strRecord);
        qDebug()<<FLINE<<strRecord;
    }



}

#include "senduidata.h"
#include "public.h"

extern QueueShareMemory *gp_SHM;

void lt_on_off_line_(QString ltisn, bool isonline)
{
    QByteArray buffer(1024,0);
    GUI_SHM_PACKET_HEADER *pHeader = (GUI_SHM_PACKET_HEADER*)buffer.data();
    GUI_SHM_LT_INFO *pLtInfo = (GUI_SHM_LT_INFO*)(buffer.data()+sizeof(GUI_SHM_PACKET_HEADER));

    pHeader->devtype = DEVICE_LT;
    pHeader->packetsize = sizeof(GUI_SHM_PACKET_HEADER)+sizeof(GUI_SHM_LT_INFO);

    if(isonline)
        pLtInfo->cmd = CMD_GUI_DEVICE_ON;
    else
        pLtInfo->cmd = CMD_GUI_DEVICE_OFF;
    strncpy(pLtInfo->ltisn , ltisn.toLatin1().data() , MAX_DEVICE_ID_LEN);

    gp_SHM->writeSHMData(buffer);

}
void sub_dev_event(QString subdevisn, QString ltisn ,int devtype,int port, int cmd)
{
    if(subdevisn.isEmpty() || port < 0 || port > MAX_LT_PORT_CNT){
        qDebug()<<FLINE<<"Send UI Device Info Error";
        return;
    }

    qDebug()<<FLINE<<"Send UI Sub Device Event"<<subdevisn<<port;

    QByteArray buffer(1024,0);
    GUI_SHM_PACKET_HEADER *pHeader = (GUI_SHM_PACKET_HEADER*)buffer.data();
    GUI_SHM_SUB_DEV_INFO *pSubDevInfo = (GUI_SHM_SUB_DEV_INFO*)(buffer.data()+sizeof(GUI_SHM_PACKET_HEADER));

    pHeader->devtype = devtype;
    pHeader->packetsize = sizeof(GUI_SHM_PACKET_HEADER)+sizeof(GUI_SHM_SUB_DEV_INFO);

    switch (cmd) {
    case REGISTER_ADD:
        pSubDevInfo->cmd = CMD_GUI_DEVICE_ON;
        break;
    case REGISTER_DROP:
        pSubDevInfo->cmd = CMD_GUI_DEVICE_OFF;
        break;
    case CMD_START_ALARM:
    case CMD_CANCEL_ALARM:
    case CMD_LOW_POWER_ALARM:
    case CMD_CANCEL_LOW_POWER_ALARM:
        pSubDevInfo->cmd = cmd;
        break;
    }

    pSubDevInfo->subdevno = port;
    strncpy(pSubDevInfo->ltisn , ltisn.toLatin1().data() , MAX_DEVICE_ID_LEN);
    strncpy(pSubDevInfo->subdevisn , subdevisn.toLatin1().data() , MAX_DEVICE_ID_LEN);

    gp_SHM->writeSHMData(buffer);

}

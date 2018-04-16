#ifndef MC_UIPROTOCOL_H
#define MC_UIPROTOCOL_H
#include "mc_protocol.h"

#pragma pack(1)
typedef struct{
    unsigned int packetsize;
    unsigned char devtype;
}GUI_SHM_PACKET_HEADER;
#pragma pack()

#pragma pack(1)
typedef struct{
    unsigned char cmd;
    char ltisn[MAX_DEVICE_ID_LEN];
}GUI_SHM_LT_INFO;
#pragma pack()

#pragma pack(1)
typedef struct{
    unsigned int cmd;
    unsigned char subdevno;
    char ltisn[MAX_DEVICE_ID_LEN];
    char subdevisn[MAX_DEVICE_ID_LEN];
}GUI_SHM_SUB_DEV_INFO;
#pragma pack()


#endif // MC_UIPROTOCOL_H

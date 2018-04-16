#ifndef SERVER_UI_PROTOCOL_H
#define SERVER_UI_PROTOCOL_H


#include "APServer/ap_protocol.h"
#define FLINE __FILE__<<"Line:"<<__LINE__


enum APState{APOK = 1 , APShort , APOPen , APVoerDrive , APProtect , APOnline , APOffline} ;
#define IP_LEN    16


#pragma pack(1)
typedef struct{
    unsigned short Flag;                  //通过flag判断包的类型
    unsigned short PacketSize;            //包体信息的大小
    char APIp[IP_LEN] ;
}UI_PACKET_HEAD ;
#pragma pack()


#pragma pack(1)
typedef struct{
     APState spstate;
}UI_AP_STATE ;
#pragma pack()

#pragma pack(1)
typedef struct{
     bool APIsEmg;
}UI_AP_EMG_MODE ;
#pragma pack()

#pragma pack(1)
typedef struct{
     unsigned char APVoice;
}UI_AP_VOICE ;
#pragma pack()

#endif // SERVER_UI_PROTOCOL_H

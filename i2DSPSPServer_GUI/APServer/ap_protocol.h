#ifndef AP_PROTOCOL_H
#define AP_PROTOCOL_H

#define CMD_AP_KEEP_LIVE  0x0001          //客户端每隔15S发送心跳包 30秒没有收到心跳包，服务器关闭客户端
#define CMD_AP_STATE      0x0002          //回馈功放状态
#define CMD_AP_INPUT      0x0003          //设置功放输入设备编号
#define CMD_SP_STATE      0x0004          //定组功放下属喇叭检测
#define MAX_SP_CNT        0x0001

#define CMD_SELF_TEST     0x0005        //设备自检
#define CMD_MC_EMG        0x0006        //通知功放应急广播，音量最大不可调
#define CMD_MC_CANCEL_EMG 0x0007        //通知功放取消应急，音量还原

#define CMD_AP_VOL        0x0008

#pragma pack(1)
typedef struct
{
    unsigned short Flag;                  //通过flag判断包的类型
    unsigned short PacketSize;            //包体信息的大小
}NETPACKET_HEADER;
#pragma pack()

#pragma pack(1)
typedef struct
{
    unsigned char APState;              //功放->服务器   回复功放状态 1 正常 2 短路 3 断路 4 欠压 5 预短路  6 online 7 offline
}NETPACKET_AP_STATE;
#pragma pack()

#pragma pack(1)
typedef struct
{
    unsigned char APVol;              //AP Volice 0 - 12 0 mute
}NETPACKET_AP_VOL;
#pragma pack()

#pragma pack(1)
typedef struct
{
    unsigned char InputNo;              //服务器->功放   设置该功放，接受音频输入设备编号(1-6 , 0 no input)
}NETPACKET_AP_INPUT;
#pragma pack()

#pragma pack(1)
typedef struct
{
    unsigned char SPState[MAX_SP_CNT];             //功放->服务器  0 正常   1 故障
}NETPACKET_SP_STATE;
#pragma pack()




#endif // AP_PROTOCOL_H

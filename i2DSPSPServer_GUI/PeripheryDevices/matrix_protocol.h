#ifndef MATRIX_PROTOCOL_H
#define MATRIX_PROTOCOL_H

#include "SysDataBase/database.h"

/********参数**********
IP:192.168.2.60
Port:6030
*************************/

#define MARTIX_SONG_NAME_MAX	90    	//歌曲名最大长度(UFT-8编码)

#define PACKAGE_ID				0x5a
#define MERGEPACKAGE_ID			(PACKAGE_ID<<8|PACKAGE_ID)

//命令定义
#define MARTIX_CMD_PLAY       	0x01   	//开始播放,包结构:包头+歌名+音量
#define MARTIX_CMD_PAUSE      	0x02   	//暂停播放,包结构:包头
#define MARTIX_CMD_CONTINUE   	0x03   	//继续播放,包结构:包头
#define MARTIX_CMD_SET_VOL		0x04	//音量调节,包结构:包头+音量
#define MARTIX_CMD_EMG_PLAY    	0x05   	//紧急广播,包结构:包头+歌名+音量(所有通道广播)
#define MARTIX_CMD_EMG_STOP   	0x06   	//停止紧急广播,包结构:包头(所有通道停止)
#define MARTIX_CMD_MOUNT_DISK	0x07	//挂载磁盘(忽略通道号等参数),命令发出后,需要等待回码确认.
#define MARTIX_CMD_UNMOUNT_DISK	0x08	//卸载磁盘(忽略通道号等参数),命令发出后,需要等待回码确认.
//#define MARTIX_CMD_QUERY_SONG	0x09	//查询歌曲是否存在:包头+歌名

#define MARTIX_MAX_VOL          254

#define RESP_OK			                    0x00
#define RESP_BITRATE_NOTSUPPORT				0x01
#define RESP_FILE_OPEN_FAILED			    0x02
#define RESP_FILE_FORMAT_NOTSUPPORT		    0x03

//命令数据结构定义(PC->设备)
#pragma pack(1)
typedef struct
{
    unsigned short szID;		//ID
    unsigned char cmd;			//命令
    unsigned char channelNum;	//通道号

    unsigned short szLength;	//本结构长度
    unsigned short data_size;	//附加的数据长度
}MARTIX_PACKET_HEADER;
#pragma pack()

#pragma pack(1)
typedef struct
{
     unsigned char vol;     	//音量值 0-254
}MARTIX_VOL;
#pragma pack()

#pragma pack(1)
typedef struct
{
     char songName[MARTIX_SONG_NAME_MAX];	//歌曲名称(含后缀名)
     MARTIX_VOL cur_vol;					//当前播放音量
}MARTIX_SONG_INFO;
#pragma pack()

//回应数据包结构定义(设备->PC)
#pragma pack(1)
typedef struct
{
    unsigned short szID;		//ID

    unsigned char resp_cmd;			//回应的命令

    unsigned short szLength;	//本结构长度
    unsigned short data_size;	//附加的数据长度
}MARTIX_RESPONSE_HEADER;
#pragma pack()



#pragma pack(1)
typedef struct
{
    unsigned char channnelNum ;      // 0-4
    unsigned char resp_status;		//命令执行的状态:1-成功,0-失败.
}MARTIX_RESPONSE_1;
#pragma pack()

#endif	//MATRIX_PROTOCOL_H

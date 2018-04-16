#ifndef USC4000_PROTOCOL_H
#define USC4000_PROTOCOL_H

#pragma pack(1)
typedef struct
{
     char synCode[2];              //同步码
     int dataLen;                  //数据长度，高位在前地位在后（大端） 命令码+数据的总长度，不包含校验码
     char cmd;                     //命令码 0x50 巡检(USC回复：0x50-正确 0x70-错误)    0x51 火警   0x52 复位（连续3次有效）  0x53 自检（连续3次有效）
}USC4000_PACK_HEADER;
#pragma pack()

#pragma pack(1)
typedef struct
{
     char usc4000No;               //USC主机号
     char loopNo;                  //回路号
     char addrNo;                  //地址号
     char devType;                 //设备类型
}USC4000_FIRE_INFO;
#pragma pack()

#pragma pack(1)
typedef struct
{
    int checksum;                 //高位在前地位在后（大端） 命令码+数据的总长度，不包含校验码
}USC4000_PACK_CHECKSUM;
#pragma pack()
#endif // USC4000_PROTOCOL_H

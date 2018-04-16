#ifndef MC_PROTOCOL_H
#define MC_PROTOCOL_H

/*********************************************************
 *                         相关参数的定义
 * *******************************************************/
#define MAX_DEVICE_ID_LEN           13          //设备ID长度为12，最后以为\0预留结束符
#define MAX_LT_CNT                  1024        //LT最大的数量

#define MAX_IPC_BUF                 1024       //共享内存的最大数据
#define MAX_LT_PORT_CNT             15         //LT下最大的端口数
#define MAX_SSID_LENGTH             32         //SSID length

#define SD100_EMG_TRIG_TIMES        1
#define SPEAKER_EMG_TRIG_TIMES      3

/*********************************************************
 *                         设备类型的定义
 * *******************************************************/
#define LT_FIRE_DEVICE_NORMAL	 0x20         //普通消防lt设备,主要用在界面通信时LT类型区分。
#define DEVICE_LT                0x00         //ＬＴ設備
#define DEVICE_CONFIG            0x01         //配置板設備
#define DEVICE_SD100             0x02         //烟雾传感器
#define DEVICE_SPEAKER           0x03         //喇叭檢測傳感器


/*********************************************************
 *                    端口号的定义
 * *******************************************************/
#define TCP_LOCAL_SVR_PORT       6010	      //TCP本地服务器端口号
#define TCP_REMOTE_SVR_PORT      6000	      //服务器CC的通信的TCP端口

/***************************************************************
 *                      系统控制命令的定义
****************************************************************/

#define CMD_START_ALARM               0X10	//开始报警
#define CMD_CANCEL_ALARM              0X11	//取消报警
#define CMD_LOW_POWER_ALARM           0x14  //低压报警
#define CMD_CANCEL_LOW_POWER_ALARM    0x15  //取消低压报警

/***************共享内存的使用*****************/
//#define CMD_GUI_EMG                      0x61
//#define CMD_GUI_CANCEL_EMG               0x62
#define CMD_GUI_DEVICE_OFF               0x62
#define CMD_GUI_DEVICE_ON                0x63

/***************************************************************
 *                                  通信协议包的定义
****************************************************************/

/******************网络数据包包头：4字节**************************/
#pragma pack(1)
typedef struct
{
    unsigned short Flag;                    //通过flag判断包的类型
    unsigned short PacketSize;              //包体信息的大小
    char DeviceId[MAX_DEVICE_ID_LEN];       //设备LTid号
}NETPACKET_MC_HEADER;
#pragma pack()

#define NETPACKET_HEADER_DROP_FLAG           0x0101        //客户端掉线包标志（仅仅为本程序掉线封装使用）
#define NETPACKET_HEADER_DETECT_FLAG         0x1a1a        //CC探测包的标志
#define NETPACKET_HEADER_REGISTER_FLAG       0x2b2b        //注册包的标志
#define NETPACKET_HEADER_COMMAND_FLAG        0x3c3c        //命令包的标志
#define NETPACKET_HEADER_LT_DETECT_FLAG      0x4d4d        //LT探测包

#define REGISTER_ONLY 0   //设备注册   /  the first time MC login CC, CC not select the database
#define REGISTER_ADD  1   //设备新增  /  not the first MC login CC, CC select the database
#define REGISTER_DROP 2   //设备掉线

/*************TCP 终端注册包(说明：此包是设备的注册信息包，有上线和掉线两种模式***************/
//STM32设备的注册信息附加在此包后面，收到数据包后根据此包的后两个字段去取注册信息即可)
#pragma pack(1)
typedef struct
{
    unsigned char  RegisterMode;                                        //0注册 1新增，2掉线
    unsigned char  LtType;                                              //lt的类型
    char LTSSID[MAX_SSID_LENGTH];                                       //LT的SSID
    unsigned char RegisterType;                                         //lt下注册的设备类型
    unsigned char RegisterNum;                                          //注册的lt下设备的数量
}TCP_PACK_REGISTER;
#pragma pack()

//烟雾传感器注册包
#pragma pack(1)
typedef struct
{
    unsigned char DeviceNO;                    //设备编号
    char DeviceID[MAX_DEVICE_ID_LEN];          //设备ID(?????????????)
    char BelongSSID[32];                       //所属SSID
}DEVICE_SD_INFO;
#pragma pack()

/**********************************TCP 控制命令包*********************************/
#pragma pack(1)
typedef struct
{
    unsigned char Cmd;                         //控制命令
    unsigned char DeviceType;                  //设备类型
    unsigned char CmdType;                     //命令的类型 1为分组任务，0单一   （与界面协议不同）
}TCP_PACK_CMD;
#pragma pack()


//终端下设备的报警
#pragma pack(1)
typedef struct
{
    unsigned char  LtPort;                     //设备挂在lt下所属的端口（若lt报警则为0）
    unsigned char DeviceType;                  //该端口下的设备类型
}TCP_TER_EMG_INFO;
#pragma pack()
/****************************************************************************************
 * 1、LT心跳包： NETPACKET_HEADER Flag = 0x4d4d DeviceId = LT ISN
2、LT上线包：NETPACKET_HEADER+TCP_PACK_REGISTER
        Flag = 0x2b2b(注册包)   DeviceId = LT ISN

        RegisterMode = 0 (REGISTERONLY 设备注册)
        LtType = 0x20 (LT_FIRE_DEVICE_NORMAL 普通消防Lt，因为以前有服务Lt)
        LTSSID = LT Wifi_Name(最长不超过32)
        RegisterType = 0x00(DEVCIE_LT , 刘工由于433协议，新定义的设备类型)
        RegisterNum = 1 (SD设备分多次注册，每次之注册一个)
3、SD100上线包：NETPACKET_HEADER+TCP_PACK_REGISTER+DEVICE_SD_INFO:
        Flag = 0x2b2b   DeviceId = LT ISN

        RegisterMode = 1 (REGISTER_ADD 设备新增)
        LtType = 0x20 (LT_FIRE_DEVICE_NORMAL 固定 普通消防Lt，因为以前有服务Lt)
        LTSSID = LT Wifi_Name(最长不超过32)
        RegisterType = 0x02(DEVCIE_SD100 , 刘工由于433协议，新定义的设备类型)
        RegisterNum = 1 (SD设备分多次注册，每次之注册一个)

        DeviceNo = 1-15 （该Lt下属设备，所处编号。从1号开始）
        DeviceId/BelongSSID 为空，由于433协议不传。
4、SD100掉线包：同上 ：
            RegisterMode = 2 (REGISTER_DROP 设备掉线)
5、SD100报警包：NETPACKET_HEAFER+TCP_PACKCMD+TCP_TER_EMG_INFO
        Flag = 0x3c3c(命令包)   DeviceId = LT ISN

        Cmd = 0x10(开始报警)
        DeviceType = 0x00 (DEVICE_LT)

        LtPort = 1-15（该Lt下属设备，所处编号。从1号开始）
        DeviceType = 0x02(DEVCIE_SD100 , 刘工由于433协议，新定义的设备类型)
6、SD100取消报警包：同上
        Cmd = 0x11(取消报警)
7、SD100低压报警/取消低压报警：同上
        Cmd = 0x14(低压报警)/0x15(取消低压报警)
 *
*****************************************************************************************/

#endif // MC_PROTOCOL_H

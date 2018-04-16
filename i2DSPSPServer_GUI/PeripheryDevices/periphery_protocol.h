#ifndef PERIPHERY_PROTOCOL_H
#define PERIPHERY_PROTOCOL_H
/**************cmd****************
  0xA05F 主电源状态
  0xA659 自检
  0xA758 按键广播
  0xA857 按键停止广播
  0xA25D 广播控制器状态
*********************************/
#define CMD_MIC_KEEP_LIVE       0x0001        //客户端每隔2S发送心跳包 4秒没有收到心跳包，视为Mic掉线,服务器关闭客户端
#define CMD_POWER_STATE         0xA05F        //电源管理设备状态
#define CMD_DEV_SELF_TEST       0xA659        //设备自检
#define CMD_BTN_BROADCAST       0xA758        //按键广播
#define CMD_BTN_STOP_BROADCAST  0xA857        //按键停止广播
#define CMD_BTN_EMG             0xA956        //按键应急广播
#define CMD_BTN_CANCEL_EMG      0xAA55        //按键取消应急广播
#define CMD_MC_STATE            0xA25D        //控制器正常工作,10S同步工作状态，工作灯绿色   控制器不同步状态，非正常状态，工作灯黄色


#pragma pack(1)
typedef struct
{
    unsigned short cmd;
    unsigned char stateCode;              //电源故障类型   前四个Bit表示主电源  后四个Bit表示备用电源  01正常 02故障 04欠压
    //控制器状态  bit0 设备故障 bit1 紧急状态  bit2 广播状态  bit3 蜂鸣器(改位为1时，蜂鸣器响)
}PACK_POWER_MODEL;
#pragma pack()

#endif // PERIPHERY_PROTOCOL_H

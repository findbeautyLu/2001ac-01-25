#ifndef app_modbus_scheduler_H
#define app_modbus_scheduler_H
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define MAX_MODBUS_NUM           1
#define PROTOL_CHECK_CONFICT     0xD020  //报文冲突检测
#define PROTOL_SYN               0xD021  //数据同步
#define PROTOL_REGEDIT           0xD022  //报文冲突检测
#define PROTOL_VERSION           0x01  //协议版本
#define CONTROL_REGEDIT          0x01//注册
#define CONTROL_UPDATA           0x02//更新
//----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void app_modbus_scheduler_task(void);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#endif
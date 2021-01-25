#ifndef app_master_slave_test_H
#define app_master_slave_test_H
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define SET_POWER            0x01        //开关机
#define SET_MODE             0x02        //模式
#define SET_WINDSTATUS       0x04        //通风状态
#define SET_FANSPEED         0x08        //风速
#define SET_TEMP             0x10        //温度
#define SET_HUM              0x20        //湿度
//----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void app_master_slave_test_task(void);
uint8_t app_master_slave_pull_device_list_num(void);
bool app_master_slave_pull_device_list(uint8_t in_port,uint16_t* out_deviceType);
void app_master_slave_push_occpy(uint16_t in_occpyBit);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#endif
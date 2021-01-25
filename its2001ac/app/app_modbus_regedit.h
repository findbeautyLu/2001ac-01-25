#ifndef app_modbus_regedit_H
#define app_modbus_regedit_H
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void app_modbus_regedit_task(void);
uint16_t GetRandomDelayTime(void);
void app_master_slave_dispose_regedit_data(uint8_t in_solidNum,ms_link_comm_data_def *in_rev_data);
bool pull_local_id(uint8_t *in_deviceId);
uint8_t app_master_slave_pull_device_list_num(uint8_t in_solidNum);
uint16_t pull_device_type(uint8_t in_solidNum,uint8_t in_port);
uint8_t *pull_device_id(uint8_t in_solidNum,uint8_t in_port);
bool app_master_slave_pull_device_list(uint8_t in_solidNum,uint8_t in_port,uint16_t* out_deviceType);
bool pull_local_device_online(uint8_t in_solidNum);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#endif
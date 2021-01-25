#ifndef app_modbus_synchronize_H
#define app_modbus_synchronize_H
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define SET_POWER            0x01        //开关机
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void app_modbus_syn_task(void);
void app_master_slave_dispose_syn_data(uint8_t in_solidNum,ms_link_comm_data_def *in_rev_data);
void app_master_slave_push_occpy(uint8_t in_solidNum,uint16_t in_occpyBit);
void app_push_own_stamp(uint32_t  in_stamp);
void app_push_need_stamp(uint32_t  in_stamp);
void app_push_rtc_stamp(uint32_t  in_stamp);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#endif
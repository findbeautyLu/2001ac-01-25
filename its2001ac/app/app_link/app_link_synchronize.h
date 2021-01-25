#ifndef app_modbus_synchronize_H
#define app_modbus_synchronize_H
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define OCCUPY_PAD_POWER                     0X01
#define OCCUPY_PAD_SET_COLD_TEMP             0X02
#define OCCUPY_PAD_SET_HEAT_TEMP             0X04
#define OCCUPY_PAD_CHILD_LOCK                0X08
#define OCCUPY_PAD_SET_FAN                   0X10
#define OCCUPY_PAD_TWO_CNONTROL_MODE         0X20
#define OCCUPY_PAD_BIND_MESSAGE              0X40
#define OCCUPY_PAD_STATUS_MESSAGE            0X80
#define OCCUPY_PAD_MEASURE_MESSAGE           0X0100
#define OCCUPY_PAD_ADV_MESSAGE               0X0200
#define OCCUPY_PAD_SYSTEM_MESSAGE            0X0400
#define OCCUPY_PAD_SYSTEM_RTC                0X0800

#define OCCUPY_SYSTEM_POWER                             0X0001
#define OCCUPY_SYSTEM_RUN_MODE                          0X0002
#define OCCUPY_SYSTEM_WIND_SET_CLOD_TEMP                0X0004
#define OCCUPY_SYSTEM_WIND_SET_HEAT_TEMP                0X0008
#define OCCUPY_SYSTEM_WIND_SET_HUM                      0X0010
#define OCCUPY_SYSTEM_WIND_SET_SPEED                    0X0020
#define OCCUPY_SYSTEM_WATER_SET_CLOD_TEMP               0X0040
#define OCCUPY_SYSTEM_WATER_SET_HEAT_TEMP               0X0080
#define OCCUPY_SYSTEM_WATER_SET_LIVING_TEMP             0X0100
#define OCCUPY_SYSTEM_MIXWATER_SET_CLOD_TEMP            0X0200
#define OCCUPY_SYSTEM_MIXWATER_SET_HEAT_TEMP            0X0400
#define OCCUPY_SYSTEM_MIXWATER_AI_ENABLE                0X0800
#define OCCUPY_SYSTEM_MIXWATER_LOW_TEMP_PROTECT         0X1000
#define OCCUPY_SYSTEM_MIXWATER_AI_DEST_TEMP             0X2000
#define OCCUPY_SYSTEM_LIS_NEW_AIR_PWM                   0X4000
#define OCCUPY_SYSTEM_LIS_BACK_AIR_PWM                  0X8000
#define OCCUPY_SYSTEM_LIS_OUTDOOR_WEATHER               0X00010000
#define OCCUPY_SYSTEM_LIS_BEFORE_FU_WEATHER             0X00020000
#define OCCUPY_SYSTEM_LIS_AFTER_FU_WEATHER              0X00040000
#define OCCUPY_SYSTEM_LIS_BACK_AIR_WEATHER              0X00080000
#define OCCUPY_SYSTEM_LIS_EXHAST_AIR_WEATHER            0X00100000
#define OCCUPY_SYSTEM_LIS_DEAD_ZONE                     0X00200000
#define OCCUPY_SYSTEM_LIS_OUT_VALVE                     0X00400000
#define OCCUPY_SYSTEM_LIS_OUT_STATUS                    0X00800000
#define OCCUPY_SYSTEM_LIS_MIXWATER_OUT                  0X01000000
#define OCCUPY_SYSTEM_LIS_MAIN_MACHINE_TEMP             0X02000000
#define OCCUPY_SYSTEM_LIS_MIXWATER_TEMP                 0X04000000
#define OCCUPY_SYSTEM_LIS_RTC                           0X08000000
#define OCCUPY_SYSTEM_LIS_485_PAREMETER                 0X10000000
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//interface list  4
1:同步任务:调度器使用         
void app_link_syn_task(void);  

2:数据压入接口:调度器使用
void app_link_syn_push_receive_data(uint8_t in_solidNum,bgk_comm_buff_def *in_rev_data);

3:对外同步接口
void app_link_syn_push_outside_updata_word(uint8_t in_solidNum，uint32_t in_updataWord);

--------------------------------E N D-----------------------------------------*/
//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++*/
//序号: 1  
//名称: app_link_syn_task
//功能: 注册列表维护:调度器使用
//入口: 无                           
//出口: 无
void app_link_syn_task(void);
//------------------------------E N D-------------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++*/
//序号: 2
//名称: app_link_syn_push_receive_data
//功能: 数据压入接口:调度器使用
//入口: in_solidNum 对应实例   in_rev_data  接收到的数据                       
//出口: 无
void app_link_syn_push_receive_data(uint8_t in_solidNum,bgk_comm_buff_def *in_rev_data);
//------------------------------E N D-------------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++*/
//序号: 3
//名称: app_link_syn_push_outside_updata_word
//功能:对外同步接口
//入口: in_solidNum 对应实例     in_updataWord:数据点更新字                          
//出口: 
void app_link_syn_push_outside_updata_word(uint8_t in_solidNum,uint32_t in_updataWord);
//------------------------------E N D-------------------------------------------




//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#endif
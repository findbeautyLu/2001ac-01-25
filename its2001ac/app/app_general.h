#ifndef app_general_H
#define app_general_H
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include ".\app_cfg.h"

#define  MAX_DATA_POINT_LEN             29//DP最大长度

enum
{
    Power_ON                =0x0001,  //开机
    Power_OFF               =0x0002,  //关机
    ChildrenLock_ON         =0x0004,
    ChildrenLock_OFF        =0x0008,
    FillClear               =0x0010,  //
    //Modbus_NotFreezeON_OFF         =0x0020,
    PowerONSlt_ON           =0x0040,
    PowerONSlt_OFF          =0x0080,
    PrgTimeRun_ON           =0x0100,
    PrgTimeRun_OFF          =0x0200,
    OnceAM4100PM25          =0x0400, //测量一次AM4100的PM2.5值
};

void app_general_task(void);
/*时间戳*/
void app_general_push_system_dp_stamp(uint16_t in_index,uint32_t in_stamp);
uint32_t app_general_pull_system_dp_stamp(uint16_t in_index);
/*故障字*/
uint16_t app_general_pull_master_error_word(void);
void app_general_push_master_error_word(uint16_t in_error);
/*开关机*/
void app_general_push_power_status(bool in_power_status);
bool  app_general_pull_power_status(void);
/*模式*/
void app_general_push_aircod_mode(AirRunMode_Def in_mode);
AirRunMode_Def app_general_pull_aircod_mode(void);
/*设定湿度*/
void app_general_push_aircod_humidity(int16_t in_set_humidity);
int16_t app_general_pull_aircod_humidity(void);
/*设定风速*/
void app_general_push_aircod_fanSpeed(NewAirLevelSet_Def in_speed);
NewAirLevelSet_Def app_general_pull_aircod_fanSpeed(void);
/*制冷设定温度、制热设定温度*/
void app_general_push_aircod_cold_temp(int16_t in_set_temp);
void app_general_push_aircod_heat_temp(int16_t in_set_temp);
int16_t app_general_pull_aircod_cold_temp(void);
int16_t app_general_pull_aircod_heat_temp(void);
/*新风PWM*/
uint8_t app_general_pull_new_air_pwm_low(void);
uint8_t app_general_pull_new_air_pwm_mid(void);
uint8_t app_general_pull_new_air_pwm_high(void);
void app_general_push_new_air_pwm_low(uint8_t in_pwm);
void app_general_push_new_air_pwm_mid(uint8_t in_pwm);
void app_general_push_new_air_pwm_high(uint8_t in_pwm);

/*回风风PWM*/
uint8_t app_general_pull_back_air_pwm_low(void);
uint8_t app_general_pull_back_air_pwm_mid(void);
uint8_t app_general_pull_back_air_pwm_high(void);
void app_general_push_back_air_pwm_low(uint8_t in_pwm);
void app_general_push_back_air_pwm_mid(uint8_t in_pwm);
void app_general_push_back_air_pwm_high(uint8_t in_pwm);

/*主机制冷制热生活热水设定水温*/
void app_general_push_set_cold_water_temp(int16_t in_set_temp);
void app_general_push_set_heat_water_temp(int16_t in_set_temp);
void app_general_push_set_living_water_temp(int16_t in_set_temp);
int16_t app_general_pull_set_cold_water_temp(void);
int16_t app_general_pull_set_heat_water_temp(void);
int16_t app_general_pull_set_living_water_temp(void);
//户外进风/氟盘前/氟盘后/回风/排风温湿度
void app_general_push_outdoor_temp(int16_t in_set_temp);
int16_t app_general_pull_outdoor_temp(void);
void app_general_push_outdoor_hum(int16_t in_set_hum);
int16_t app_general_pull_outdoor_hum(void);
void app_general_push_before_fu_temp(int16_t in_set_temp);
int16_t app_general_pull_before_fu_temp(void);
void app_general_push_before_fu_hum(int16_t in_set_hum);
int16_t app_general_pull_before_fu_hum(void);
void app_general_push_after_fu_temp(int16_t in_set_temp);
int16_t app_general_pull_after_fu_temp(void);
void app_general_push_after_fu_hum(int16_t in_set_hum);
int16_t app_general_pull_after_fu_hum(void);
void app_general_push_backair_temp(int16_t in_set_temp);
int16_t app_general_pull_backair_temp(void);
void app_general_push_backair_hum(int16_t in_set_hum);
int16_t app_general_pull_backair_hum(void);
void app_general_push_exhastair_temp(int16_t in_set_temp);
int16_t app_general_pull_exhastair_temp(void);
void app_general_push_exhastair_hum(int16_t in_set_hum);
int16_t app_general_pull_exhastair_hum(void);
/*回风co2 pm25*/
void app_general_push_backair_co2(int16_t in_set_co2);
int16_t app_general_pull_backair_co2(void);
void app_general_push_backair_pm25(int16_t in_set_pm25);
int16_t app_general_pull_backair_pm25(void);
/*设定温度带宽*/
void app_general_push_aircod_setTemp_deadZone(int16_t in_deadZone);
int16_t app_general_pull_aircod_setTemp_deadZone(void);
/*设定湿度带宽*/
void app_general_push_aircod_setHum_deadZone(int16_t in_deadZone);
int16_t app_general_pull_aircod_setHum_deadZone(void);
/*新风风机转速*/
uint16_t   app_general_pull_aircod_newair_speed(void);
void app_general_push_aircod_newair_speed(uint16_t in_speed);
/*回风风机转速*/
uint16_t app_general_pull_aircod_backair_speed(void);
void app_general_push_aircod_backair_speed(uint16_t in_speed);
/*氟机水机频率*/
void app_general_push_fu_machine_fre(uint16_t in_fre);
uint16_t app_general_pull_fu_machine_fre(void);
void app_general_push_water_machine_fre(uint16_t in_fre);
uint16_t app_general_pull_water_machine_fre(void);

/*主机供水温度、生活热水温度、户外温度*/
int16_t app_general_pull_master_supply_temp(void);
void app_general_push_master_supply_temp(int16_t in_temp);
int16_t app_general_pull_master_living_temp(void);
void app_general_push_master_living_temp(int16_t in_temp);
int16_t app_general_pull_master_outdoor_temp(void);
void app_general_push_master_outdoor_temp(int16_t in_temp);

/*湿度温度co2pm25修正系数  户外 氟盘前 氟盘后 回风 排风*/
int16_t app_general_pull_adjust_outdoor_hum(void);
int16_t app_general_pull_adjust_outdoor_temp(void);
int16_t app_general_pull_adjust_outdoor_co2(void);
int16_t app_general_pull_adjust_outdoor_pm25(void);
int16_t app_general_pull_adjust_beforfu_hum(void);
int16_t app_general_pull_adjust_beforfu_temp(void);
int16_t app_general_pull_adjust_beforfu_co2(void);
int16_t app_general_pull_adjust_beforfu_pm25(void);
int16_t app_general_pull_adjust_afterfu_hum(void);
int16_t app_general_pull_adjust_afterfu_temp(void);
int16_t app_general_pull_adjust_afterfu_co2(void);
int16_t app_general_pull_adjust_afterfu_pm25(void);
int16_t app_general_pull_adjust_backair_hum(void);
int16_t app_general_pull_adjust_backair_temp(void);
int16_t app_general_pull_adjust_backair_co2(void);
int16_t app_general_pull_adjust_backair_pm25(void);
int16_t app_general_pull_adjust_exhastair_hum(void);
int16_t app_general_pull_adjust_exhastair_temp(void);
int16_t app_general_pull_adjust_exhastair_co2(void);
int16_t app_general_pull_adjust_exhastair_pm25(void);
void app_general_push_adjust_outdoor_hum(int16_t in_hum);
void app_general_push_adjust_outdoor_temp(int16_t in_temp);
void app_general_push_adjust_outdoor_co2(int16_t in_co2);
void app_general_push_adjust_outdoor_pm25(int16_t in_pm25);
void app_general_push_adjust_beforfu_hum(int16_t in_hum);
void app_general_push_adjust_beforfu_temp(int16_t in_temp);
void app_general_push_adjust_beforfu_co2(int16_t in_co2);
void app_general_push_adjust_beforfu_pm25(int16_t in_pm25);
void app_general_push_adjust_afterfu_hum(int16_t in_hum);
void app_general_push_adjust_afterfu_temp(int16_t in_temp);
void app_general_push_adjust_afterfu_co2(int16_t in_co2);
void app_general_push_adjust_afterfu_pm25(int16_t in_pm25);
void app_general_push_adjust_backair_hum(int16_t in_hum);
void app_general_push_adjust_backair_temp(int16_t in_temp);
void app_general_push_adjust_backair_co2(int16_t in_co2);
void app_general_push_adjust_backair_pm25(int16_t in_pm25);
void app_general_push_adjust_exhastair_hum(int16_t in_hum);
void app_general_push_adjust_exhastair_temp(int16_t in_temp);
void app_general_push_adjust_exhastair_co2(int16_t in_co2);
void app_general_push_adjust_exhastair_pm25(int16_t in_pm25);
/*环控能需*/
bool app_general_pull_anergy_need(void);
void app_general_push_anergy_need(bool in_status);
/*传感器错误*/
uint16_t app_general_pull_ntc_error_word(void);
void app_general_push_ntc_error_word(uint16_t in_error);

/*混水制冷/制热设定温度 */
void app_general_push_set_mixwater_cold_temp(int16_t in_set_temp);
void app_general_push_set_mixwater_heat_temp(int16_t in_set_temp);
int16_t app_general_pull_set_mixwater_cold_temp(void);
int16_t app_general_pull_set_mixwater_heat_temp(void);

/*输配进水温度*/
int16_t app_general_pull_in_water_measure_temp(void);
void app_general_push_in_water_measure_temp(int16_t in_temp);
/*输配回水温度*/
int16_t app_general_pull_back_water_measure_temp(void);
void app_general_push_back_water_measure_temp(int16_t in_temp);
/*输配混水温度*/
int16_t app_general_pull_mix_water_measure_temp(void);
void app_general_push_mix_water_measure_temp(int16_t in_temp);
/*输配水泵状态*/
bool app_general_pull_pump_output(void);
void app_general_push_pump_output(bool in_status);
/*输配三通阀门输出状态*/
bool app_general_pull_three_vavle_output(void);
void app_general_push_three_vavle_output(bool in_status);
/*输配混水AI目标温度 */
int16_t app_general_pull_ai_dest_temp(void);
void app_general_push_ai_dest_temp(int16_t in_temp);
/*输配混水AI使能*/
bool app_general_pull_ai_enable_status(void);
void app_general_push_ai_enable_status(bool in_status);
/*输配防冻保护*/
uint8_t app_general_pull_mix_freeze_protect(void);
void app_general_push_mix_freeze_protect(uint8_t  in_stemp);
uint16_t GetRandomDelayTime(void);

/*除湿输出*/
bool  app_general_pull_humidity_output(void);
/*综合传感器故障*/
bool pull_integrated_sensor_error(void);

/*外机接口映射 写*/
void app_general_push_cool_backwind_settemp(uint16_t in_settemp);
void app_general_push_hot_backwind_settemp(uint16_t in_settemp);
void app_general_push_cool_humidity_set(uint16_t in_setvalue);
void app_general_push_lifehotwater_settemp(uint16_t in_settemp);
void app_general_push_cool_outwater_settemp(uint16_t in_settemp);
void app_general_push_cool_backwater_settemp(uint16_t in_settemp);
void app_general_push_hot_outwater_settemp(uint16_t in_settemp);
void app_general_push_hot_backwater_settemp(uint16_t in_settemp);
/*外机接口映射 读*/
int16_t app_general_pull_coolbackwind_set_temp(void);
int16_t app_general_pull_hotbackwind_set_temp(void);
int16_t app_general_pull_cooloutwater_set_temp(void);
int16_t app_general_pull_coolbackwater_set_temp(void);
int16_t app_general_pull_hotoutwater_set_temp(void);
int16_t app_general_pull_hotbackwater_set_temp(void);











//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#endif

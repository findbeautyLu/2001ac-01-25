//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include ".\app_cfg.h"
#include "stdlib.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

static uint16_t protection_cpor_bits;//
#define feedbcak_wind_error             sdt_bit0        //转速故障
#define rs1001_offline                  sdt_bit1       //综合传感器故障

typedef struct
{//共用数据     
        uint16_t                   system_error_word;       //环控故障字
        int16_t                    outdoor_temp;
        int16_t                    outdoor_hum;
        int16_t                    before_fu_temp;
        int16_t                    before_fu_hum;
        int16_t                    after_fu_temp;
        int16_t                    after_fu_hum;
        int16_t                    back_air_temp;
        int16_t                    back_air_hum;
        int16_t                    exhast_air_temp;
        int16_t                    exhast_air_hum;
        int16_t                    back_air_co2;
        int16_t                    back_air_pm25;
        uint16_t                   new_air_speed;
        uint16_t                   back_air_speed;
        uint16_t                   water_machine_fre;
        uint16_t                   fu_machine_fre;
        int16_t                    master_supply_temp;
        int16_t                    master_living_temp;
        int16_t                    master_outdoor_temp;
        uint16_t                   mixColdSetTemp;           //混水制冷设定温度
        uint16_t                   mixHeatSetTemp;           //混水制热设定温度

		
        uint16_t                   ntc_error;              //传感器故障
        int16_t                    measure_mixTemp;         //混水测量温度
        int16_t                    backWaterMeasureTemp;    //回水测量温度
        int16_t                    inWaterMeasureTemp;    //进水测量温度
        int16_t                    dewPointTemp;            //露点温度   
        int16_t                    destTemp;                //目标温度  
        bool                       aiEnable;                 //ai使能
        bool                       energyNeed;               //环控能需
        uint8_t                    lowTempProtectConfig;//防冻温度，0为关闭
        bool                       output_pump:1;          //水泵输出
        bool                       output_threeWayValve:1;  //三通阀输出
        uint32_t                   dpStamp[MAX_DATA_POINT_LEN];
}SysPara_t;

SysPara_t s_sysPara = {
        .outdoor_temp = 220,
        .outdoor_hum = 50,
        .before_fu_temp= 220,
        .before_fu_hum= 50,
        .after_fu_temp= 220,
        .after_fu_hum= 50,
        .back_air_temp= 220,
        .back_air_hum= 50,
        .exhast_air_temp= 220,
        .exhast_air_hum= 50,
        .back_air_co2= 10,
        .back_air_pm25= 10,
        .master_living_temp = 500,
        .master_supply_temp = 400,
};

uint16_t GetRandomDelayTime(void)
{//20-3020ms的随机数
    uint16_t result = 0;
    result = (rand()&0x0190)*5+ 300;/* random factor 0-47*/      
    return  result;
}

/*时间戳*/
void app_general_push_system_dp_stamp(uint16_t in_index,uint32_t in_stamp)
{
    s_sysPara.dpStamp[(in_index-DP_ADDR_START)] = in_stamp;
}
uint32_t app_general_pull_system_dp_stamp(uint16_t in_index)
{
    return s_sysPara.dpStamp[(in_index-DP_ADDR_START)];
}

/*故障字*/
uint16_t app_general_pull_master_error_word(void)
{
    return s_sysPara.system_error_word;
}
void app_general_push_master_error_word(uint16_t in_error)
{
    s_sysPara.system_error_word =  in_error;
}
/*开关机*/
void app_general_push_power_status(bool in_power_status)
{  
    if(in_power_status != StoRunParameter.systemPower)
    {
        StoRunParameter.systemPower = in_power_status;
        app_link_syn_push_outside_updata_word(SYSTEM_MASTER,OCCUPY_SYSTEM_POWER);
        
        app_push_once_save_sto_parameter();
    }
	if(StoRunParameter.systemPower)
	{//开机
		APP_push_aricod_message(DRIVE_BOARD_POWER_ON_OFF,1);
	}
	else
	{//关机
		APP_push_aricod_message(DRIVE_BOARD_POWER_ON_OFF,0);
	}      
}

bool  app_general_pull_power_status(void)
{
    if(StoRunParameter.systemPower)
    {
        return true;
    }
    return false;
}
/*模式*/
void app_general_push_aircod_mode(AirRunMode_Def in_mode)
{
	if((in_mode ==AIR_MODE_COOL) || (in_mode ==AIR_MODE_HEAT)||
	   (in_mode ==AIR_MODE_HUMIDITY)||(in_mode ==AIR_MODE_FAN))
	{
		if(StoRunParameter.airRunmode != in_mode)
		{
			StoRunParameter.airRunmode = in_mode;
            app_link_syn_push_outside_updata_word(SYSTEM_MASTER,OCCUPY_SYSTEM_RUN_MODE);
			app_push_once_save_sto_parameter();           
		}
		APP_push_aricod_message(DRIVE_BOARD_SET_MODE,StoRunParameter.airRunmode);
    }
}
AirRunMode_Def app_general_pull_aircod_mode(void)
{
    return StoRunParameter.airRunmode;
}



void app_general_push_cool_backwind_settemp(uint16_t in_settemp)
{
	if(StoRunParameter.cool_backwind_settemp != in_settemp)
	{
		StoRunParameter.cool_backwind_settemp = in_settemp;
	}//5007制冷回风
	APP_push_aricod_message(DRIVE_BOARD_COOL_SET_TEMP,StoRunParameter.cool_backwind_settemp);
}
void app_general_push_hot_backwind_settemp(uint16_t in_settemp)
{
	if(StoRunParameter.hot_backwind_settemp != in_settemp)
	{
		StoRunParameter.hot_backwind_settemp = in_settemp;
	}//5008制热回风
	APP_push_aricod_message(DRIVE_BOARD_HOT_SET_TEMP,StoRunParameter.hot_backwind_settemp);
}
void app_general_push_cool_humidity_set(uint16_t in_setvalue)
{
	if(StoRunParameter.humidity_set != in_setvalue)
	{
		StoRunParameter.humidity_set = in_setvalue;
	}//5009相对湿度设置
	APP_push_aricod_message(DRIVE_BOARD_HUMIDITY_SET_TEMP,StoRunParameter.humidity_set);
}
void app_general_push_lifehotwater_settemp(uint16_t in_settemp)
{
	if(StoRunParameter.set_living_water_temp != in_settemp)
	{
		StoRunParameter.set_living_water_temp = in_settemp;
	}//5010相对湿度设置
	APP_push_aricod_message(DRIVE_BOARD_LIFE_HOTWATER_SET_TEMP,StoRunParameter.set_living_water_temp);
}
void app_general_push_cool_outwater_settemp(uint16_t in_settemp)
{
	if(StoRunParameter.cool_outwater_settemp != in_settemp)
	{
		StoRunParameter.cool_outwater_settemp = in_settemp;
	}//5007制冷回风
	APP_push_aricod_message(DRIVE_BOARD_COOL_OUTWATER_SETTEMP,StoRunParameter.cool_outwater_settemp);
}
void app_general_push_cool_backwater_settemp(uint16_t in_settemp)
{
	if(StoRunParameter.cool_backwater_settemp != in_settemp)
	{
		StoRunParameter.cool_backwater_settemp = in_settemp;
	}//5025制冷回水
	APP_push_aricod_message(DRIVE_BOARD_COOL_BACKWATER_SETTEMP,StoRunParameter.cool_backwater_settemp);
}
void app_general_push_hot_outwater_settemp(uint16_t in_settemp)
{
	if(StoRunParameter.hot_outwater_settemp != in_settemp)
	{
		StoRunParameter.hot_outwater_settemp = in_settemp;
	}//5026制热回风
	APP_push_aricod_message(DRIVE_BOARD_HOT_OUTWATER_SETTEMP,StoRunParameter.hot_outwater_settemp);
}
void app_general_push_hot_backwater_settemp(uint16_t in_settemp)
{
	if(StoRunParameter.hot_backwater_settemp != in_settemp)
	{
		StoRunParameter.hot_backwater_settemp = in_settemp;
	}//5027制热回水
	APP_push_aricod_message(DRIVE_BOARD_HOT_BACKWATER_SETTEMP,StoRunParameter.hot_backwater_settemp);
}

void app_general_push_two_value_state(uint16_t in_state)
{

	APP_push_aricod_message(DRIVE_BOARD_TWO_WAY_VALUE_STATE,in_state);
}

/*设定湿度*/
void app_general_push_aircod_humidity(int16_t in_set_humidity)
{
	if((in_set_humidity > 0) && (in_set_humidity<=100))
	{
		if(StoRunParameter.humidity_set != in_set_humidity)
		{
			StoRunParameter.humidity_set = in_set_humidity;
            app_link_syn_push_outside_updata_word(SYSTEM_MASTER,OCCUPY_SYSTEM_WIND_SET_HUM);
			app_push_once_save_sto_parameter();
		}
	}
}
int16_t app_general_pull_aircod_humidity(void)
{
    return StoRunParameter.humidity_set;
}

/*设定风速*/
void app_general_push_aircod_fanSpeed(NewAirLevelSet_Def in_speed)
{
    if((in_speed ==LowFanSet)||
	   (in_speed ==MiddleFanSet)||(in_speed ==HighFanSet))
	{
		if(StoRunParameter.NewAirLevelSet != in_speed)
		{
			StoRunParameter.NewAirLevelSet = in_speed;
            app_link_syn_push_outside_updata_word(SYSTEM_MASTER,OCCUPY_SYSTEM_WIND_SET_SPEED);
			app_push_once_save_sto_parameter();
		}
    }
}
NewAirLevelSet_Def app_general_pull_aircod_fanSpeed(void)
{
    return StoRunParameter.NewAirLevelSet;
}
/*通风制冷设定温度、制热设定温度*/
void app_general_push_aircod_cold_temp(int16_t in_set_temp)
{
	if((in_set_temp >= 50) && (in_set_temp <= 350)&&(in_set_temp%5==0))
	{
		if(StoRunParameter.coldTemp_set != in_set_temp)
		{
			StoRunParameter.coldTemp_set = in_set_temp;
            app_link_syn_push_outside_updata_word(SYSTEM_MASTER,OCCUPY_SYSTEM_WIND_SET_CLOD_TEMP);
			app_push_once_save_sto_parameter();
		}
	}
}

void app_general_push_aircod_heat_temp(int16_t in_set_temp)
{
	if((in_set_temp >= 50) && (in_set_temp <= 350)&&(in_set_temp%5==0))
	{
		if(StoRunParameter.hotTemp_set != in_set_temp)
		{
			StoRunParameter.hotTemp_set = in_set_temp;   
            app_link_syn_push_outside_updata_word(SYSTEM_MASTER,OCCUPY_SYSTEM_WIND_SET_HEAT_TEMP);           
			app_push_once_save_sto_parameter();
		}
        
	}
}

int16_t app_general_pull_aircod_set_temp(void)
{
    int16_t  runSetTemp = 0;
	if(StoRunParameter.airRunmode == AIR_MODE_COOL)
	{
		runSetTemp = StoRunParameter.coldTemp_set;
	}
	else if(StoRunParameter.airRunmode == AIR_MODE_HEAT)
	{
		runSetTemp = StoRunParameter.hotTemp_set;
	}
	else
	{
		runSetTemp = StoRunParameter.coldTemp_set;
	}
	return runSetTemp;
}

int16_t app_general_pull_aircod_cold_temp(void)
{
    return StoRunParameter.coldTemp_set;
}
int16_t app_general_pull_aircod_heat_temp(void)
{
    return StoRunParameter.hotTemp_set;
}
/*新风PWM*/
uint8_t app_general_pull_new_air_pwm_low(void)
{
    return StoRunParameter.newAirLowPwm;
}
uint8_t app_general_pull_new_air_pwm_mid(void)
{
    return StoRunParameter.newAirMidPwm;
}
uint8_t app_general_pull_new_air_pwm_high(void)
{
    return StoRunParameter.newAirHighPwm;
}
void app_general_push_new_air_pwm_low(uint8_t in_pwm)
{
    if(in_pwm<=100)
    {
        StoRunParameter.newAirLowPwm = in_pwm;
        app_link_syn_push_outside_updata_word(SYSTEM_MASTER,OCCUPY_SYSTEM_LIS_NEW_AIR_PWM);
    }
}
void app_general_push_new_air_pwm_mid(uint8_t in_pwm)
{
    if(in_pwm<=100)
    {
        StoRunParameter.newAirMidPwm = in_pwm;
        app_link_syn_push_outside_updata_word(SYSTEM_MASTER,OCCUPY_SYSTEM_LIS_NEW_AIR_PWM);
    }
}
void app_general_push_new_air_pwm_high(uint8_t in_pwm)
{
    if(in_pwm<=100)
    {
        StoRunParameter.newAirHighPwm = in_pwm;
        app_link_syn_push_outside_updata_word(SYSTEM_MASTER,OCCUPY_SYSTEM_LIS_NEW_AIR_PWM);
    }
}

/*回风风PWM*/
uint8_t app_general_pull_back_air_pwm_low(void)
{
    return StoRunParameter.backAirLowPwm;
}
uint8_t app_general_pull_back_air_pwm_mid(void)
{
    return StoRunParameter.newAirMidPwm;
}
uint8_t app_general_pull_back_air_pwm_high(void)
{
    return StoRunParameter.backAirHighPwm;
}
void app_general_push_back_air_pwm_low(uint8_t in_pwm)
{
    if(in_pwm<=100)
    {
        StoRunParameter.backAirLowPwm = in_pwm;
        app_link_syn_push_outside_updata_word(SYSTEM_MASTER,OCCUPY_SYSTEM_LIS_BACK_AIR_PWM);
    }
}
void app_general_push_back_air_pwm_mid(uint8_t in_pwm)
{
    if(in_pwm<=100)
    {
        StoRunParameter.backAirMidPwm = in_pwm;
        app_link_syn_push_outside_updata_word(SYSTEM_MASTER,OCCUPY_SYSTEM_LIS_BACK_AIR_PWM);
    }
}
void app_general_push_back_air_pwm_high(uint8_t in_pwm)
{
    if(in_pwm<=100)
    {
        StoRunParameter.backAirHighPwm = in_pwm;
        app_link_syn_push_outside_updata_word(SYSTEM_MASTER,OCCUPY_SYSTEM_LIS_BACK_AIR_PWM);
    }
}

/*主机制冷制热生活热水设定水温*/
void app_general_push_set_cold_water_temp(int16_t in_set_temp)
{
//	if((in_set_temp >= 50) && (in_set_temp <= 350)&&(in_set_temp%5==0))
//	{
		if(StoRunParameter.set_cold_water_temp != in_set_temp)
		{
            app_link_syn_push_outside_updata_word(SYSTEM_MASTER,OCCUPY_SYSTEM_WATER_SET_CLOD_TEMP);
            APP_push_aricod_message(DRIVE_BOARD_COOL_BACKWATER_SETTEMP,200);
			StoRunParameter.set_cold_water_temp = in_set_temp;
		}
//	}
}

void app_general_push_set_heat_water_temp(int16_t in_set_temp)
{
	//if((in_set_temp >= 50) && (in_set_temp <= 600)&&(in_set_temp%5==0))
//	{
		if(StoRunParameter.set_heat_water_temp != in_set_temp)
		{
			StoRunParameter.set_heat_water_temp = in_set_temp;   
            app_link_syn_push_outside_updata_word(SYSTEM_MASTER,OCCUPY_SYSTEM_WATER_SET_HEAT_TEMP);  
            APP_push_aricod_message(DRIVE_BOARD_HOT_BACKWATER_SETTEMP,500);      
		}        
	//}
}

void app_general_push_set_living_water_temp(int16_t in_set_temp)
{ 
	//if((in_set_temp >= 50) && (in_set_temp <= 350)&&(in_set_temp%5==0))
	//{
		if(StoRunParameter.set_living_water_temp != in_set_temp)
		{
			StoRunParameter.set_living_water_temp = in_set_temp;  
            app_link_syn_push_outside_updata_word(SYSTEM_MASTER,OCCUPY_SYSTEM_WATER_SET_LIVING_TEMP);         
		}        
	//}
}

int16_t app_general_pull_set_cold_water_temp(void)
{
    return StoRunParameter.set_cold_water_temp;
}
int16_t app_general_pull_set_heat_water_temp(void)
{
    return StoRunParameter.set_heat_water_temp;
}
int16_t app_general_pull_set_living_water_temp(void)
{
    return StoRunParameter.set_living_water_temp;
}

/*户外进风/氟盘前/氟盘后/回风/排风温湿度*/
void app_general_push_outdoor_temp(int16_t in_set_temp)
{
    s_sysPara.outdoor_temp = in_set_temp;
}
int16_t app_general_pull_outdoor_temp(void)
{
    return s_sysPara.outdoor_temp;
}
void app_general_push_outdoor_hum(int16_t in_set_hum)
{
    s_sysPara.outdoor_hum = in_set_hum;
}
int16_t app_general_pull_outdoor_hum(void)
{
    return s_sysPara.outdoor_hum;
}
void app_general_push_before_fu_temp(int16_t in_set_temp)
{
    s_sysPara.before_fu_temp = in_set_temp;
}
int16_t app_general_pull_before_fu_temp(void)
{
    return s_sysPara.before_fu_temp;
}
void app_general_push_before_fu_hum(int16_t in_set_hum)
{
    s_sysPara.before_fu_hum = in_set_hum;
}
int16_t app_general_pull_before_fu_hum(void)
{
    return s_sysPara.before_fu_hum;
}
void app_general_push_after_fu_temp(int16_t in_set_temp)
{
    s_sysPara.after_fu_temp = in_set_temp;
}
int16_t app_general_pull_after_fu_temp(void)
{
    return s_sysPara.after_fu_temp;
}
void app_general_push_after_fu_hum(int16_t in_set_hum)
{
    s_sysPara.after_fu_hum = in_set_hum;
}
int16_t app_general_pull_after_fu_hum(void)
{
    return s_sysPara.after_fu_hum;
}
void app_general_push_backair_temp(int16_t in_set_temp)
{
    s_sysPara.back_air_temp = in_set_temp;
}
int16_t app_general_pull_backair_temp(void)
{
    return s_sysPara.back_air_temp;
}
void app_general_push_backair_hum(int16_t in_set_hum)
{
    s_sysPara.back_air_hum = in_set_hum;
}
int16_t app_general_pull_backair_hum(void)
{
    return s_sysPara.back_air_hum;
}
void app_general_push_exhastair_temp(int16_t in_set_temp)
{
    s_sysPara.exhast_air_temp = in_set_temp;
}
int16_t app_general_pull_exhastair_temp(void)
{
    return s_sysPara.exhast_air_temp;
}
void app_general_push_exhastair_hum(int16_t in_set_hum)
{
    s_sysPara.exhast_air_hum = in_set_hum;
}
int16_t app_general_pull_exhastair_hum(void)
{
    return s_sysPara.exhast_air_hum;
}
/*回风co2 pm25*/
void app_general_push_backair_co2(int16_t in_set_co2)
{
    s_sysPara.back_air_co2 = in_set_co2;
}
int16_t app_general_pull_backair_co2(void)
{
    return s_sysPara.back_air_co2;
}
void app_general_push_backair_pm25(int16_t in_set_pm25)
{
    s_sysPara.back_air_pm25 = in_set_pm25;
}
int16_t app_general_pull_backair_pm25(void)
{
    return s_sysPara.back_air_pm25;
}

/*设定温度带宽*/
void app_general_push_aircod_setTemp_deadZone(int16_t in_deadZone)
{
	if((in_deadZone >= 5) && (in_deadZone <= 100)&&(in_deadZone%5==0))
	{		
		if(StoRunParameter.temp_DeadZone != in_deadZone)
		{
			StoRunParameter.temp_DeadZone = in_deadZone;
            app_link_syn_push_outside_updata_word(SYSTEM_MASTER,OCCUPY_SYSTEM_LIS_DEAD_ZONE);
			app_push_once_save_sto_parameter();
		}
	}
}
int16_t app_general_pull_aircod_setTemp_deadZone(void)
{
    return StoRunParameter.temp_DeadZone;
}
/*设定湿度带宽*/
void app_general_push_aircod_setHum_deadZone(int16_t in_deadZone)
{
	if((in_deadZone <= 20) && (in_deadZone > 0))
	{		
		if(StoRunParameter.humidity_DeadZone != in_deadZone)
		{
			StoRunParameter.humidity_DeadZone = in_deadZone;
            app_link_syn_push_outside_updata_word(SYSTEM_MASTER,OCCUPY_SYSTEM_LIS_DEAD_ZONE);
			app_push_once_save_sto_parameter();
		}
	}
}
int16_t app_general_pull_aircod_setHum_deadZone(void)
{
    return StoRunParameter.humidity_DeadZone;
}

/*新风风机转速*/
uint16_t app_general_pull_aircod_newair_speed(void)
{
    return s_sysPara.new_air_speed;
}
void app_general_push_aircod_newair_speed(uint16_t in_speed)
{
    s_sysPara.new_air_speed = in_speed;
}
/*回风风机转速*/
uint16_t app_general_pull_aircod_backair_speed(void)
{	
	return s_sysPara.back_air_speed;
}
void app_general_push_aircod_backair_speed(uint16_t in_speed)
{
    s_sysPara.back_air_speed = in_speed;
}
/*氟机水机频率*/
void app_general_push_fu_machine_fre(uint16_t in_fre)
{
    s_sysPara.fu_machine_fre = in_fre;
}
uint16_t app_general_pull_fu_machine_fre(void)
{
    return s_sysPara.fu_machine_fre;
}
void app_general_push_water_machine_fre(uint16_t in_fre)
{
    s_sysPara.water_machine_fre = in_fre;
}
uint16_t app_general_pull_water_machine_fre(void)
{
   return s_sysPara.water_machine_fre; 
}
//主机供水温度、生活热水温度、户外温度
int16_t app_general_pull_master_supply_temp(void)
{
    return s_sysPara.master_supply_temp;
}
void app_general_push_master_supply_temp(int16_t in_temp)
{
    s_sysPara.master_supply_temp = in_temp;
}
int16_t app_general_pull_master_living_temp(void)
{
    return s_sysPara.master_living_temp;
}
void app_general_push_master_living_temp(int16_t in_temp)
{
    s_sysPara.master_living_temp = in_temp;
}
int16_t app_general_pull_master_outdoor_temp(void)
{
    return s_sysPara.master_outdoor_temp;
}
void app_general_push_master_outdoor_temp(int16_t in_temp)
{
    s_sysPara.master_outdoor_temp = in_temp;
}
/*湿度温度co2pm25修正系数  户外 氟盘前 氟盘后 回风 排风*/
int16_t app_general_pull_adjust_outdoor_hum(void)
{
    return StoRunParameter.adjust_outdoor_hum;
}
int16_t app_general_pull_adjust_outdoor_temp(void)
{
    return StoRunParameter.adjust_outdoor_temp;
}
int16_t app_general_pull_adjust_outdoor_co2(void)
{
    return StoRunParameter.adjust_outdoor_co2;
}

int16_t app_general_pull_adjust_outdoor_pm25(void)
{
    return StoRunParameter.adjust_outdoor_pm25;
}
int16_t app_general_pull_adjust_beforfu_hum(void)
{
    return StoRunParameter.adjust_befor_fu_hum;
}
int16_t app_general_pull_adjust_beforfu_temp(void)
{
    return StoRunParameter.adjust_befor_fu_temp;
}
int16_t app_general_pull_adjust_beforfu_co2(void)
{
    return StoRunParameter.adjust_befor_fu_co2;
}
int16_t app_general_pull_adjust_beforfu_pm25(void)
{
    return StoRunParameter.adjust_befor_fu_pm25;
}
int16_t app_general_pull_adjust_afterfu_hum(void)
{
    return StoRunParameter.adjust_after_fu_hum;
}
int16_t app_general_pull_adjust_afterfu_temp(void)
{
    return StoRunParameter.adjust_after_fu_temp;
}
int16_t app_general_pull_adjust_afterfu_co2(void)
{
    return StoRunParameter.adjust_after_fu_co2;
}
int16_t app_general_pull_adjust_afterfu_pm25(void)
{
    return StoRunParameter.adjust_after_fu_pm25;
}

int16_t app_general_pull_adjust_backair_hum(void)
{
    return StoRunParameter.adjust_back_air_hum;
}
int16_t app_general_pull_adjust_backair_temp(void)
{
    return StoRunParameter.adjust_back_air_temp;
}
int16_t app_general_pull_adjust_backair_co2(void)
{
    return StoRunParameter.adjust_back_air_co2;
}
int16_t app_general_pull_adjust_backair_pm25(void)
{
    return StoRunParameter.adjust_back_air_pm25;
}
int16_t app_general_pull_adjust_exhastair_hum(void)
{
    return StoRunParameter.adjust_exhast_air_hum;
}
int16_t app_general_pull_adjust_exhastair_temp(void)
{
    return StoRunParameter.adjust_exhast_air_temp;
}
int16_t app_general_pull_adjust_exhastair_co2(void)
{
    return StoRunParameter.adjust_exhast_air_co2;
}
int16_t app_general_pull_adjust_exhastair_pm25(void)
{
    return StoRunParameter.adjust_exhast_air_pm25;
}

void app_general_push_adjust_outdoor_hum(int16_t in_hum)
{
    StoRunParameter.adjust_outdoor_hum = in_hum;
}
void app_general_push_adjust_outdoor_temp(int16_t in_temp)
{
    StoRunParameter.adjust_outdoor_temp = in_temp;
}
void app_general_push_adjust_outdoor_co2(int16_t in_co2)
{
    StoRunParameter.adjust_outdoor_co2 = in_co2;
}
void app_general_push_adjust_outdoor_pm25(int16_t in_pm25)
{
    StoRunParameter.adjust_outdoor_pm25 = in_pm25;
}

void app_general_push_adjust_beforfu_hum(int16_t in_hum)
{
    StoRunParameter.adjust_befor_fu_hum = in_hum;
}
void app_general_push_adjust_beforfu_temp(int16_t in_temp)
{
    StoRunParameter.adjust_befor_fu_temp = in_temp;
}
void app_general_push_adjust_beforfu_co2(int16_t in_co2)
{
    StoRunParameter.adjust_befor_fu_co2 = in_co2;
}
void app_general_push_adjust_beforfu_pm25(int16_t in_pm25)
{
    StoRunParameter.adjust_befor_fu_pm25 = in_pm25;
}

void app_general_push_adjust_afterfu_hum(int16_t in_hum)
{
    StoRunParameter.adjust_after_fu_hum = in_hum;
}
void app_general_push_adjust_afterfu_temp(int16_t in_temp)
{
    StoRunParameter.adjust_after_fu_temp = in_temp;
}
void app_general_push_adjust_afterfu_co2(int16_t in_co2)
{
    StoRunParameter.adjust_after_fu_co2 = in_co2;
}
void app_general_push_adjust_afterfu_pm25(int16_t in_pm25)
{
    StoRunParameter.adjust_after_fu_pm25 = in_pm25;
}

void app_general_push_adjust_backair_hum(int16_t in_hum)
{
    StoRunParameter.adjust_back_air_hum = in_hum;
}
void app_general_push_adjust_backair_temp(int16_t in_temp)
{
    StoRunParameter.adjust_back_air_temp = in_temp;
}
void app_general_push_adjust_backair_co2(int16_t in_co2)
{
    StoRunParameter.adjust_back_air_co2 = in_co2;
}
void app_general_push_adjust_backair_pm25(int16_t in_pm25)
{
    StoRunParameter.adjust_back_air_pm25 = in_pm25;
}

void app_general_push_adjust_exhastair_hum(int16_t in_hum)
{
    StoRunParameter.adjust_exhast_air_hum = in_hum;
}
void app_general_push_adjust_exhastair_temp(int16_t in_temp)
{
    StoRunParameter.adjust_exhast_air_temp = in_temp;
}
void app_general_push_adjust_exhastair_co2(int16_t in_co2)
{
    StoRunParameter.adjust_exhast_air_co2 = in_co2;
}
void app_general_push_adjust_exhastair_pm25(int16_t in_pm25)
{
    StoRunParameter.adjust_exhast_air_pm25 = in_pm25;
}

/*环控能需*/
bool app_general_pull_anergy_need(void)
{
    return s_sysPara.energyNeed;
}
void app_general_push_anergy_need(bool in_status)
{
    s_sysPara.energyNeed = in_status;
}

/*传感器错误*/
uint16_t app_general_pull_ntc_error_word(void)
{
    return s_sysPara.ntc_error;
}
void app_general_push_ntc_error_word(uint16_t in_error)
{
    s_sysPara.ntc_error =  in_error;
}

/*绑定的阀门列表 (风盘)*/

/*绑定的阀门列表 (辐射)*/

/*阀门绑定故障*/

/*混水制冷/制热设定温度 */
void app_general_push_set_mixwater_cold_temp(int16_t in_set_temp)
{
//	if((in_set_temp >= 50) && (in_set_temp <= 350)&&(in_set_temp%5==0))
	//{
		if(s_sysPara.mixColdSetTemp != in_set_temp)
		{
			s_sysPara.mixColdSetTemp = in_set_temp;
            app_link_syn_push_outside_updata_word(SYSTEM_MASTER,OCCUPY_SYSTEM_MIXWATER_SET_CLOD_TEMP);
		}
	//}
}

void app_general_push_set_mixwater_heat_temp(int16_t in_set_temp)
{
	//if((in_set_temp >= 50) && (in_set_temp <= 350)&&(in_set_temp%5==0))
	//{
		if(s_sysPara.mixHeatSetTemp != in_set_temp)
		{
			s_sysPara.mixHeatSetTemp = in_set_temp; 
            app_link_syn_push_outside_updata_word(SYSTEM_MASTER,OCCUPY_SYSTEM_MIXWATER_SET_HEAT_TEMP);          
		}
        
	//}
}

int16_t app_general_pull_set_mixwater_cold_temp(void)
{
    return s_sysPara.mixColdSetTemp;
}

int16_t app_general_pull_set_mixwater_heat_temp(void)
{
    return s_sysPara.mixHeatSetTemp;
}

/*输配进水温度*/
int16_t app_general_pull_in_water_measure_temp(void)
{
    return s_sysPara.inWaterMeasureTemp;
}
void app_general_push_in_water_measure_temp(int16_t in_temp)
{
    s_sysPara.inWaterMeasureTemp = in_temp;
}
/*输配回水温度*/
int16_t app_general_pull_back_water_measure_temp(void)
{
    return s_sysPara.backWaterMeasureTemp;
}
void app_general_push_back_water_measure_temp(int16_t in_temp)
{
    s_sysPara.backWaterMeasureTemp = in_temp;
}
/*输配混水温度*/
int16_t app_general_pull_mix_water_measure_temp(void)
{
    return s_sysPara.measure_mixTemp;
}
void app_general_push_mix_water_measure_temp(int16_t in_temp)
{
    s_sysPara.measure_mixTemp = in_temp;
}

/*输配水泵状态*/
bool app_general_pull_pump_output(void)
{
    return s_sysPara.output_pump;
}
void app_general_push_pump_output(bool in_status)
{
    s_sysPara.output_pump = in_status;
}
/*输配三通阀门输出状态*/
bool app_general_pull_three_vavle_output(void)
{
    return s_sysPara.output_threeWayValve;
}
void app_general_push_three_vavle_output(bool in_status)
{
    s_sysPara.output_threeWayValve = in_status;
}
/*输配混水AI目标温度 */
int16_t app_general_pull_ai_dest_temp(void)
{
    return s_sysPara.destTemp;
}
void app_general_push_ai_dest_temp(int16_t in_temp)
{
    s_sysPara.destTemp = in_temp;
}
/*输配混水AI使能*/
bool app_general_pull_ai_enable_status(void)
{
    return s_sysPara.aiEnable;
}
void app_general_push_ai_enable_status(bool in_status)
{
    s_sysPara.aiEnable = in_status;
}
/*输配防冻保护*/
uint8_t app_general_pull_mix_freeze_protect(void)
{
    return s_sysPara.lowTempProtectConfig;
}
void app_general_push_mix_freeze_protect(uint8_t  in_set_temp)
{
    if((in_set_temp >= 50) && (in_set_temp <= 100)&&(in_set_temp%5==0))
    {
        if(in_set_temp != s_sysPara.lowTempProtectConfig)
        {         
            s_sysPara.lowTempProtectConfig = in_set_temp;
            app_link_syn_push_outside_updata_word(SYSTEM_MASTER,OCCUPY_SYSTEM_MIXWATER_LOW_TEMP_PROTECT);
        }
    }  
}
//
int16_t app_general_pull_coolbackwind_set_temp(void)
{
    return StoRunParameter.cool_backwind_settemp;
}

//
int16_t app_general_pull_hotbackwind_set_temp(void)
{
    return StoRunParameter.hot_backwind_settemp;
}

//
int16_t app_general_pull_cooloutwater_set_temp(void)
{
    return StoRunParameter.cool_outwater_settemp;
}

//
int16_t app_general_pull_coolbackwater_set_temp(void)
{
    return StoRunParameter.cool_backwater_settemp;
}

//
int16_t app_general_pull_hotoutwater_set_temp(void)
{
    return StoRunParameter.hot_outwater_settemp;
}

//
int16_t app_general_pull_hotbackwater_set_temp(void)
{
    return StoRunParameter.hot_backwater_settemp;
}

int16_t app_general_pull_humidity_setvalue(void)
{
    return StoRunParameter.humidity_set;
}
int16_t app_general_pull_life_hotwater_settemp(void)
{
    return StoRunParameter.set_living_water_temp;
}


bool   aircod_elect_hot_s = false;//电加热
bool   aircod_newbackwindValve_s = false;//新风回风风阀
bool   aircod_bypassValve_s = false;//旁通风阀
bool   aircod_inloopValve_s = false;//内循环风阀
/*新风风机输出*/
uint8_t   aircod_ec_newwind_s = 0;//新风风机
uint8_t app_general_pull_ecNewWind_output(void)
{
	return aircod_ec_newwind_s;
}
/*回风风机输出*/
uint8_t  aircod_ec_backwind_s = 0;//回风风机
uint8_t app_general_pull_ecBackWind_output(void)
{
	return aircod_ec_backwind_s;
}

/*新风联动风阀输出*/
bool aircod_newLinkwindValve_s = false;
bool app_general_pull_aircod_newAir_link_vavle(void)
{
    return aircod_newLinkwindValve_s;
}
/*回风联动风阀输出*/
bool aircod_backLinkwindValve_s = false;
bool app_general_pull_aircod_backAir_link_vavle(void)
{
    return aircod_backLinkwindValve_s;
}
/*预冷水盘阀输出*/
bool aircod_preColdValve_s = false;
bool app_general_pull_pre_cold_vavle(void)
{
    return aircod_preColdValve_s;
}
/*预热水盘阀输出*/
bool aircod_preHeatValve_s = false;
bool app_general_pull_pre_heat_vavle(void)
{
    return aircod_preHeatValve_s;
}
/*综合传感器故障*/
bool pull_integrated_sensor_error(void)
{
	if(protection_cpor_bits & rs1001_offline)
	{
		return true;
	}
	return false;
}
/*除湿输出*/
static bool air_humidity_s_output = false;
bool  app_general_pull_humidity_output(void)
{
	return air_humidity_s_output;
}
//-----------------------------------------------------------------------------
//除湿逻辑
//BN_TRUE 输出
//-----------------------------------------------------------------------------
#define MAX_HUM_VALVE   100
bool app_dehumidification_logic(int16_t _in_current_humitity,int16_t _in_set_huminty,int16_t _in_hum_deadZone)
{
    static bool lastOutputFlag = false;
    if((_in_current_humitity > MAX_HUM_VALVE) ||
       (_in_set_huminty > MAX_HUM_VALVE) ||
         (_in_hum_deadZone > MAX_HUM_VALVE))
    {
        return false;
    }
    if(_in_set_huminty <= _in_hum_deadZone)
    {
        return false;
    }
    if(_in_current_humitity > (_in_set_huminty + _in_hum_deadZone))
    {
        lastOutputFlag = true;
    }
    else if(_in_current_humitity < (_in_set_huminty - _in_hum_deadZone))
    {
        lastOutputFlag = false;
    }
    return lastOutputFlag;
}
/*采暖输出*/
static bool air_warm_s_output = false;
bool  app_general_pull_warm_output(void)
{
	return air_warm_s_output;
}
/*采暖逻辑*/
#define MAX_TEMP_VALVE   1000
bool app_temp_logic(int16_t _in_current_temp,int16_t _in_set_temp,int16_t _in_temp_deadZone)
{
    static bool lastOutputFlag = false;
    if((_in_current_temp > MAX_TEMP_VALVE) ||
       (_in_set_temp > MAX_TEMP_VALVE) ||
         (_in_temp_deadZone > MAX_TEMP_VALVE))
    {
        return false;
    }
    if(_in_set_temp <= _in_temp_deadZone)
    {
        return false;
    }
    if(_in_current_temp >= (_in_set_temp + _in_temp_deadZone))
    {
        lastOutputFlag = false;
    }
    else if(_in_current_temp <= (_in_set_temp - _in_temp_deadZone))
    {
        lastOutputFlag = true;
    }
    return lastOutputFlag;
}

uint8_t app_pull_newAir_pwm_value(NewAirLevelSet_Def in_fanSpeed)
{
    uint8_t pwmValue = 0;
    switch (in_fanSpeed)
    {
        case LowFanSet:
        {
            pwmValue = StoRunParameter.newAirLowPwm;
            break;
        }   
        case MiddleFanSet:
        {
            pwmValue = StoRunParameter.newAirMidPwm;
            break;
        }   
        case HighFanSet:
        {
            pwmValue = StoRunParameter.newAirHighPwm;
            break;
        }   
        default:
        {
            pwmValue = StoRunParameter.newAirLowPwm;
            break;
        }       
    }
    return pwmValue;
}
uint8_t app_pull_backAir_pwm_value(NewAirLevelSet_Def in_fanSpeed)
{
    uint8_t pwmValue = 0;
    switch (in_fanSpeed)
    {
        case LowFanSet:
        {
            pwmValue = StoRunParameter.backAirLowPwm;
            break;
        }   
        case MiddleFanSet:
        {
            pwmValue = StoRunParameter.backAirMidPwm;
            break;
        }   
        case HighFanSet:
        {
            pwmValue = StoRunParameter.backAirHighPwm;
            break;
        }   
        default:
        {
            pwmValue = StoRunParameter.backAirLowPwm;
            break;
        }       
    }
    return pwmValue;
}

bool app_pull_aircod_humidity_output(void)
{
    bool humidityFlag = false;
    if(pull_integrated_sensor_error())
    {//传感器错误
        humidityFlag = false;        
    }
    else
    {
        humidityFlag = app_dehumidification_logic((int16_t)Pull_RSS1001H_Humidity(3),StoRunParameter.humidity_set,StoRunParameter.humidity_DeadZone);
    }
    return humidityFlag;
}

bool app_pull_aircod_warm_output(void)
{
    bool warmFlag = false;
    if(pull_integrated_sensor_error())
    {//传感器错误
        warmFlag = false;        
    }
    else
    {
        warmFlag = app_temp_logic((int16_t)Pull_RSS1001H_Temperature(3),StoRunParameter.hotTemp_set,StoRunParameter.temp_DeadZone);
    }
    return warmFlag;
}

void app_pill_ec_fan_speed(void)
{  
    macro_createTimer(measure_delay,timerType_millisecond,0);
    pbc_timerClockRun_task(&measure_delay);
    if(pbc_pull_timerIsCompleted(&measure_delay))
    {
        pbc_reload_timerClock(&measure_delay,10000);
        mde_tachFan_Task();      
    }
}

void app_general_aircod_run_task(void)
{
	bool powerOff_now = false;
    app_pill_ec_fan_speed();
//-----------------------------------------------------------------------------
    if(StoRunParameter.systemPower)
    {
        powerOff_now=false;
    }
    else
    {
        powerOff_now=true;
    }    
//-----------------------------------------------------------------------------
    if(powerOff_now)
    {//关机
        if(StoRunParameter.mildewFlag)
        {//开启除霉
            aircod_newLinkwindValve_s = true;
            aircod_backLinkwindValve_s = true;
            air_humidity_s_output = app_pull_aircod_humidity_output();
            if(air_humidity_s_output)
            {

            }
            else
            {     
                aircod_ec_newwind_s = app_pull_newAir_pwm_value(StoRunParameter.NewAirLevelSet);
                aircod_ec_backwind_s = app_pull_backAir_pwm_value(StoRunParameter.NewAirLevelSet);
            }          
        }
        else
        {         
            aircod_ec_newwind_s = 0;//风机
            aircod_ec_backwind_s = 0;
            aircod_newLinkwindValve_s = false;//风阀
            aircod_backLinkwindValve_s = false; 
            air_humidity_s_output = false;
            air_warm_s_output = false;
            /*空调待机*/
            /*有防冻请求空调制热*/
        }  
        aircod_preColdValve_s = false;
        aircod_preHeatValve_s = false;   

    }
    else
    { //开机   
        aircod_newLinkwindValve_s = true;
        aircod_backLinkwindValve_s = true;   	
        switch(StoRunParameter.airRunmode)
        {					
            case AIR_MODE_FAN://新风模式
            {               
                aircod_preColdValve_s = false;
                aircod_preHeatValve_s = false;  
                air_humidity_s_output = false;
                air_warm_s_output = false;
                aircod_ec_newwind_s = app_pull_newAir_pwm_value(StoRunParameter.NewAirLevelSet);
                aircod_ec_backwind_s = app_pull_backAir_pwm_value(StoRunParameter.NewAirLevelSet);
                /*空调热水模式*/
				app_general_push_two_value_state(0);//开启
				
                break;
            }
            case AIR_MODE_COOL://制冷模式
            {             
                aircod_preHeatValve_s = false; 
                air_warm_s_output = false; 
                air_humidity_s_output = app_pull_aircod_humidity_output();
                if(air_humidity_s_output)
                {
                    aircod_preColdValve_s = true;
                    /*氟盘除湿+热水  水盘设定制冷水温*/
                    /*空调计算风量*/
                }
                else
                {   
                    if(s_sysPara.output_pump)
                    {//有能需
                        /*氟盘除湿+热水  水盘设定制冷水温*/
                    }
                    else
                    {
                        /*氟盘除湿+热水  水盘关闭*/
                    }
                    aircod_preColdValve_s = false;   
                    aircod_ec_newwind_s = app_pull_newAir_pwm_value(StoRunParameter.NewAirLevelSet);
                    aircod_ec_backwind_s = app_pull_backAir_pwm_value(StoRunParameter.NewAirLevelSet);
                }  

				app_general_push_cool_backwind_settemp(app_general_pull_coolbackwind_set_temp());//制冷回风20
				app_general_push_cool_humidity_set(app_general_pull_humidity_setvalue());//湿度50%
				app_general_push_cool_outwater_settemp(app_general_pull_cooloutwater_set_temp());//制冷出水20
				app_general_push_cool_backwater_settemp(app_general_pull_coolbackwater_set_temp());//制冷回水25
				app_general_push_lifehotwater_settemp(app_general_pull_life_hotwater_settemp());//生活热水45
				app_general_push_two_value_state(1);//开启
                break;
            }
            case AIR_MODE_HEAT://制热模式
            {               
                aircod_preColdValve_s = false;
                air_warm_s_output =  app_pull_aircod_warm_output();              
                air_humidity_s_output = false;
                if(air_warm_s_output)
                {
                    aircod_preHeatValve_s = true;
                    /*氟盘暖气+热水  水盘设定制热水温*/
                }
                else
                {
                    if(s_sysPara.output_pump)
                    {//有能需
                        /*氟盘暖气+热水  水盘设定制热水温*/
                    }
                    else
                    {
                        /*氟盘暖气+热水  水盘关闭*/
                    }
                    aircod_preHeatValve_s = false;
                }
                
                aircod_ec_newwind_s = app_pull_newAir_pwm_value(StoRunParameter.NewAirLevelSet);
                aircod_ec_backwind_s = app_pull_backAir_pwm_value(StoRunParameter.NewAirLevelSet);
				
				//app_general_push_hot_backwind_settemp(app_general_pull_hotbackwind_set_temp());//制热回风30
				//app_general_push_hot_outwater_settemp(app_general_pull_hotoutwater_set_temp());//制热出水35
				//app_general_push_hot_backwater_settemp(app_general_pull_hotbackwater_set_temp());//制热回水35
				app_general_push_lifehotwater_settemp(app_general_pull_life_hotwater_settemp());//生活热水55
				//app_general_push_two_value_state(1);//开启
                break;
            }
            case AIR_MODE_HUMIDITY://除湿模式
            {	              				
                aircod_preColdValve_s = true;
                aircod_preHeatValve_s = false;  
                air_humidity_s_output = app_pull_aircod_humidity_output();
                if(air_humidity_s_output)
                {
                     /*氟盘除湿+热水  水盘设定制冷水温*/
                }
                else
                {     
                     /*氟盘除湿+热水  水盘关闭*/
                    aircod_ec_newwind_s = app_pull_newAir_pwm_value(StoRunParameter.NewAirLevelSet);
                    aircod_ec_backwind_s = app_pull_backAir_pwm_value(StoRunParameter.NewAirLevelSet);
                }   
				//int16_t app_general_pull_coolbackwind_set_temp(void);
				//int16_t app_general_pull_hotbackwind_set_temp(void);
				//int16_t app_general_pull_cooloutwater_set_temp(void);
				//int16_t app_general_pull_coolbackwater_set_temp(void);
				//int16_t app_general_pull_hotoutwater_set_temp(void);
				//int16_t app_general_pull_hotbackwater_set_temp(void);
				//int16_t app_general_pull_humidity_setvalue(void);
				//int16_t app_general_pull_life_hotwater_settemp(void);

				app_general_push_cool_backwind_settemp(app_general_pull_coolbackwind_set_temp());//制冷回风20
				app_general_push_cool_humidity_set(app_general_pull_humidity_setvalue());//湿度50%
				app_general_push_cool_outwater_settemp(app_general_pull_cooloutwater_set_temp());//制冷出水20
				app_general_push_cool_backwater_settemp(app_general_pull_coolbackwater_set_temp());//制冷回水25
				app_general_push_lifehotwater_settemp(app_general_pull_life_hotwater_settemp());//生活热水45
				app_general_push_two_value_state(1);//开启
                break;
            }
            default:
            {                              
                aircod_preColdValve_s = false;
                aircod_preHeatValve_s = false;  
                air_humidity_s_output = false;
                aircod_ec_newwind_s = app_pull_newAir_pwm_value(StoRunParameter.NewAirLevelSet);
                aircod_ec_backwind_s = app_pull_backAir_pwm_value(StoRunParameter.NewAirLevelSet);
                break;
            }
        }      
    }
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
    macro_createTimer(timer_sys_pwoerup,timerType_second,0);  //上电延时
    static bool sys_powered = false;
    pbc_timerClockRun_task(&timer_sys_pwoerup);
	if(sys_powered)
    {
        if(pbc_pull_timerIsCompleted(&timer_sys_pwoerup))
        {   
//-----------------------------------------------------------------------------
//预冷预热启停处理
            #define RELAY_PRE_COLD    10
            static bool preColdValve_s_backup = false;
                                                
            if(preColdValve_s_backup != aircod_preColdValve_s)
            {   
                preColdValve_s_backup =   aircod_preColdValve_s;            
                if(aircod_preColdValve_s)  
                {
                    mde_relay_on(RELAY_PRE_COLD,0);
                }
                else
                {
                    mde_relay_off(RELAY_PRE_COLD,0);
                }
            }

            #define RELAY_PRE_HEAT    11
            static bool preHeatValve_s_backup = false;	
            if(preHeatValve_s_backup != aircod_preHeatValve_s)
            {   
                preHeatValve_s_backup =  aircod_preHeatValve_s;            
                if(aircod_preHeatValve_s)  
                {
                    mde_relay_on(RELAY_PRE_HEAT,0);
                }
                else
                {
                    mde_relay_off(RELAY_PRE_HEAT,0);
                }
            }
//--------------------------------------------------------------------	 
//------------------------------------------------------------------           
//联动风阀与EC风机			
            #define RELAY_NEW_LINK_VALVE_ON    3
            #define RELAY_NEW_LINK_VALVE_OFF   2
            #define RELAY_BACK_LINK_VALVE_ON   5
            #define RELAY_BACK_LINK_VALVE_OFF  4

            #define EC_PWM_OUTPUT_NEW_AIR   0
            #define EC_PWM_OUTPUT_BACK_AIR  1
            macro_createTimer(timer_ec_output,timerType_second,0);//风机输出延时
            macro_createTimer(timer_linkvavle_output,timerType_second,0);//风阀输出延时
						
            pbc_timerClockRun_task(&timer_ec_output);
            pbc_timerClockRun_task(&timer_linkvavle_output);
						
			static bool newLinkwindValve_s_backup = false;
			static uint8_t ec_newwind_backup = 0;

			static bool backLinkwindValve_s_backup = false;

            if(newLinkwindValve_s_backup != aircod_newLinkwindValve_s)
            {//联动风阀输出
                newLinkwindValve_s_backup = aircod_newLinkwindValve_s;
                if(newLinkwindValve_s_backup)
                {
                    mde_relay_on(RELAY_NEW_LINK_VALVE_ON,0);
                    mde_relay_off(RELAY_NEW_LINK_VALVE_OFF,0);
                    pbc_reload_timerClock(&timer_ec_output,10);
                }
            }
            if(backLinkwindValve_s_backup != aircod_backLinkwindValve_s)
            {//联动风阀输出
                backLinkwindValve_s_backup = aircod_backLinkwindValve_s;
                if(backLinkwindValve_s_backup)
                {
                    mde_relay_on(RELAY_BACK_LINK_VALVE_ON,0);
                    mde_relay_off(RELAY_BACK_LINK_VALVE_OFF,0);
                    pbc_reload_timerClock(&timer_ec_output,10);
                }
            }
            if(aircod_newLinkwindValve_s && aircod_backLinkwindValve_s)
            {
                if(ec_newwind_backup != aircod_ec_newwind_s)
                {   
                    if(pbc_pull_timerIsCompleted(&timer_ec_output))
                    {
                        ec_newwind_backup = aircod_ec_newwind_s;
                    }
                }
                if(ec_newwind_backup)
                {
                    mde_SetInFanPwm(EC_PWM_OUTPUT_NEW_AIR,aircod_ec_newwind_s);
                    mde_SetInFanPwm(EC_PWM_OUTPUT_BACK_AIR,aircod_ec_backwind_s);
                }
            }
            else
            {//风阀不输出时关闭空调外机关闭风机5s后再关闭风阀
                if(ec_newwind_backup != aircod_ec_newwind_s)
                {
                    ec_newwind_backup = aircod_ec_newwind_s;
                    if(aircod_ec_newwind_s == false)
                    {
                        mde_SetInFanPwm(EC_PWM_OUTPUT_NEW_AIR,0);
                        mde_SetInFanPwm(EC_PWM_OUTPUT_BACK_AIR,0);
                    }
                }
                if(aircod_ec_newwind_s == false)
                {
                    if(app_general_pull_aircod_newair_speed() < 20) 
                    {
                        mde_relay_off(RELAY_NEW_LINK_VALVE_ON,0);
                        mde_relay_on(RELAY_NEW_LINK_VALVE_OFF,0);
                        mde_relay_off(RELAY_BACK_LINK_VALVE_ON,0);
                        mde_relay_on(RELAY_BACK_LINK_VALVE_OFF,0);
                    }
                }
            }
        }
    }
    else
    {
        pbc_reload_timerClock(&timer_sys_pwoerup,20);
        sys_powered = true;
    }			
}
void time_stamp_task(void)
{
    
    macro_createTimer(timer_stamp,timerType_millisecond,0);
    pbc_timerClockRun_task(&timer_stamp);
    if(pbc_pull_timerIsCompleted(&timer_stamp))
    {
        pbc_timeStamp_100ms_run_task();
        pbc_reload_timerClock(&timer_stamp,100);
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//name: 通用应用task
//-----------------------------------------------------------------------------
void app_general_task(void)
{
    static sdt_bool sys_cfged = sdt_false;
    mde_IWDG_FeedDog();
    if(sys_cfged)
    {
        mde_hc595_task();        
        app_sto_run_parameter_task();  
		app_pwm_control_task();
        app_logic_realy_task();
        app_master_comp_task();
        app_master_sensor_task();
        app_link_scheduler_task();  
        app_modbus_onewire_task();
        app_modbus_remote_task();      
        app_real_time_clock_task();    
        app_setpmotor_task();
        app_temperature_task();    
        app_upgrade_easy_task();
        app_dac_control_task();
        app_digti_in_task();
        #ifndef DEBUG_TEST
        app_general_aircod_run_task();  
        #endif
        time_stamp_task();
           
        app_led_task();
    }
    else
    {
        sys_cfged = sdt_true;      
        app_read_run_parameter();      
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

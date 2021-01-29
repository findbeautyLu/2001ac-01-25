//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef app_storage_H
#define app_storage_H
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef enum
{
    AIR_MODE_COOL             =0x01,  //制冷
    AIR_MODE_HEAT             =0x02,  //制热
    AIR_MODE_HUMIDITY         =0x03,  //除湿
	AIR_MODE_FAN              =0x00,  //通风
}AirRunMode_Def;

typedef enum
{
   AIR_WIND_HEAT         = 0x00,  //热交换(新风)
   AIR_WIND_BYPASS       = 0x01,  //旁通
   AIR_WIND_INLOOP       = 0x02,  //内循环
}airWindPathStatus_Def;

typedef enum
{   
    AutoFanSet   =0x00,
    LowFanSet    =0x01,
    MiddleFanSet =0x02,
    HighFanSet   =0x03,

}NewAirLevelSet_Def;

typedef union
{
    sdt_int32u sto_data[256];
    struct
    {
        uint8_t                  localId[6];                  //本地ID
        uint8_t                    deviceAddress;            //485地址
        uint8_t                    baudrateValue;            //485波特率
        uint8_t                    evenOddCheck;             //485奇偶校验位 
        uint8_t                    stopBit;                  //停止位    
        uint8_t                  newAirLowPwm;
        uint8_t                  newAirMidPwm;
        uint8_t                  newAirHighPwm;
        uint8_t                  backAirLowPwm;
        uint8_t                  backAirMidPwm;
        uint8_t                  backAirHighPwm;
        uint8_t                 airInFanPwmValue_Low;      //10-100
        uint8_t                 airInFanPwmValue_Middle;   //
        uint8_t                 airInFanPwmValue_High;
        uint8_t                 airOutFanPwmValue_Low;     //10-100
        uint8_t                 airOutFanPwmValue_Middle;  //
        uint8_t                 airOutFanPwmValue_High;
        uint8_t                 ModbusSalveAddr;           //modbus地址
        AirRunMode_Def           airRunmode;           //运行模式
        NewAirLevelSet_Def       NewAirLevelSet;       //设定风速 
        uint16_t                DeviceStatusWord;     //状态字   开关机      
        uint16_t                ModbusSavleBaudrate;       //波特率，2400 4800 9600 19200
        uint16_t                PM25_DeadZone;             //PM25死区值
        uint16_t                CO2_DeadZone;              //CO2死区值		      
        int16_t                 humidity_set;         //设定湿度      
        int16_t                  hotTemp_set;          //制冷设定温度(控制回风温度)
		int16_t                  coldTemp_set;         //制热设定温度     
        int16_t                  set_cold_water_temp;   //主机制冷模式设定水温
        int16_t                  set_heat_water_temp;   //主机制热模式设定水温
        int16_t                  set_living_water_temp; //主机生活热水设定水温
        int16_t                  humidity_DeadZone;     //通风湿度带宽 
        int16_t                  temp_DeadZone;         //通风温度带宽 
        int16_t                  adjust_outdoor_hum;
        int16_t                  adjust_outdoor_temp;
        int16_t                  adjust_outdoor_co2;
        int16_t                  adjust_outdoor_pm25;
        int16_t                  adjust_befor_fu_hum;
        int16_t                  adjust_befor_fu_temp;
        int16_t                  adjust_befor_fu_co2;
        int16_t                  adjust_befor_fu_pm25;
        int16_t                  adjust_after_fu_hum;
        int16_t                  adjust_after_fu_temp;
        int16_t                  adjust_after_fu_co2;
        int16_t                  adjust_after_fu_pm25;
        int16_t                  adjust_back_air_hum;
        int16_t                  adjust_back_air_temp;
        int16_t                  adjust_back_air_co2;
        int16_t                  adjust_back_air_pm25;
        int16_t                  adjust_exhast_air_hum;
        int16_t                  adjust_exhast_air_temp;
        int16_t                  adjust_exhast_air_co2;
        int16_t                  adjust_exhast_air_pm25; 
        uint16_t                  cool_backwind_settemp;
        uint16_t                  hot_backwind_settemp; 
        uint16_t                  cool_outwater_settemp;
        uint16_t                  cool_backwater_settemp; 
        uint16_t                  hot_outwater_settemp;
        uint16_t                  hot_backwater_settemp; 
        bool                     mildewFlag;                //除霉
        bool                     systemPower;               //开关机
    };
}StoRunParamter_Def;
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void app_read_run_parameter(void);
void app_sto_run_parameter_task(void);
void app_push_once_save_sto_parameter(void);
uint8_t * app_pull_local_id(void);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
extern StoRunParamter_Def  StoRunParameter;
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#endif
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include ".\app_cfg.h"

StoRunParamter_Def  StoRunParameter = 
{
        .localId[0] = 0xac,
        .localId[1] = 0x20,
        .localId[2] = 0x12,
        .localId[3] = 0x24,
        .localId[4] = 0x00,
        .localId[5] = 0x01,
};
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
static sdt_bool cfged = sdt_false;
//-------------------------------------------------------------------------------
static void sto_run_parameter_cfg(void)
{
    mde_storage_block_set_length_w(0,sizeof(StoRunParameter));
}
uint8_t * app_pull_local_id(void)
{
    return &StoRunParameter.localId[0];
}
//-----------------------------------------------------------------------------
void RestoreFactoryStorage(void)
{
    StoRunParameter.airRunmode = AIR_MODE_FAN;
    StoRunParameter.hotTemp_set = 200;
    StoRunParameter.coldTemp_set = 260;
    StoRunParameter.humidity_set = 60;
    StoRunParameter.humidity_DeadZone = 3;
    StoRunParameter.temp_DeadZone = 10;
    StoRunParameter.newAirLowPwm = 30;
    StoRunParameter.newAirMidPwm =60;
    StoRunParameter.newAirHighPwm = 90;
    StoRunParameter.backAirLowPwm = 30;
    StoRunParameter.backAirMidPwm = 60;
    StoRunParameter.backAirHighPwm = 90;
    
    StoRunParameter.airInFanPwmValue_Low = 35;
    StoRunParameter.airInFanPwmValue_Middle =65;
    StoRunParameter.airInFanPwmValue_High = 95;
    StoRunParameter.airOutFanPwmValue_Low = 35;
    StoRunParameter.airOutFanPwmValue_Middle = 65;
    StoRunParameter.airOutFanPwmValue_High = 95;
    
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void app_read_run_parameter(void)
{
    if(cfged)
    {     
    }
    else
    {
        cfged = sdt_true;
        sto_run_parameter_cfg();
    }
    if(mde_read_storage_block(0,&StoRunParameter.sto_data[0]))
    {
        StoRunParameter.localId[0] = 0xae;
        StoRunParameter.localId[1] = 0x21;
        StoRunParameter.localId[2] = 0x01;
        StoRunParameter.localId[3] = 0x15;
        StoRunParameter.localId[4] = 0x00;
        StoRunParameter.localId[5] = 0x03;
        StoRunParameter.deviceAddress = 0x01;
        StoRunParameter.baudrateValue = 0x02;
        StoRunParameter.evenOddCheck = 0x00;
        StoRunParameter.stopBit = 0;
       // StoRunParameter.NewAirLevelSet = LowFanSet;
       // StoRunParameter.systemPower = true;
      //  StoRunParameter.airRunmode = AIR_MODE_COOL;
      //  StoRunParameter.hotTemp_set = 200;
      //  StoRunParameter.coldTemp_set = 260;
      //  StoRunParameter.humidity_set = 60;
      //  StoRunParameter.humidity_DeadZone = 3;
      //  StoRunParameter.temp_DeadZone = 10;
     //   StoRunParameter.newAirLowPwm = 30;
      //  StoRunParameter.newAirMidPwm =60;
      //  StoRunParameter.newAirHighPwm = 90;
     //   StoRunParameter.backAirLowPwm = 30;
     //   StoRunParameter.backAirMidPwm = 60;
     //   StoRunParameter.backAirHighPwm = 90;      
    //    StoRunParameter.airInFanPwmValue_Low = 35;
   //     StoRunParameter.airInFanPwmValue_Middle =65;
     //   StoRunParameter.airInFanPwmValue_High = 95;
    //    StoRunParameter.airOutFanPwmValue_Low = 35;
    //    StoRunParameter.airOutFanPwmValue_Middle = 65;
     //   StoRunParameter.airOutFanPwmValue_High = 95;      
    //    StoRunParameter.set_cold_water_temp = 100;
   //     StoRunParameter.set_heat_water_temp = 400;
     //   StoRunParameter.set_living_water_temp = 500;    
//        StoRunParameter.adjust_outdoor_hum = 1;
//        StoRunParameter.adjust_outdoor_temp = 1;
//        StoRunParameter.adjust_outdoor_co2 = 1;
//        StoRunParameter.adjust_outdoor_pm25 = 1;
//        StoRunParameter.adjust_befor_fu_hum = 1;
//        StoRunParameter.adjust_befor_fu_temp = 1;
//        StoRunParameter.adjust_befor_fu_co2 = 1;
//        StoRunParameter.adjust_befor_fu_pm25 = 1;
//        StoRunParameter.adjust_after_fu_hum = 1;
//        StoRunParameter.adjust_after_fu_temp = 1;
//        StoRunParameter.adjust_after_fu_co2 = 1;
//        StoRunParameter.adjust_after_fu_pm25 = 1;
//        StoRunParameter.adjust_back_air_hum = 1;
//        StoRunParameter.adjust_back_air_temp = 1;
//        StoRunParameter.adjust_back_air_co2 = 1;
//        StoRunParameter.adjust_back_air_pm25 = 1;
//        StoRunParameter.adjust_exhast_air_hum = 1;
//        StoRunParameter.adjust_exhast_air_temp = 1;
//        StoRunParameter.adjust_exhast_air_co2 = 1;
//        StoRunParameter.adjust_exhast_air_pm25 = 1;
    }
    else
    {
        RestoreFactoryStorage();
        app_push_once_save_sto_parameter();
    }   
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
macro_createTimer(timer_notSave,(timerType_millisecond | timStatusBits_onceTriggered),0);
//-------------------------------------------------------------------------------
void app_sto_run_parameter_task(void)
{
    if(cfged)
    {     
    }
    else
    {
        cfged = sdt_true;
        sto_run_parameter_cfg();
    }
    
    pbc_timerClockRun_task(&timer_notSave);
    if(pbc_pull_timerIsOnceTriggered(&timer_notSave))
    {
        StoRunParamter_Def rd_sto;
        sdt_int32u i;
        sdt_bool enable_write = sdt_false;
        
        if(mde_read_storage_block(0,&rd_sto.sto_data[0]))
        {
            for(i = 0;i < (sizeof(StoRunParameter)/4);i ++)
            {
                if(rd_sto.sto_data[i] != StoRunParameter.sto_data[i])//数据有变化存储
                {
                    enable_write = sdt_true;
                    break;
                }
            }
        }
        else
        {
            enable_write = sdt_true;
        }
        if(enable_write)
        {
            mde_write_storage_block(0,&StoRunParameter.sto_data[0]);
        }
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void app_push_once_save_sto_parameter(void)
{
    pbc_reload_timerClock(&timer_notSave,3000);//3s后存储一次
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
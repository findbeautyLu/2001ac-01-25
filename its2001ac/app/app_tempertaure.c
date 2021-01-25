#include ".\app_cfg.h"

int16_t temp1,temp2,temp3,temp4,temp5,temp6;
void app_temperature_task(void)
{
    mde_NTCSensor_ScanTask();
    macro_createTimer(measure_delay,timerType_millisecond,0);
    pbc_timerClockRun_task(&measure_delay);
    if(pbc_pull_timerIsCompleted(&measure_delay))
    {
        pbc_reload_timerClock(&measure_delay,1000);
        temp1 = mde_NTCSensor_GetTemperature(SENSOR_ONE,B_3380_10K,Res01_Degree);
        temp2 = mde_NTCSensor_GetTemperature(SENSOR_TWO,B_3380_10K,Res01_Degree);
        temp3 = mde_NTCSensor_GetTemperature(SENSOR_THREE,B_3380_10K,Res01_Degree);
        temp4 = mde_NTCSensor_GetTemperature(SENSOR_FOUR,B_3380_10K,Res01_Degree);
        temp5 = mde_NTCSensor_GetTemperature(SENSOR_FIVE,B_3380_10K,Res01_Degree);
        temp6 = mde_NTCSensor_GetTemperature(SENSOR_SIX,B_3380_10K,Res01_Degree);    
    }
}
//------------------------------E N D-------------------------------------------




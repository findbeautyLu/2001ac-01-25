#include ".\app_cfg.h"

static realTime_t currentTime;
macro_createTimer(timer_notWrite,(timerType_millisecond | timStatusBits_onceTriggered),0);
void app_clock_timing_read(void)
{  
    macro_createTimer(measure_delay,timerType_millisecond,0);
    pbc_timerClockRun_task(&measure_delay);
    if(pbc_pull_timerIsCompleted(&measure_delay))
    {
        pbc_reload_timerClock(&measure_delay,1000);
        mde_sd3078_read(&currentTime);      
    }
}

realTime_t* app_real_time_pull_rtc(void)
{
    return &currentTime;
}

void app_real_time_push_rtc(realTime_t in_rtc)
{
    if((in_rtc.second <= 59) && 
    (in_rtc.minute <= 59) &&
    (in_rtc.hour <= 23)&&
    (in_rtc.day <= 31)&&
    (in_rtc.month <= 12))
    {
        currentTime = in_rtc;
    }
}
bool writeRTCFlag = false;
void app_real_time_clock_task(void)
{
	uint8_t i = 0;
    static sdt_bool sys_cfged = sdt_false;
    if(sys_cfged)
    {       
        pbc_timerClockRun_task(&timer_notWrite);      
		if(writeRTCFlag)
		{
			if(pbc_pull_timerIsOnceTriggered(&timer_notWrite))
			{
				mde_sd3078_write(&currentTime);
				writeRTCFlag = false;
				for(i = 0; i < 9;i++)
				{
					app_general_push_system_dp_stamp((DP_ADDR_START+i),pbc_timeStamp_get_stamp());
				}
				for(i = 14;i < 24;i++)
				{
					app_general_push_system_dp_stamp((DP_ADDR_START+i),pbc_timeStamp_get_stamp());
				}
				app_general_push_system_dp_stamp(DP_ADDR_SYSTEM_LIS_MAIN_MACHINE_TEMP,pbc_timeStamp_get_stamp());
				app_general_push_system_dp_stamp(DP_ADDR_SYSTEM_LIS_485_PAREMETER,pbc_timeStamp_get_stamp());
			}				
		}
		else
		{
			app_clock_timing_read();
		}
    }
    else
    {
        mde_sd3078_configure();
//        currentTime.hour = 9;
//        currentTime.minute = 43;
//        currentTime.week = 2;
//        currentTime.day = 12;
//        currentTime.month = 1;
//        currentTime.year = 21;
//        mde_sd3078_write(&currentTime);
        sys_cfged = true;
        mde_sd3078_read(&currentTime);
        pbc_timeStamp_get_absolutely_time(((uint8_t *)&currentTime));//最新时间戳
        uint8_t i = 0;
        for(i = 0; i < 9;i++)
        {
            app_general_push_system_dp_stamp((DP_ADDR_START+i),pbc_timeStamp_get_stamp());
        }
        for(i = 14;i < 24;i++)
        {
            app_general_push_system_dp_stamp((DP_ADDR_START+i),pbc_timeStamp_get_stamp());
        }
        app_general_push_system_dp_stamp(DP_ADDR_SYSTEM_LIS_MAIN_MACHINE_TEMP,pbc_timeStamp_get_stamp());
        app_general_push_system_dp_stamp(DP_ADDR_SYSTEM_LIS_485_PAREMETER,pbc_timeStamp_get_stamp());
    } 
}
void app_push_once_save_write_rtc(void)
{
    pbc_reload_timerClock(&timer_notWrite,3000);//3s后写一次
	writeRTCFlag = true;
}
//------------------------------E N D-------------------------------------------





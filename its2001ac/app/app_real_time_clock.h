//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#ifndef _APP_REAL_TIME_CLOCK_H
#define _APP_REAL_TIME_CLOCK_H
//------------------------------E N D-------------------------------------------
//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
void app_real_time_clock_task(void);
realTime_t* app_real_time_pull_rtc(void);
void app_real_time_push_rtc(realTime_t in_rtc);
void app_push_once_save_write_rtc(void);
//------------------------------E N D-------------------------------------------
#endif
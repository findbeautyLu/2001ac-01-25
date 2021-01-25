#ifndef __PBC_TIMESTAMP_H__
#define __PBC_TIMESTAMP_H__
#include <stdbool.h>
#include "stdint.h"

#define START_YEAR          20
#define START_MONTH         11
#define START_DAY           1
#define START_HOUR          0
#define START_MINUTE        0
#define START_SECOND        0
#define START_MILLSECOND    0
////++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++
#define  MINUTE_ADDR    0
#define  HOUR_ADDR      1
#define  WEEK_ADDR      2
#define  SECOND_ADDR    3
#define  DAY_ADDR       4
#define  MONHT_ADDR     5
#define  YEAR_ADDR      6
////func  
////名称: pbc_absolutely_time
////功能: 根据rtc与start值运算计算出初始时间戳 
////入口: uint8_t指针    rtc      <<----------------------------- 传入
////出口: 无 
void pbc_timeStamp_get_absolutely_time(uint8_t *rtc);
////------------------------------E N D-----------------------------------------


////++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++
///task  
////名称: pbc_timeStamp_get_stamp
////功能: 获取当前当前运行时间戳 
////入口: uint16_t  millsecond    <<----------------------------- 传入
////出口: 无 
uint32_t pbc_timeStamp_get_stamp(void);
////------------------------------E N D-----------------------------------------

void pbc_timeStamp_100ms_run_task(void);
////++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++
///task  
////名称: pbc_timeStamp_task
////功能: 时间戳任务 
////入口: uint16_t  millsecond    <<----------------------------- 传入
////出口: 无 
void pbc_timeStamp_task(uint16_t millsecond);
////------------------------------E N D-----------------------------------------
#endif

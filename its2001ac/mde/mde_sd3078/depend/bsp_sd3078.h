//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#ifndef _BSP_SD3078_H
#define _BSP_SD3078_H
//------------------------------E N D-------------------------------------------
#include "stdint.h"
#include <stdbool.h>

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++

typedef union
{
    uint8_t buf[7];
    struct
    {
        uint8_t second;    
        uint8_t minite;     
        uint8_t hour;    
        uint8_t week;
        uint8_t day;
        uint8_t month;
        uint8_t year;
    };
}sd3078Para_t;

#define ACK                     0xff
#define NACK                    0X00
#define WriteMode               0x64
#define ReadMode                0x65

#define WriteEnable             0x00
#define ReadEnable              0xff

#define WriteEnableRegAdd_1     0x10
#define WriteEnableRegAdd_2_3   0x0f

#define WriteEnableCmd_1        0x80
#define WriteEnableCmd_2_3      0xff

#define WriteDisableCmd_1       0x00
#define WriteDisbaleCmd_2_3     0x7b



#define BuffSecond              0
#define BuffMinnute             1
#define BuffHour                2

bool bsp_sd3078_configure(void);
bool bsp_sd3078_readRealTime(sd3078Para_t *_out_rtc);
bool bsp_sd3078_writeRealTime(sd3078Para_t *rtc);
//------------------------------E N D-------------------------------------------

#endif

//-----------------------BSP_SD3078.h--END------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#include ".\mde_sd3078.h"
#include ".\depend\bsp_sd3078.h"

//------------------------------E N D-------------------------------------------


bool mde_sd3078_configure(void)
{
    if(bsp_sd3078_configure())
    {
        return true;
    }
    return false;
}

void mde_sd3078_write(realTime_t *ptRT)
{
    sd3078Para_t sd3078Para;
    sd3078Para.second = ptRT->second;
    sd3078Para.hour = ptRT->hour;
    sd3078Para.minite = ptRT->minute;
    sd3078Para.week = ptRT->week;
    sd3078Para.year = ptRT->year;
    sd3078Para.month = ptRT->month;
    sd3078Para.day = ptRT->day;
    bsp_sd3078_writeRealTime(&sd3078Para);
}

void mde_sd3078_read(realTime_t *_out_ptRT)
{
    sd3078Para_t sd3078Para;
    if(bsp_sd3078_readRealTime(&sd3078Para))
    {
        _out_ptRT->hour   = sd3078Para.hour;
        _out_ptRT->minute = sd3078Para.minite;
        _out_ptRT->week   = sd3078Para.week;
        _out_ptRT->second = sd3078Para.second;
        _out_ptRT->year = sd3078Para.year;
        _out_ptRT->month   = sd3078Para.month;
        _out_ptRT->day = sd3078Para.day;
    }       
}



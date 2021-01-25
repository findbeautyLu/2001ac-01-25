//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\pbc\pbc_sysTick\pbc_sysTick.h"
#include<stdint.h>
#include<stdio.h>
#include<stdbool.h>
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef struct
{
    uint8_t  dacSetValue;   //0--100
    uint8_t  adcRunValue;   //
    void(*pushDacValue)(uint8_t Value);
}dacParameter_Def;

static bool cfged = false;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include ".\sod_dac.h"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void dac_run(void)
{
    uint8_t i = 0;
    if(cfged)
    {     
    }
    else
    {
        mde_dac_solid_cfg();
        cfged = true;
    }
    for(i = 0;i < max_solid;i++)
    {
        if(mde_dac[i].dacSetValue == mde_dac[i].adcRunValue)
        {

        }
        else
        {
            if(mde_dac[i].dacSetValue>mde_dac[i].adcRunValue)
            {
               mde_dac[i].adcRunValue++;
            }
            else
            {
                mde_dac[i].adcRunValue--;
            }
            mde_dac[i].pushDacValue(mde_dac[i].adcRunValue);
        }      
    }
}

void mde_push_dac_value(uint8_t in_solidNum,uint8_t in_dacValue)
{
    if(cfged)
    {     
    }
    else
    {
        mde_dac_solid_cfg();
        cfged = true;
    }
    mde_dac[in_solidNum].dacSetValue = in_dacValue;
}

void mde_dac_task(void)
{
    macro_createTimer(measure_delay,timerType_millisecond,0);
    pbc_timerClockRun_task(&measure_delay);
    if(cfged)
    {    
        if(pbc_pull_timerIsCompleted(&measure_delay))
        {
            pbc_reload_timerClock(&measure_delay,500);
            dac_run();
        }
    }
    else
    {
        cfged = true;
        mde_dac_solid_cfg();
    } 
}
//+++++++++++++++++++++++++++++++solid+++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
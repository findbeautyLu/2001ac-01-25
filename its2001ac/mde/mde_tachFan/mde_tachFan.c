//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include ".\depend\bsp_tachFan.h"
#include "..\..\pbc\pbc_sysTick\pbc_sysTick.h"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define  MinimumPWMValue  10
#define  MaxPWMValue      100
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef struct
{
    uint16_t lastSysTick;
    uint8_t  PwmSetValue;   //0--100
    uint8_t  PwmRunValue;   //
    void(*LoadPwmToPort)(INT8U Value);
}PwmControlParameter_Def;

void PwmControlTask(PwmControlParameter_Def *Me)
{ 
    if(Me->PwmSetValue==Me->PwmRunValue)
    {
    }
    else
    {
        if(Me->PwmSetValue>Me->PwmRunValue)
        {
            if(Me->PwmRunValue<MinimumPWMValue)   //MinimumPWMValue==10
            {
                Me->PwmRunValue=MinimumPWMValue;
            }
            else
            {
                Me->PwmRunValue++;
            }
        }
        else
        {
            Me->PwmRunValue--;
        }
        Me->LoadPwmToPort(Me->PwmRunValue);
    }
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include ".\sod_tachFan.h"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
static INT16U airTach_minute[max_solid];
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void mde_tachFan_Task(void)
{
    static BOOLEAN cfged = BN_FALSE;
    uint8_t i = 0;
    if(cfged)
    {
        bsp_fan_tach_task();         
        for(i = 0;i < max_solid;i++)
        {
            airTach_minute[i] = bsp_pull_inair_tach(i);
        }
    }
    else
    {
        cfged = BN_TRUE;
        bsp_tachFan_cfg();
    }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
uint16_t mde_pull_airTach(uint8_t in_solidNum)
{
    return(airTach_minute[in_solidNum]);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


void mde_EcMotorControlTask(void)
{
    static bool cfged = false;
    uint8_t i = 0;
    if(cfged)
    {
        macro_createTimer(measure_delay,timerType_millisecond,0);
        pbc_timerClockRun_task(&measure_delay);
        if(pbc_pull_timerIsCompleted(&measure_delay))
        {
            pbc_reload_timerClock(&measure_delay,500);
            for(i = 0; i < max_solid;i++)
            {
                PwmControlTask(&mde_pwm[i]);
            }
        }
    }
    else
    {
        cfged = true;
        mde_tach_solid_cfg();
    }
    
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void mde_SetInFanPwm(uint8_t in_solidNum,uint8_t in_PwmData)
{
    if(in_PwmData>100)
    {        
        mde_pwm[in_solidNum].PwmSetValue=100;
    }
    else
    {
        mde_pwm[in_solidNum].PwmSetValue=in_PwmData;
    }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
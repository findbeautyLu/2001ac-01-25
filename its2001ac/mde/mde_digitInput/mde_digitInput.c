//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include ".\mde_digitInput.h"
#include "..\..\pbc\pbc_sysTick\pbc_sysTick.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define SWITCH_DELAY_1S    10

typedef struct  
{
    bool             digitInputStatus;
    uint8_t          statusChangeCount;
    bool(*phyReadInputStatsu)(void);
}digit_parameter_def;

static bool cfged = false;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include ".\sod_digitInput.h"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void mde_digit_in_task(void)
{
    macro_createTimer(measure_delay,timerType_millisecond,100);
    pbc_timerClockRun_task(&measure_delay);
    uint8_t i = 0;
    bool phyReadInputStatsu = false;
    if(cfged)
    {
        if(pbc_pull_timerIsCompleted(&measure_delay))
        {
            pbc_reload_timerClock(&measure_delay,100);          
            for(i = 0;i < max_solid;i++)
            {
                phyReadInputStatsu = digitInut[i].phyReadInputStatsu();
                if(digitInut[i].digitInputStatus == false)
                {
                    if(phyReadInputStatsu == false)
                    {
                        digitInut[i].statusChangeCount++;
                        if(digitInut[i].statusChangeCount >= SWITCH_DELAY_1S)
                        {
                            digitInut[i].statusChangeCount = SWITCH_DELAY_1S;
                            digitInut[i].digitInputStatus = true;
                        }
                    }
                }
                else
                {
                    if(phyReadInputStatsu)
                    {
                        if(digitInut[i].statusChangeCount > 0)
                        {
                            digitInut[i].statusChangeCount--;
                            if(digitInut[i].statusChangeCount == 0)
                            {
                                digitInut[i].digitInputStatus = false;
                            }
                        }
                    }
                }
            }
        }
    } 
    else
    {
        cfged = true;
        mde_digit_input_solid_cfg();
    }   
}

bool mde_gigit_pull_status(uint8_t in_solidNum)
{
    if(!cfged)
    {
        cfged = true;
        mde_digit_input_solid_cfg();
    }
    if(in_solidNum < max_solid)
    {
        return digitInut[in_solidNum].digitInputStatus;
    }
    else
    {
        while(1);
    }
}
//++++++++++++++++++++++++++++++++++++End++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include ".\depend\bsp_tach.h"
#include "..\..\pbc\pbc_sysTick\pbc_sysTick.h"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include ".\sod_tach.h"
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
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
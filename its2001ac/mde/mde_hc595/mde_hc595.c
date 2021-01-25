//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#include ".\mde_hc595.h"
#include ".\depend\bsp_hc595.h"
#include ".\stdio.h"
//------------------------------E N D-------------------------------------------
void mde_hc59c_push_status(uint8_t in_port,bool in_status)
{
    if(in_port <= 31)
    {
        uint32_t tempData = BSP_pll_hc595_data();
        if(in_status)
        {
            tempData |= (0x01 << in_port);
        }
        else
        {
            tempData &= (~(0x01 << in_port));
        } 
        BSP_push_hc595_data(tempData);
    }
}

void mde_hc595_task(void)
{
    static bool initFlag = false;
    if(!initFlag)
    {
        initFlag = true;
        BSP_hc595_Configure();
    }
    else
    {
        BSP_hc595_control();
    }
    
}
//-----------------------Mod_Backlight.c--END------------------------------------




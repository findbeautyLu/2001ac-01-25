//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#ifndef _SOD_LED_H
#define _SOD_LED_H
//------------------------------E N D-------------------------------------------
//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#include ".\depend\bsp_Led.h"
#include <stdint.h>
#include <stdbool.h>
//------------------------------E N D-------------------------------------------
#define max_solid    4

static led_device_t  mde_led[max_solid];
void mde_led_solid_cfg(void)
{
    mde_led[0].led_on = BSP_Led1_Enable;
    mde_led[0].led_off = BSP_Led1_Disable;
    mde_led[0].ledonFlag = false;
   // mde_led[0].handleTickCfg = 1;

    mde_led[1].led_on = BSP_Led2_Enable;
    mde_led[1].led_off = BSP_Led2_Disable;
    mde_led[1].ledonFlag = false;
    //mde_led[1].handleTickCfg = 1;

    mde_led[2].led_on = BSP_Led3_Enable;
    mde_led[2].led_off = BSP_Led3_Disable;
    mde_led[2].ledonFlag = false;
   // mde_led[2].handleTickCfg = 1;

    mde_led[3].led_on = BSP_Led4_Enable;
    mde_led[3].led_off = BSP_Led4_Disable;
    mde_led[3].ledonFlag = false;
   // mde_led[3].handleTickCfg = 1; 
}
#endif


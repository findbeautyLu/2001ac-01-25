//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#ifndef _SOD_RELAY_H
#define _SOD_RELAY_H
//------------------------------E N D-------------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#include ".\depend\bsp_Relay.h"
#include<stdint.h>
#include<stdio.h>
#include<stdbool.h>
//------------------------------E N D-------------------------------------------

#define max_solid    12

static relay_t relay[max_solid];

void mde_relay_solid_cfg(void)
{
    uint8_t i = 0;
    for(i = 0; i < max_solid;i++)
    {
        relay[i].mode = RELAY_MODE_OFF;
        relay[i].onEnableFlag = true;
        relay[i].onDelay = 0;
        relay[i].offDelay = 0;
        relay[i].onEnableDelay = 0;
    }
    relay[0].relay_on = BSP_Relay1_Enable;
    relay[0].relay_off = BSP_Relay1_Disable;
    
    relay[1].relay_on = BSP_Relay2_Enable;
    relay[1].relay_off = BSP_Relay2_Disable;

    relay[2].relay_on = BSP_Relay3_Enable;
    relay[2].relay_off = BSP_Relay3_Disable;

    relay[3].relay_on = BSP_Relay4_Enable;
    relay[3].relay_off = BSP_Relay4_Disable;

    relay[4].relay_on = BSP_Relay5_Enable;
    relay[4].relay_off = BSP_Relay5_Disable;

    relay[5].relay_on = BSP_Relay6_Enable;
    relay[5].relay_off = BSP_Relay6_Disable;

    relay[6].relay_on = BSP_Relay7_Enable;
    relay[6].relay_off = BSP_Relay7_Disable;

    relay[7].relay_on = BSP_Relay8_Enable;
    relay[7].relay_off = BSP_Relay8_Disable;

    relay[8].relay_on = BSP_Relay9_Enable;
    relay[8].relay_off = BSP_Relay9_Disable;

    relay[9].relay_on = BSP_Relay10_Enable;
    relay[9].relay_off = BSP_Relay10_Disable;

    relay[10].relay_on = BSP_Relay11_Enable;
    relay[10].relay_off = BSP_Relay11_Disable;

    relay[11].relay_on = BSP_Relay12_Enable;
    relay[11].relay_off = BSP_Relay12_Disable;
}
#endif


//-----------------------Mod_Relay.h--END------------------------------------

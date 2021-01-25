//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#ifndef _SOD_NTC3380_H
#define _SOD_NTC3380_H
//------------------------------E N D-------------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#include ".\depend\bsp_ntcSensor.h"
//------------------------------E N D-------------------------------------------
#define max_solid    6

static NtcSensorParameter_Def mde_sensor[max_solid];

void mde_ntc_sensor_solid_cfg(void)
{
//-----------------------------------------------------------------------------
    BSP_ADC_Configure();
    mde_sensor[0].SelectAdcChannel = BSP_ADC_Channal_one;
    mde_sensor[0].timeOutDelay.timStatusBits = timStatusBits_typeMillsecond;
    mde_sensor[0].channelClockDelay.timStatusBits = timStatusBits_typeMillsecond;

    mde_sensor[1].SelectAdcChannel = BSP_ADC_Channal_two;
    mde_sensor[1].timeOutDelay.timStatusBits = timStatusBits_typeMillsecond;
    mde_sensor[1].channelClockDelay.timStatusBits = timStatusBits_typeMillsecond;

    mde_sensor[2].SelectAdcChannel = BSP_ADC_Channal_three;
    mde_sensor[2].timeOutDelay.timStatusBits = timStatusBits_typeMillsecond;
    mde_sensor[2].channelClockDelay.timStatusBits = timStatusBits_typeMillsecond;

    mde_sensor[3].SelectAdcChannel = BSP_ADC_Channal_four;
    mde_sensor[3].timeOutDelay.timStatusBits = timStatusBits_typeMillsecond;
    mde_sensor[3].channelClockDelay.timStatusBits = timStatusBits_typeMillsecond;

    mde_sensor[4].SelectAdcChannel = BSP_ADC_Channal_five;
    mde_sensor[4].timeOutDelay.timStatusBits = timStatusBits_typeMillsecond;
    mde_sensor[4].channelClockDelay.timStatusBits = timStatusBits_typeMillsecond;

    mde_sensor[5].SelectAdcChannel = BSP_ADC_Channal_six;
    mde_sensor[5].timeOutDelay.timStatusBits = timStatusBits_typeMillsecond;
    mde_sensor[5].channelClockDelay.timStatusBits = timStatusBits_typeMillsecond;
//-----------------------------------------------------------------------------
}

#endif
//-----------------------Mod_ADC_NTC.h--END-----------------------------------


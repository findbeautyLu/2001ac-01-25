//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef _SOD_TACHFAN_H
#define _SOD_TACHFAN_H
//----------------------------------------------------------------------------------
#include<stdint.h>
#include<stdio.h>
#include<stdbool.h>
#include ".\depend\bsp_tachFan.h"
#define max_solid    3

static PwmControlParameter_Def mde_pwm[max_solid];

void mde_tach_solid_cfg(void)
{
    PwmControl_Configure();
    mde_pwm[0].LoadPwmToPort = TIM8CH4_Loading_PwmDutyOut;
    mde_pwm[1].LoadPwmToPort = TIM8CH3_Loading_PwmDutyOut;
    mde_pwm[2].LoadPwmToPort = TIM8CH2_Loading_PwmDutyOut;
}
#endif
//++++++++++++++++++++++++++++++++++End+++++++++++++++++++++++++++++++++++++++++
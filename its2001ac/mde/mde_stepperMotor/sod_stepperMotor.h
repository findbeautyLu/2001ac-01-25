//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef _SOD_STEPPERMOTOR_H
#define _SOD_STEPPERMOTOR_H
//----------------------------------------------------------------------------------
#include<stdint.h>
#include<stdio.h>
#include<stdbool.h>
#include ".\depend\bsp_Steppermotor.h"
//----------------------------------------------------------------------------------
#define max_solid    2
#define FanValveBit_IsOpened     0x01   //1 opened
#define FanValveBit_IsClosed     0x02   //1 closed
#define FanValveBit_Opening      0x04   
#define FanValveBit_Closing      0x08
#define FanValveBit_OnceOpen     0x10   
#define FanValveBit_OnceClose    0x20   

static StepRunParameter_Def mde_stepperMotor[max_solid];

void mde_stepperMotor_solid_cfg(void)
{
//-----------------------------------------------------------------------------
    mde_stepperMotor[0].Current_Step = 0;
    mde_stepperMotor[0].StepRunPulse = 0;
    mde_stepperMotor[0].FanValve_Bits = FanValveBit_IsOpened;
    mde_stepperMotor[0].motor_output = BSP_FirstMotor_Output;

    mde_stepperMotor[1].Current_Step = 0;
    mde_stepperMotor[1].StepRunPulse = 0;
    mde_stepperMotor[1].FanValve_Bits = FanValveBit_IsOpened;
    mde_stepperMotor[1].motor_output = BSP_SecondMotor_Output;

   /* mde_stepperMotor[2].Current_Step = 0;
    mde_stepperMotor[2].StepRunPulse = 0;
    mde_stepperMotor[2].FanValve_Bits = FanValveBit_IsOpened;
    mde_stepperMotor[2].motor_output = BSP_ThirdMotor_Output;

    mde_stepperMotor[3].Current_Step = 0;
    mde_stepperMotor[3].StepRunPulse = 0;
    mde_stepperMotor[3].FanValve_Bits = FanValveBit_IsOpened;
    mde_stepperMotor[3].motor_output = BSP_FourMotor_Output;*/
//-----------------------------------------------------------------------------
}
#endif
//++++++++++++++++++++++++++++++++++End+++++++++++++++++++++++++++++++++++++++++
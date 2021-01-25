//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include ".\depend\bsp_Steppermotor.h"
#include "..\..\pbc\pbc_sysTick\pbc_sysTick.h"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
#define DISTANCE_STEP      1250
//------------------------------------------------------------------------------
typedef struct
{
    INT16S   StepRunPulse;   //0 Stop Negative--CCW  Positive--CW
    INT8U    Current_Step;   //0-7
    INT8U    FanValve_Bits;   //
    timerClock_def measure_delay;
    void (*motor_output)(StepperOutLevel_Def _In_OutLevel);
}StepRunParameter_Def;

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include ".\sod_stepperMotor.h"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//名称: 马达开阀
//功能: 
//入口: 无
//
//出口: 无
void mde_motor_GotoOpen(uint8_t in_solidNum)
{
    if(in_solidNum < max_solid)
    {
        mde_stepperMotor[in_solidNum].FanValve_Bits |= FanValveBit_OnceOpen;
        mde_stepperMotor[in_solidNum].FanValve_Bits &= ~FanValveBit_OnceClose;
    }
    else
    {
        while (1);
    }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//名称: 马达关阀
//功能: 
//入口: 无
//
//出口: 无
void mde_motor_GotoClose(uint8_t in_solidNum)
{
    if(in_solidNum < max_solid)
    {
        mde_stepperMotor[in_solidNum].FanValve_Bits &= ~FanValveBit_OnceOpen;
        mde_stepperMotor[in_solidNum].FanValve_Bits |= FanValveBit_OnceClose;
    }
    else
    {
        while (1);
    }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
BOOLEAN mde_pull_motor_busy(uint8_t in_solidNum)
{
    uint8_t i = 0;
    BOOLEAN result = BN_FALSE;
    if(in_solidNum < max_solid)
    { 
        if(mde_stepperMotor[i].FanValve_Bits & (INT8U)(FanValveBit_Opening|FanValveBit_Closing|FanValveBit_OnceOpen|FanValveBit_OnceClose))
        {
            result = BN_TRUE;
        } 
    }
    else
    {
        while(1);
    }
    return result;
}

BOOLEAN _pull_motor_busy(uint8_t in_solidNum)
{
    uint8_t i = 0;
    BOOLEAN result = BN_FALSE;
    if(in_solidNum < max_solid)
    {
        for(i = 0;i < max_solid;i++)
        {
            if(i != in_solidNum)
            {
                if(mde_stepperMotor[i].FanValve_Bits & (INT8U)(FanValveBit_Opening|FanValveBit_Closing))
                {
                    result = BN_TRUE;
                }
            }
        }
    }
    else
    {
        while(1);
    }
    return result;
}

void mde_SetppermotorRun_Task(void)
{
    static BOOLEAN Cfged = BN_FALSE;
    uint8_t i = 0;
    if(Cfged)
    {
        for(i = 0;i < max_solid;i++)
        {
            if(_pull_motor_busy(i) == BN_FALSE)
            {
                if(mde_stepperMotor[i].FanValve_Bits & FanValveBit_OnceOpen)
                {
                    mde_stepperMotor[i].FanValve_Bits &= ~FanValveBit_OnceOpen;
                    if(mde_stepperMotor[i].FanValve_Bits & FanValveBit_IsOpened)
                    {
                    }
                    else if(mde_stepperMotor[i].FanValve_Bits & FanValveBit_IsClosed)
                    {
                        mde_stepperMotor[i].FanValve_Bits &= ~FanValveBit_IsClosed;
                        mde_stepperMotor[i].FanValve_Bits |= FanValveBit_Opening;
                        mde_stepperMotor[i].StepRunPulse = DISTANCE_STEP;
                    }
                    else if(mde_stepperMotor[i].FanValve_Bits & FanValveBit_Opening)
                    {
                    }
                    else if(mde_stepperMotor[i].FanValve_Bits & FanValveBit_Closing)
                    {
                        mde_stepperMotor[i].FanValve_Bits |= FanValveBit_Opening;
                        mde_stepperMotor[i].FanValve_Bits &= ~FanValveBit_Closing;
                        mde_stepperMotor[i].StepRunPulse = DISTANCE_STEP + mde_stepperMotor[i].StepRunPulse;
                    }
                }
                else if(mde_stepperMotor[i].FanValve_Bits & FanValveBit_OnceClose)
                {
                    mde_stepperMotor[i].FanValve_Bits &= ~FanValveBit_OnceClose;
                    if(mde_stepperMotor[i].FanValve_Bits & FanValveBit_IsOpened)
                    {
                        mde_stepperMotor[i].FanValve_Bits &= ~FanValveBit_IsOpened;
                        mde_stepperMotor[i].FanValve_Bits |= FanValveBit_Closing;
                        mde_stepperMotor[i].StepRunPulse = -DISTANCE_STEP;
                    }
                    else if(mde_stepperMotor[i].FanValve_Bits & FanValveBit_IsClosed)
                    {
                    }
                    else if(mde_stepperMotor[i].FanValve_Bits & FanValveBit_Opening)
                    {
                        mde_stepperMotor[i].FanValve_Bits &= ~FanValveBit_Opening;
                        mde_stepperMotor[i].FanValve_Bits |= FanValveBit_Closing;
                        mde_stepperMotor[i].StepRunPulse = mde_stepperMotor[i].StepRunPulse - DISTANCE_STEP;
                    }
                    else if(mde_stepperMotor[i].FanValve_Bits & FanValveBit_Closing)
                    {
                    }
                }
            }
          //  macro_createTimer(measure_delay,timerType_millisecond,8);
            pbc_timerMillRun_task(&mde_stepperMotor[i].measure_delay);
            if(pbc_pull_timerIsCompleted(&mde_stepperMotor[i].measure_delay))
            {
                pbc_reload_timerClock(&mde_stepperMotor[i].measure_delay,8);
                if(0 == mde_stepperMotor[i].StepRunPulse)
                {
                    if(mde_stepperMotor[i].FanValve_Bits & FanValveBit_Opening)
                    {
                        mde_stepperMotor[i].FanValve_Bits &= ~FanValveBit_Opening;
                        mde_stepperMotor[i].FanValve_Bits |= FanValveBit_IsOpened;
                    }
                    else if(mde_stepperMotor[i].FanValve_Bits & FanValveBit_Closing)
                    {
                        mde_stepperMotor[i].FanValve_Bits &= ~FanValveBit_Closing;
                        mde_stepperMotor[i].FanValve_Bits |= FanValveBit_IsClosed;
                    }
                    mde_stepperMotor[i].motor_output(StepM_Stop);
                }
                else
                {
                    switch(mde_stepperMotor[i].Current_Step)
                    {
                        case 0:
                        {
                            mde_stepperMotor[i].motor_output(StepM_00);
                            break;
                        }
                        case 1:
                        {
                            mde_stepperMotor[i].motor_output(StepM_01);
                            break;
                        }
                        case 2:
                        {
                            mde_stepperMotor[i].motor_output(StepM_02);
                            break;
                        }
                        case 3:
                        {
                            mde_stepperMotor[i].motor_output(StepM_02);
                            break;
                        }
                        case 4:
                        {
                            mde_stepperMotor[i].motor_output(StepM_04);
                            break;
                        }
                        case 5:
                        {
                            mde_stepperMotor[i].motor_output(StepM_05);
                            break;
                        }
                        case 6:
                        {
                            mde_stepperMotor[i].motor_output(StepM_06);
                            break;
                        }
                        case 7:
                        {
                            mde_stepperMotor[i].motor_output(StepM_07);
                            break;
                        }
                        default:
                        {
                            mde_stepperMotor[i].Current_Step = 0;
                            break;
                        }
                    } 
                    if(mde_stepperMotor[i].StepRunPulse > 0)
                    {
                        mde_stepperMotor[i].StepRunPulse --;
                        mde_stepperMotor[i].Current_Step ++;
                        if(mde_stepperMotor[i].Current_Step >7)
                        {
                            mde_stepperMotor[i].Current_Step = 0;
                        }
                    }
                    else
                    {
                        mde_stepperMotor[i].StepRunPulse ++;
                        mde_stepperMotor[i].Current_Step --;
                        if(mde_stepperMotor[i].Current_Step >7)
                        {
                            mde_stepperMotor[i].Current_Step = 7;
                        }
                    }                 
                }
            }
        }
    }
    else
    {
        mde_stepperMotor_solid_cfg();
        for(i = 0; i < max_solid;i++)
        {
            mde_motor_GotoClose(i);
        }       
        Cfged = BN_TRUE;
    }
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//--------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
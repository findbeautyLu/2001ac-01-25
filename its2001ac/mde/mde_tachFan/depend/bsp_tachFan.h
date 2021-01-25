//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef _BSP_TACHFAN_H
#define _BSP_TACHFAN_H
//----------------------------------------------------------------------------------
#ifndef  Data_Retyped
    #include ".\DataTypedef.h"
#endif
#include<stdint.h>
#include<stdio.h>
#include<stdbool.h>
//-----------------------------------------------------------------------------------
//Function
//名称: 风速测量配置
//功能: 配置IO管脚、定时器
//入口: 无
//
//出口: 无

void bsp_tachFan_cfg(void);
void bsp_fan_tach_task(void);
void PwmControl_Configure(void);
void TIM8CH4_Loading_PwmDutyOut(INT8U PwmData);
void TIM8CH3_Loading_PwmDutyOut(INT8U PwmData);
void TIM8CH2_Loading_PwmDutyOut(INT8U PwmData);
//-----------------------------------------------------------------------------------
uint16_t bsp_pull_inair_tach(uint8_t in_solidNum);
//-----------------------------------------------------------------------------------
#endif
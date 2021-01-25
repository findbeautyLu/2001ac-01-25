
/*******************************************************************************
*
*	模块名称 : 独立看门狗
*	文件名称 : BSP_IWDG.c
*	版    本 : V1.0
*	说    明 : 1、配置和喂狗函数
*                  
*                    
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0   2018-08-23  jinzh 正式发布
*	Copyright (C), 2015-2020,   menredGroup
*       编译环境 ：IAR FOR STM8 V2.2      
*******************************************************************************/


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#include "stm32f10x.h"
//------------------------------E N D-------------------------------------------
//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*********************配置看门狗4S超时时间**********************************
#define LsiFreq   40000
void BSP_IWDG_Configure(void)
{//64*1000/40kHz = 1.6s
    RCC_LSICmd(ENABLE);
  
  /* Wait till LSI is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
    {}
  
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    IWDG_SetPrescaler(IWDG_Prescaler_32);

    /* Set counter reload value to obtain 1000ms IWDG TimeOut.
       Counter Reload Value = 1000ms/IWDG counter clock period
                            = 1000ms / (LSI/32)
                            = 1s / (LsiFreq/32)
                            = LsiFreq/(32 * 1)
                            = LsiFreq/32
     */
    IWDG_SetReload(LsiFreq/32);

    /* Reload IWDG counter */
    IWDG_ReloadCounter();

    /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
    IWDG_Enable();
}
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//******************************喂狗********************************************
void BSP_IWDG_FeedDog(void)
{
	IWDG_ReloadCounter();
}
//------------------------------E N D-------------------------------------------

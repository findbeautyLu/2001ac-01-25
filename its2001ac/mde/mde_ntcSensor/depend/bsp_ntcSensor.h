//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#ifndef _BSP_NTC3380_H
#define _BSP_NTC3380_H
//------------------------------E N D-------------------------------------------
#include <stdbool.h>
#include <stdint.h>
#include "stm32f10x.h"
//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**********本库支持4通道，仅需将Max修改为4******************
//------------------------------E N D-------------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Init
//名称:  初始化
//功能:  配置ADC参数
//入口:  无
//出口:  无
void BSP_ADC_Configure(void);
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Function
//名称:  ADC开启转换
//功能:  
//入口:  无
//出口:  无
void BSP_ADC12_Start(void);
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Function
//名称:  获取单次ADC值
//功能:  
//入口:  无
//出口:  INT16U 的ADC值
uint16_t BSP_ADC1_GetValue(void);
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Function
//名称:  ADC通道选择
//功能:  
//入口:                _In_Channel  <<-----------------------传入
//入口宏定义，不同函数调用可以修改宏定义的值
void BSP_ADC_Channal_one(void);
void BSP_ADC_Channal_two(void);
void BSP_ADC_Channal_three(void);
void BSP_ADC_Channal_four(void);
void BSP_ADC_Channal_five(void);
void BSP_ADC_Channal_six(void);
//------------------------------E N D-------------------------------------------
//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Event
//名称:  ADC单次转换完成事件
//功能:  
//入口:  无              
//出口:  BN_TRUE代表转换完成
bool BSP_ADC1_SampleOnceFinish(void);
//------------------------------E N D-------------------------------------------


#endif

//-----------------------BSP_NTC3380.h--END------------------------------------
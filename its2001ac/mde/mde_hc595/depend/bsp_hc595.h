//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#ifndef _BSP_HC595_H
#define _BSP_HC595_H
//------------------------------E N D-------------------------------------------
//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//------------------------------E N D-------------------------------------------
#include<stdint.h>
#include<stdio.h>
#include<stdbool.h>
//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//名称:hc595硬件配置
//功能:
//入口:无
//出口:无
void BSP_hc595_Configure(void);
//------------------------------E N D-------------------------------------------
//名称:hc595推入数据
void BSP_push_hc595_data(uint32_t in_hc595Data);
//名称:hc595获取数据
uint32_t BSP_pll_hc595_data(void);
//名称:hc595移位存储数据控制
void BSP_hc595_control(void);
//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++

//------------------------------E N D-------------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++

//------------------------------E N D-------------------------------------------
#endif


//-----------------------BSP_Led.h--END------------------------------------

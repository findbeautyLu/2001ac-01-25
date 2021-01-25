//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef _MOD_SWITCHIN_H
#define _MOD_SWITCHIN_H
//-----------------------------------------------------------------------------
#include <stdint.h>
#include <stdbool.h>
//-----------------------------------------------------------------------------
typedef enum
{
    DIGITIN_1  = (uint8_t)0,
    DIGITIN_2,
    DIGITIN_3,
    DIGITIN_4,
}digit_number_t;

//Task
//名称: 4路数字输入判定任务
//功能: 判定高低电平输入
//入口: in_sysTick  1ms时基 主程序调用    
//出口: 
void mde_digit_in_task(void);

//Task
//名称: 获取制定数字输入端口状态
//功能: 判定高低电平输入
//入口: in_sysTick  1ms时基 主程序调用    
//出口: bool  true开关闭合  false开关断开
bool mde_gigit_pull_status(uint8_t in_solidNum);
//------------------------------------------------------------------------------
#endif
//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#ifndef _MOD_HC595_H
#define _MOD_HC595_H
//------------------------------E N D-------------------------------------------
//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#include <stdint.h>
#include <stdbool.h>
//------------------------------E N D-------------------------------------------
//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//Function
//名称: 74HC扫描扫描任务，移位寄存器速率166K
//功能: 驱动LED、步进电机及继电器的输出
//入口: 
//出口: 无
void mde_hc595_task(void);

//名称: 0-31端口开启或者关闭
//功能: 扩展IO，实现响应端口开启或者关闭
//入口: in_port 端口号 0-31  in_status  true 开启 false 关闭
//出口: 无
void mde_hc59c_push_status(uint8_t in_port,bool in_status);
#endif
//-----------------------Mod_LED.h--END-----------------------------------

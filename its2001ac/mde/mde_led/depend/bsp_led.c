//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#include "stm32f10x.h"
#include ".\BSP_led.h"

//------------------------------E N D-------------------------------------------
//#define LED1_DATA    T_BIT4
//#define LED2_DATA    T_BIT5 
//#define LED3_DATA    T_BIT6
//#define LED4_DATA    T_BIT7 

#define LED1_DATA    4
#define LED2_DATA    5 
#define LED3_DATA    6
#define LED4_DATA    7 
void BSP_Led1_Enable(void)
{
    mde_hc59c_push_status(LED1_DATA,false);
}

void BSP_Led1_Disable(void)
{
    mde_hc59c_push_status(LED1_DATA,true);
}
void BSP_Led2_Enable(void)
{
    mde_hc59c_push_status(LED2_DATA,false);
}

void BSP_Led2_Disable(void)
{
    mde_hc59c_push_status(LED2_DATA,true);
}
void BSP_Led3_Enable(void)
{
    mde_hc59c_push_status(LED3_DATA,false);
}

void BSP_Led3_Disable(void)
{
    mde_hc59c_push_status(LED3_DATA,true);
}
void BSP_Led4_Enable(void)
{
    mde_hc59c_push_status(LED4_DATA,false);
}
void BSP_Led4_Disable(void)
{
    mde_hc59c_push_status(LED4_DATA,true);
}
//-----------------------BSP_LED.c--END-----------------------------------

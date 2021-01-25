/***************************************************************************
* Designed by Osman Li <Li.haimeng@menred.com> 
* Copyright , Menred Group Corporation.
* This software is owned by Menred Group and is protected by and subject to 
* worldwide patent protection (china and foreign)
 ***************************************************************************/
 /***************************************************************************
* Release Notes:
*     V1.1  
*         Data: 
*          
*     V1.0  
*         Data:2015.1.26
*         Official release
****************************************************************************/

/*============================ INCLUDES ======================================*/
#include "stm32f10x.h"
#include ".\bsp_digitInput.h"
/*============================ MACROS ========================================*/
#define SWITCH_1_PORT     GPIOB       //左边GPIO
#define SWITCH_1_PIN      GPIO_Pin_9
#define SWITCH_2_PORT     GPIOB       //左边GPIO
#define SWITCH_2_PIN      GPIO_Pin_8
#define SWITCH_3_PORT     GPIOB       //左边GPIO
#define SWITCH_3_PIN      GPIO_Pin_7
#define SWITCH_4_PORT     GPIOB       //左边GPIO
#define SWITCH_4_PIN      GPIO_Pin_6
/*============================ MACROFIED FUNCTIONS ===========================*/
void bsp_switch_in_config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* GPIOA clock enable, */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;            //推挽输出

    GPIO_InitStructure.GPIO_Pin = SWITCH_1_PIN;
    GPIO_Init(SWITCH_1_PORT, &GPIO_InitStructure); 

    GPIO_InitStructure.GPIO_Pin = SWITCH_2_PIN;
    GPIO_Init(SWITCH_2_PORT, &GPIO_InitStructure); 

    GPIO_InitStructure.GPIO_Pin = SWITCH_3_PIN;
    GPIO_Init(SWITCH_3_PORT, &GPIO_InitStructure); 

    GPIO_InitStructure.GPIO_Pin = SWITCH_4_PIN;
    GPIO_Init(SWITCH_4_PORT, &GPIO_InitStructure); 
}
bool BSP_switch1_pull_statsu(void)
{
    if(GPIO_ReadInputDataBit(SWITCH_1_PORT,SWITCH_1_PIN))
    {
        return true;
    }
    return false;
}
bool BSP_switch2_pull_statsu(void)
{
    if(GPIO_ReadInputDataBit(SWITCH_2_PORT,SWITCH_2_PIN))
    {
        return true;
    }
    return false;
}
bool BSP_switch3_pull_statsu(void)
{
    if(GPIO_ReadInputDataBit(SWITCH_3_PORT,SWITCH_3_PIN))
    {
        return true;
    }
    return false;
}
bool BSP_switch4_pull_statsu(void)
{
    if(GPIO_ReadInputDataBit(SWITCH_4_PORT,SWITCH_4_PIN))
    {
        return true;
    }
    return false;
}
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/

/* EOF */

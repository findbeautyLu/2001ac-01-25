//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include ".\snail_data_types.h"
#include "stm32f10x.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//名称:STM32时钟配置函数
//功能:配置晶振、FLASH等待时间、系统时钟、AHB时钟、APB1时钟、APB2时钟
//12M HSE 72M SystemClock
//入口:无,在系统复位后,第一个调用
//出口:无
//-----------------------------------------------------------------------------
void bsp_clock_cfg(void)
{
    sdt_int32u count=0;

    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);  //disable jtag

    __disable_interrupt();
    for(;count<50000;count++)
    { 
        #ifdef NDEBUG
        IWDG_ReloadCounter();
        #endif
        __no_operation();  
    }
    RCC_DeInit();                        //系统时钟复位
    RCC_HSEConfig(RCC_HSE_ON);           //打开晶体振荡器
    if(ERROR==RCC_WaitForHSEStartUp())
    {
        while(1);                           //晶体出错,等待复位
    }
        
    FLASH->ACR |= FLASH_ACR_PRFTBE;                          //开启指令预取功能/* Enable Prefetch Buffer */
    FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);  //指令等待2周期 /* Flash 2 wait state */
    FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_2;    
    
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_6);  //PLL配置
    RCC_PLLCmd(ENABLE);                                 //打开PLL
    while((RCC->CR & RCC_CR_PLLRDY) == 0)               //等待PLL稳定
    {
    }
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);          //切换到PLL时钟
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08)
    {
    }                                           //等待PLL成为系统时钟源
    RCC_HCLKConfig(RCC_SYSCLK_Div1);            //AHB时钟,
    RCC_PCLK1Config(RCC_HCLK_Div2);             //APB1时钟,AHB时钟2分频
    RCC_PCLK2Config(RCC_HCLK_Div1);             //APB2时钟,
//-----------------------------------------------------------------------------
//设置中断向量的指向
//-----------------------------------------------------------------------------
    #ifdef NDEBUG
    NVIC_SetVectorTable(NVIC_VectTab_FLASH,0x00002000);        //中断向量定位到Flash
    #else
    NVIC_SetVectorTable(NVIC_VectTab_FLASH,0x00000000);   
    #endif
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
//-----------------------------------------------------------------------------
    __enable_interrupt();
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

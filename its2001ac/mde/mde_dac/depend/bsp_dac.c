//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "stm32f10x.h"
#include "intrinsics.h"
#include ".\bsp_dac.h"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//DAC输出控制
//分辨率12位
//PA4   DAC_OUT1
//PA5   DAC_OUT2 
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define DAC1_PORT     GPIOA       //左边GPIO
#define DAC1_PIN      GPIO_Pin_4

#define DAC2_PORT     GPIOA       //左边GPIO
#define DAC2_PIN      GPIO_Pin_5

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void bsp_dac1_output(uint8_t value)
{
    uint16_t dacData;
    if(value>100)
    {
        value=100;
    }
    dacData = (((uint32_t)value*4096)/100);
    if(dacData > 4095)
    {
        dacData = 4095;
    }
    DAC_SetChannel1Data(DAC_Align_12b_R,dacData);
    DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void bsp_dac2_output(uint8_t value)
{
    uint16_t dacData;
    if(value>100)
    {
        value=100;
    }
    dacData = (((uint32_t)value*4096)/100);
    if(dacData > 4095)
    {
        dacData = 4095;
    }
    DAC_SetChannel2Data(DAC_Align_12b_R,dacData);
    DAC_SoftwareTriggerCmd(DAC_Channel_2,ENABLE);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void bsp_dac_cfg(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    DAC_InitTypeDef  DAC_InitStructure;
    /* 设置TIM8CLK为72MHZ */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
    /* GPIOC clock enable, Enable AFIO function */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    /* PC6,7,8: Config to PWM output mode */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;            // 复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Pin =  DAC1_PIN;
    GPIO_Init(DAC1_PORT, &GPIO_InitStructure); 
    
    GPIO_InitStructure.GPIO_Pin =  DAC2_PIN;
    GPIO_Init(DAC2_PORT, &GPIO_InitStructure);
    
    DAC_InitStructure.DAC_Trigger = DAC_Trigger_Software;
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
    DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bits11_0;
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
    DAC_Init(DAC_Channel_1,&DAC_InitStructure);
    DAC_Cmd(DAC_Channel_1,ENABLE);
    
    DAC_Init(DAC_Channel_2,&DAC_InitStructure);
    DAC_Cmd(DAC_Channel_2,ENABLE);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
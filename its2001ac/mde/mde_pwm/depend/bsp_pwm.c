//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "stm32f10x.h"
#include "intrinsics.h"
#include ".\bsp_pwm.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//EC马达PWM输出控制
//PWM输出频率 3K
//PC9   TIM8_CH4  
//PC8   TIM8_CH3 
//PC7   TIM8_CH2
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define PWM_FREQENCY   499
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void TIM8CH4_Loading_PwmDutyOut(INT8U PwmData)
{
    if(PwmData>100)
    {
        PwmData=100;
    }
    PwmData=(100-PwmData);
	TIM8->CCR4 = ((PwmData * PWM_FREQENCY )/100);
   // TIM_SetCompare4(TIM8,((PwmData * PWM_FREQENCY )/100));
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void TIM8CH3_Loading_PwmDutyOut(INT8U PwmData)
{
    if(PwmData>100)
    {
        PwmData=100;
    }
    PwmData=(100-PwmData);
	TIM8->CCR3 = ((PwmData * PWM_FREQENCY )/100);
    //TIM_SetCompare3(TIM8,((PwmData * PWM_FREQENCY )/100));
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void TIM8CH2_Loading_PwmDutyOut(INT8U PwmData)
{
    if(PwmData>100)
    {
        PwmData=100;
    }
    PwmData=(100-PwmData);
	TIM8->CCR2 = ((PwmData * PWM_FREQENCY )/100);
   // TIM_SetCompare2(TIM8,((PwmData * PWM_FREQENCY )/100));
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void PwmControl_Configure(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    
    /* 设置TIM8CLK为72MHZ */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE); 
    /* GPIOC clock enable, Enable AFIO function */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
    /* PC6,7,8: Config to PWM output mode */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;            // 复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9 | GPIO_Pin_8 | GPIO_Pin_7;
    GPIO_Init(GPIOC, &GPIO_InitStructure); 
    
//    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//    TIM_OCInitTypeDef  TIM_OCInitStructure;
//    
//    TIM_DeInit(TIM8);
//    
//    /* Time base configuration */    
//    TIM_TimeBaseStructure.TIM_Prescaler = 47;        // prescaler = 47, TIM_CLK = 72MHZ/(47+1) = 1.5MHZ.     
//    TIM_TimeBaseStructure.TIM_Period = PWM_FREQENCY -1 ;         // 当定时器从0计数到999，即为1000次，为一个定时周期
//                                                    // pwm F = 1.5MHZ/(499+1) = 3kHZ.  
//    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;    //设置时钟分频系数：不分频(这里用不到)
//    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
//    TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
//    
//    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;       //配置为PWM模式1
//	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset ;
//    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //当定时器计数值小于CCR1_Val时为高电平
//
//    /* PWM1 Mode configuration: Channel4 */
//    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//    TIM_OCInitStructure.TIM_Pulse = (0 * PWM_FREQENCY )/100;            //设置通道3的电平跳变值，输出另外一个占空比的PWM
//    TIM_OC4Init(TIM8, &TIM_OCInitStructure);                        //使能通道4
//    TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);  
//    
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//    TIM_OCInitStructure.TIM_Pulse = (0 * PWM_FREQENCY )/100;  
//    TIM_OC3Init(TIM8, &TIM_OCInitStructure);                        //使能通道3
//    TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);  
//    
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//    TIM_OCInitStructure.TIM_Pulse = (0 * PWM_FREQENCY )/100;  
//    TIM_OC2Init(TIM8, &TIM_OCInitStructure);                        //使能通道2
//    TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);  
//    
//    TIM_ARRPreloadConfig(TIM8, ENABLE);                                // 使能TIM8重载寄存器ARR
//
//    /* TIM8 enable counter */
//    TIM_Cmd(TIM8, ENABLE);                   // 使能定时器8     
//    TIM_CtrlPWMOutputs(TIM8, ENABLE);        // 注意： 配置定时器8的PWM模式，必须加上这句话！！
//--------------------------------------------------------------------------------
	TIM8->ARR=PWM_FREQENCY -1;             //设定计数器自动重装值
 	
    TIM8->PSC=47;             //预分频器不分频
 	
    TIM8->CCMR1|=6<<12;      //CH2 PWM1模式
 	TIM8->CCMR2|=6<<12;      //CH2 PWM4模式
	TIM8->CCMR2|=6<<4;      //CH2 PWM3模式
   // TIM8->CCMR1|=6<<4;       //CH1 PWM1模式
 
    TIM8->CCMR1|=1<<11;      //CH2预装载使能  
 	TIM8->CCMR2|=1<<11;      //CH4预装载使能  
	TIM8->CCMR2|=1<<3;      //CH3预装载使能  
  //  TIM8->CCMR1|=1<<3;       //CH1预装载使能  
 
    TIM8->CCER|=1<<4;       //CH2输出使能    
 	TIM8->CCER|=1<<8;       //CH3输出使能    
	TIM8->CCER|=1<<12;      //CH4输出使能    
   // TIM8->CCER|=1<<0;        //CH1输出使能
 
    TIM8->BDTR |= 1<<15;     //TIM8必须要这句话才能输出PWM
 
    TIM8->CR1=0x80;        //ARPE使能
 
    TIM8->CR1|=0x01;         //使能定时器8 
//-----------------------------------------------------------------------------
    TIM8CH4_Loading_PwmDutyOut(0);
    TIM8CH3_Loading_PwmDutyOut(0);
    TIM8CH2_Loading_PwmDutyOut(0);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
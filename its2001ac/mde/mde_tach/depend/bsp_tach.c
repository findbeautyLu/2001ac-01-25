//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "stm32f10x.h"
#include "intrinsics.h"
#include ".\bsp_tach.h"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//tach pin
//PB15  --- EXIT8  --- in air  
//PB14  --- EXIT10 --- out air 

//TIM5 --- 
//---------------------------------------------------------------------------------------------------
//测量方法,收集10s的脉冲数量，计算rpm值
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define MAX_NUM 3
typedef enum
{
    read_pulseW_low,
    read_pulseW_high,
}read_pulseW_def;

typedef struct
{
    INT8U  air_pulse_gather_index;  
    bool   phyReadInputStatsu;
    INT8U  air_pulse_width;
    INT16U this_air_pulse;
    INT16U now_tach_Fan;
    INT16U air_pulse_gather_cnt[10];
    read_pulseW_def read_pulseW_air;
}air_pulse_gather_def;

//--------------------------------------------------------------------------------------------
static volatile air_pulse_gather_def air_pulse_gather[MAX_NUM];
static volatile BOOLEAN new_data;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void bsp_tachFan_cfg(void)
{   
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
/* Enable GPIOA clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    /* Configure PA.08 pin as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
//------------------------------------------------------------------------------------------
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;              //TIM5 
//------------------------------------------------------------------------------------------ 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); 
//------------------------------------------------------------------------------------------
    TIM_DeInit(TIM5);
    /* Time base configuration */    
    TIM_TimeBaseStructure.TIM_Prescaler =(720-1);                    //72M 100K  
    TIM_TimeBaseStructure.TIM_Period =50-1  ;                    // 当定时器从0计数到999，即为1000次，为一个定时周期,1个unit 10us, 
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;        //设置时钟分频系数：2分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     //向上计数模式
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM5,TIM_FLAG_Update);
    TIM_Cmd(TIM5,ENABLE); 
//--------------------------------------------------------------------------------------------
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);  
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;       
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;   
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);  
//-------------------------------------------------------------------------------------------
    TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
    TIM_ClearFlag(TIM5,TIM_FLAG_Update);               //清除溢出中断标志  
    TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);  
    TIM_Cmd(TIM5,ENABLE);                              //开启时钟  
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//--------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
//500us
void TIM5_IRQHandler(void)
{
    static INT32U timer_1s;
    uint8_t i = 0;

    if(SET==TIM_GetFlagStatus(TIM5,TIM_FLAG_Update))
    {
        TIM_ClearFlag(TIM5,TIM_FLAG_Update);  
        for(i = 0; i < MAX_NUM;i++)
        {
            air_pulse_gather[0].phyReadInputStatsu = (GPIOB->IDR & 0x8000);
            air_pulse_gather[1].phyReadInputStatsu = (GPIOB->IDR & 0x4000);
            air_pulse_gather[2].phyReadInputStatsu = (GPIOB->IDR & 0x2000);
            if(air_pulse_gather[i].phyReadInputStatsu)
            {
                if(read_pulseW_high == air_pulse_gather[i].read_pulseW_air)
                {
                    air_pulse_gather[i].air_pulse_width ++;
                    if(air_pulse_gather[i].air_pulse_width > 4) //2.5ms
                    {
                        air_pulse_gather[i].this_air_pulse ++;
                        air_pulse_gather[i].air_pulse_width = 0;
                        air_pulse_gather[i].read_pulseW_air = read_pulseW_low;
                    }
                }
                else
                {
                    air_pulse_gather[i].air_pulse_width = 0;
                }
            }
            else
            {
                if(read_pulseW_low == air_pulse_gather[i].read_pulseW_air)
                {
                    air_pulse_gather[i].air_pulse_width ++;
                    if(air_pulse_gather[i].air_pulse_width > 4)
                    {
                        air_pulse_gather[i].air_pulse_width = 0;
                        air_pulse_gather[i].read_pulseW_air = read_pulseW_high;
                    }
                }
                else
                {
                    air_pulse_gather[i].air_pulse_width = 0;
                }
            }          
        }     
        timer_1s ++;
        if(timer_1s > (2000-1))
        {
            timer_1s = 0;
            
            INT8U i;
            
            for(i = 0;i < 9;i ++)
            {
                air_pulse_gather[0].air_pulse_gather_cnt[i] = air_pulse_gather[0].air_pulse_gather_cnt[i+1];
                air_pulse_gather[1].air_pulse_gather_cnt[i] = air_pulse_gather[1].air_pulse_gather_cnt[i+1];
                air_pulse_gather[2].air_pulse_gather_cnt[i] = air_pulse_gather[2].air_pulse_gather_cnt[i+1];
            }
            air_pulse_gather[0].air_pulse_gather_cnt[9] = air_pulse_gather[0].this_air_pulse;
            air_pulse_gather[1].air_pulse_gather_cnt[9] = air_pulse_gather[1].this_air_pulse;
            air_pulse_gather[2].air_pulse_gather_cnt[9] = air_pulse_gather[2].this_air_pulse;
            air_pulse_gather[0].this_air_pulse = 0;
            air_pulse_gather[1].this_air_pulse = 0;
            air_pulse_gather[2].this_air_pulse = 0;
            new_data = BN_TRUE;
        }
    }
}
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
void bsp_fan_tach_task(void)
{
    if(new_data)
    {
        new_data = BN_FALSE;
        INT8U i,j;

        INT8U valid_dc = 0;
        INT32U Fan_ammount[MAX_NUM] = {0,0,0};
        
        for(j = 0;j< MAX_NUM;j++)
        {
            if((0 == air_pulse_gather[j].air_pulse_gather_cnt[8])&&(0 == air_pulse_gather[j].air_pulse_gather_cnt[9]))
            {
                air_pulse_gather[j].now_tach_Fan = 0;
            }
            else
            {
                for(i = 0;i < 10;i ++)
                {
                    if((0 != air_pulse_gather[j].air_pulse_gather_cnt[i]) || (0 != valid_dc))
                    {
                        valid_dc++;
                        Fan_ammount[j] += air_pulse_gather[j].air_pulse_gather_cnt[i];
                    }
                }
                if(0 == valid_dc)
                {
                    air_pulse_gather[j].now_tach_Fan = 0;
                }
                else
                {
                    air_pulse_gather[j].now_tach_Fan = Fan_ammount[j]*60/(valid_dc);
                }
            }      
            valid_dc = 0;
        }
    }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
uint16_t bsp_pull_inair_tach(uint8_t in_solidNum)
{
    return air_pulse_gather[in_solidNum].now_tach_Fan;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
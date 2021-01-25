//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#include "stm32f10x.h"
//------------------------------E N D-------------------------------------------
/*4片595  最左边为31-24QA为31，最右边为7-0从QA为7*/
#define SN595_RCLK_PORT     GPIOA       //左边GPIO
#define SN595_RCLK_PIN      GPIO_Pin_7
#define SN595_SRCLK_PORT    GPIOC       //左边GPIO
#define SN595_SRCLK_PIN     GPIO_Pin_5
#define SN595_DATA_PORT     GPIOB       //左边GPIO
#define SN595_DATA_PIN      GPIO_Pin_0

#define SN595_RESET_PORT     GPIOC      //左边GPIO
#define SN595_RESET_PIN      GPIO_Pin_4
//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//宏定义背光IO管脚寄存器宏定义
//------------------------------E N D-------------------------------------------
static uint32_t hc595Data = 0;
void BSP_push_hc595_data(uint32_t in_hc595Data)
{
    hc595Data = in_hc595Data;
}
uint32_t BSP_pll_hc595_data(void)
{
    return hc595Data;
}
//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//配置PG2口为推挽,初始化为低
void BSP_hc595_Configure(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* GPIOA clock enable, */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB, ENABLE);   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;            //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Pin = SN595_RCLK_PIN;
    GPIO_Init(SN595_RCLK_PORT, &GPIO_InitStructure); 
    
    GPIO_InitStructure.GPIO_Pin = SN595_SRCLK_PIN;
    GPIO_Init(SN595_SRCLK_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = SN595_DATA_PIN;
    GPIO_Init(SN595_DATA_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = SN595_RESET_PIN;
    GPIO_Init(SN595_RESET_PORT, &GPIO_InitStructure);

    GPIO_ResetBits(SN595_RCLK_PORT,SN595_RCLK_PIN); 
    GPIO_ResetBits(SN595_SRCLK_PORT,SN595_SRCLK_PIN); 
    GPIO_ResetBits(SN595_DATA_PORT,SN595_DATA_PIN); 
    GPIO_SetBits(SN595_RESET_PORT,SN595_RESET_PIN); 

}
//------------------------------E N D-------------------------------------------
void BSP_hc595_control(void)
{
    uint8_t i;
    uint32_t temp;
    uint32_t tempData;
    tempData = hc595Data;
    for(i = 0;i < 32;i++)
    {
        GPIO_ResetBits(SN595_SRCLK_PORT,SN595_SRCLK_PIN);
        temp = (tempData & 0x0001);
        if(temp)
        {     
            GPIO_SetBits(SN595_DATA_PORT,SN595_DATA_PIN);         
        }
        else
        {
            GPIO_ResetBits(SN595_DATA_PORT,SN595_DATA_PIN);
        }
        tempData >>=1;
        GPIO_SetBits(SN595_SRCLK_PORT,SN595_SRCLK_PIN);//上升沿发生移位
    }
    GPIO_ResetBits(SN595_RCLK_PORT,SN595_RCLK_PIN);
    uint16_t j = 100;
    while(j--);
    GPIO_SetBits(SN595_RCLK_PORT,SN595_RCLK_PIN);//上升沿将数据送到输出锁存器
}


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++

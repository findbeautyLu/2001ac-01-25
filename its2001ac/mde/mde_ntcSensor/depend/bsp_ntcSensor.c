#include ".\BSP_ntcSensor.h"
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************内部变量定义***************************************
 //宏定义NTC IO管脚寄存器宏定义
#define NTC1_PIN                            GPIO_Pin_0
#define NTC1_GPIO_PORT                      GPIOC

#define NTC2_PIN                            GPIO_Pin_1
#define NTC2_GPIO_PORT                      GPIOC

#define NTC3_PIN                            GPIO_Pin_2
#define NTC3_GPIO_PORT                      GPIOC

#define NTC4_PIN                            GPIO_Pin_3
#define NTC4_GPIO_PORT                      GPIOC

#define NTC5_PIN                            GPIO_Pin_0
#define NTC5_GPIO_PORT                      GPIOA

#define NTC6_PIN                            GPIO_Pin_1
#define NTC6_GPIO_PORT                      GPIOA
//------------------------------E N D-------------------------------------------

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************PC4口宏定义****************************************
//------------------------------E N D-------------------------------------------
//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**********************ADC_IO口配置为浮空输入**********************************
void BSP_ADC_Hardware_Configure(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    /* Enable ADC1 and GPIOC clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3 | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Pin = NTC1_PIN;
    GPIO_Init(NTC1_GPIO_PORT, &GPIO_InitStructure);				// PC0,输入时不用设置速率
    GPIO_InitStructure.GPIO_Pin = NTC2_PIN;
    GPIO_Init(NTC2_GPIO_PORT, &GPIO_InitStructure);				// PC0,输入时不用设置速率
    GPIO_InitStructure.GPIO_Pin = NTC3_PIN;
    GPIO_Init(NTC3_GPIO_PORT, &GPIO_InitStructure);				// PC0,输入时不用设置速率
    GPIO_InitStructure.GPIO_Pin = NTC4_PIN;
    GPIO_Init(NTC4_GPIO_PORT, &GPIO_InitStructure);				// PC0,输入时不用设置速率
    GPIO_InitStructure.GPIO_Pin = NTC5_PIN;
    GPIO_Init(NTC5_GPIO_PORT, &GPIO_InitStructure);				// PC0,输入时不用设置速率
    GPIO_InitStructure.GPIO_Pin = NTC6_PIN;
    GPIO_Init(NTC6_GPIO_PORT, &GPIO_InitStructure);				// PC0,输入时不用设置速率
    /* Configure PC.0  as analog input */  
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************ADC软件配置****************************************
void BSP_ADC_Software_Configure(void)
{
    ADC_InitTypeDef ADC_InitStructure;

    BSP_ADC_Hardware_Configure();

    /* ADC3 configuration */	
    ADC_DeInit(ADC3);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;			//独立ADC模式
    ADC_InitStructure.ADC_ScanConvMode = DISABLE ; 	 				//扫描模式用于多通道采集
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;			//单次采集
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//不使用外部触发转换
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 	//采集数据右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 1;	 								//要转换的通道数目1
    ADC_Init(ADC3, &ADC_InitStructure);
    
    /*配置ADC时钟，为PCLK2的8分频，即9MHz*/
    RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
    /*配置ADC1的通道11为55.	5个采样周期，序列为1 */ 
    ADC_RegularChannelConfig(ADC3, ADC_Channel_10, 1, ADC_SampleTime_55Cycles5);
    
    ADC_ClearFlag(ADC3,ADC_FLAG_EOC);

    /* Enable ADC1 */
    ADC_Cmd(ADC3, ENABLE);
    
    /*复位校准寄存器 */   
    ADC_ResetCalibration(ADC3);
    /*等待校准寄存器复位完成 */
    while(ADC_GetResetCalibrationStatus(ADC3));
    
    /* ADC校准 */
    ADC_StartCalibration(ADC3);
    /* 等待校准完成*/
    while(ADC_GetCalibrationStatus(ADC3));

    
    /* 由于没有采用外部触发，所以使用软件触发ADC转换 */ 
    ADC_SoftwareStartConvCmd(ADC3, ENABLE);
}
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//*************************ADC初始化配置****************************************
void BSP_ADC_Configure(void)
{
     BSP_ADC_Hardware_Configure();
     BSP_ADC_Software_Configure();
}
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************ADC通道选择****************************************
//Function
//名称:  获取单次ADC值
//功能:  
//入口:  无
//出口:  INT16U 的ADC值
 
//------------------------------E N D-------------------------------------------
uint16_t BSP_ADC1_GetValue(void)
{
    return(ADC3->DR);
}

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***************************ADC开启转换****************************************
void BSP_ADC12_Start(void)
{
    ADC_SoftwareStartConvCmd(ADC3, ENABLE);
}
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//***********************读取单次转换完成事件***********************************
 

bool BSP_ADC1_SampleOnceFinish(void)
{
    if(ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC ))
    {
        return true;
    }
    return false;
}
//------------------------------E N D-------------------------------------------
void BSP_ADC_ChannalSelect(uint8_t _In_Channel)
{
    ADC_RegularChannelConfig(ADC3,_In_Channel,1,ADC_SampleTime_55Cycles5);
}


void BSP_ADC_Channal_one(void)
{
    BSP_ADC_ChannalSelect(ADC_Channel_10);
}

void BSP_ADC_Channal_two(void)
{
    BSP_ADC_ChannalSelect(ADC_Channel_11);
}
void BSP_ADC_Channal_three(void)
{
    BSP_ADC_ChannalSelect(ADC_Channel_12);
}

void BSP_ADC_Channal_four(void)
{
    BSP_ADC_ChannalSelect(ADC_Channel_13);
}
void BSP_ADC_Channal_five(void)
{
    BSP_ADC_ChannalSelect(ADC_Channel_0);
}

void BSP_ADC_Channal_six(void)
{
    BSP_ADC_ChannalSelect(ADC_Channel_1);
}
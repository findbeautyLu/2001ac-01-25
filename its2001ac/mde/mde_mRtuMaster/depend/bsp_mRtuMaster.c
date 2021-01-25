//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "stm32f10x.h"
#include ".\snail_macro.h"
#include ".\snail_data_types.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*串口2*/
//uart2 txd--PA2  rxd--PA3
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
macro_creat_queueBasce(uart2_queue_rxd,64);
macro_creat_queueBasce(uart2_queue_txd,255);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//中断服务函数
//-----------------------------------------------------------------------------
void USART2_IRQHandler(void)
{
    if((SET==USART_GetFlagStatus(USART2,USART_FLAG_ORE))||\
       (SET==USART_GetFlagStatus(USART2,USART_FLAG_NE))||\
       (SET==USART_GetFlagStatus(USART2,USART_FLAG_FE)))
    {
        USART_GetFlagStatus(USART2,USART_FLAG_ORE);
        USART_ReceiveData(USART2);
    }
    else if(SET==USART_GetFlagStatus(USART2,USART_FLAG_PE))
    {
        USART_GetFlagStatus(USART2,USART_FLAG_PE);
        USART_ReceiveData(USART2);
    }
    else if(SET==USART_GetFlagStatus(USART2,USART_FLAG_RXNE))
    {
        sdt_int8u Read_reg;
        Read_reg = USART_ReceiveData(USART2);
       
        sdt_int8u n_bytes;
        macro_pull_queueBasce_bytes(uart2_queue_rxd,n_bytes);

        sdt_bool queunIsFull;
        macro_pull_queueBasce_full(uart2_queue_rxd,n_bytes,queunIsFull);
        if(queunIsFull)
        {
        }
        else
        {
            macro_push_queueBasce_data(uart2_queue_rxd,n_bytes,Read_reg);
        }
    }
    if(SET==USART_GetITStatus(USART2,USART_IT_TXE))
    {
        if(SET==USART_GetFlagStatus(USART2,USART_FLAG_TXE))
        {
            sdt_int8u n_bytes,rd_byte_details;
            macro_pull_queueBasce_bytes(uart2_queue_txd,n_bytes);
            if(n_bytes)
            {
                macro_pull_queueBasce_data(uart2_queue_txd,n_bytes,rd_byte_details);
                USART_SendData(USART2,rd_byte_details);
            }
            else
            {
                USART_ITConfig(USART2,USART_IT_TXE,DISABLE);
            }
        }
    }
}
//#endif
////+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++interface++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------
void bsp_uart2_cfg(void)
{
    GPIO_InitTypeDef    GPIO_USART2INIT; 
    USART_InitTypeDef   USART2_INIT;
    NVIC_InitTypeDef    USART2_NVIC_INIT;

 //-----------------------------------------------------------------------------   
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);  //开启串口时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);   //开启GPIOA口时钟
//-----------------------------------------------------------------------------  
    GPIO_USART2INIT.GPIO_Pin=GPIO_Pin_2;                   //PA2 U2_TXD
    GPIO_USART2INIT.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_USART2INIT.GPIO_Mode=GPIO_Mode_AF_PP;            //输出复用推挽
    GPIO_Init(GPIOA,&GPIO_USART2INIT);
    GPIO_SetBits(GPIOA,GPIO_Pin_2);
//-----------------------------------------------------------------------------     
    GPIO_USART2INIT.GPIO_Pin=GPIO_Pin_3;                   //PA3 U2_RXD
    GPIO_USART2INIT.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_USART2INIT.GPIO_Mode=GPIO_Mode_IPU;               //输入上拉
    GPIO_Init(GPIOA,&GPIO_USART2INIT);
//-----------------------------------------------------------------------------  
    GPIO_USART2INIT.GPIO_Pin=GPIO_Pin_6;                   //PA6 TRSEL
    GPIO_USART2INIT.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_USART2INIT.GPIO_Mode=GPIO_Mode_Out_PP;            //推挽输出
    GPIO_Init(GPIOA,&GPIO_USART2INIT);
    GPIO_ResetBits(GPIOA,GPIO_Pin_6);

//-----------------------------------------------------------------------------   
    USART_DeInit(USART2);
    USART2_INIT.USART_BaudRate=9600;
    USART2_INIT.USART_Parity=USART_Parity_No;
    USART2_INIT.USART_WordLength=USART_WordLength_8b;    

    USART2_INIT.USART_StopBits=USART_StopBits_1;
    USART2_INIT.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    USART2_INIT.USART_Mode=(USART_Mode_Rx+USART_Mode_Tx);  //收发使能
//-----------------------------------------------------------------------------
    USART_Init(USART2,&USART2_INIT);
    USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);           //使能接收中断
    USART_Cmd(USART2,ENABLE);
//-----------------------------------------------------------------------------
    USART2_NVIC_INIT.NVIC_IRQChannel=USART2_IRQn;
    USART2_NVIC_INIT.NVIC_IRQChannelPreemptionPriority=0;  //主优先级
    USART2_NVIC_INIT.NVIC_IRQChannelSubPriority=0;         //子优先级
    USART2_NVIC_INIT.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&USART2_NVIC_INIT);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;              //TIM4 3.5T的时序管理
//----------------------------------------------------------------------------- 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
//-----------------------------------------------------------------------------
    TIM_DeInit(TIM2);
    /* Time base configuration */    
    TIM_TimeBaseStructure.TIM_Prescaler =(72*2)-1;                    //72M 500k ,2us
    TIM_TimeBaseStructure.TIM_Period =0xffff  ;                      // 当定时器从0计数到999，即为1000次，为一个定时周期,1个unit 2us
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;        //设置时钟分频系数：不分频(这里用不到)
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     //向上计数模式
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
//-----------------------------------------------------------------------------
    TIM_Cmd(TIM2,ENABLE);                              //开启时钟  
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//name:获取一个字节的接收数据
//out: sdt_true 获取成功，sdt_falas 获取失败
//-----------------------------------------------------------------------------
sdt_bool bsp_pull_oneByte_uart2_rxd(sdt_int8u* out_byte_details)
{
    sdt_int8u n_bytes,rd_byte_details;
    __disable_interrupt();
    macro_pull_queueBasce_bytes(uart2_queue_rxd,n_bytes);
    __enable_interrupt();
    if(n_bytes)
    {
        __disable_interrupt();
        macro_pull_queueBasce_data(uart2_queue_rxd,n_bytes,rd_byte_details);
        __enable_interrupt();
        *out_byte_details = rd_byte_details;
        return(sdt_true);
    }
    else
    {
        
        return(sdt_false);
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//name:压入一个字节的数据到发送队列
//out: sdt_true 压入成功，sdt_false 压入失败
//-----------------------------------------------------------------------------
sdt_bool bsp_push_oneByte_uart2_txd(sdt_int8u in_byte_details)
{
    sdt_int8u n_bytes;
    __disable_interrupt();
    macro_pull_queueBasce_bytes(uart2_queue_txd,n_bytes);
    __enable_interrupt();
    sdt_bool queunIsFull;
    macro_pull_queueBasce_full(uart2_queue_txd,n_bytes,queunIsFull);
    if(queunIsFull)
    {
        return(sdt_false);
    }
    else
    {
        __disable_interrupt();
        macro_push_queueBasce_data(uart2_queue_txd,n_bytes,in_byte_details);
        __enable_interrupt();
        USART_ITConfig(USART2,USART_IT_TXE,ENABLE);
        return(sdt_true);
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// static sdt_int16u test[32];
// static sdt_int8u test_idx = 0;
//-----------------------------------------------------------------------------
sdt_bool bsp_uart2_busFree(sdt_int8u t_char_dis)
{
    //static sdt_int16u rd_rxd_depart_cnt;
    static  sdt_int16u rd_cnt;
    static  sdt_int16u free_cnt;

    rd_cnt = TIM2->CNT;
    free_cnt = rd_cnt;

    sdt_int16u limit_ft;
    
    limit_ft = 104*t_char_dis/2;  //57600 T == 17us
    if(free_cnt > limit_ft)      //9600 3.5T 1040*3.25/2
    {
        return(sdt_true);
    }
    else
    {
        return(sdt_false);
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void bsp_restart_tim2(void)
{
   TIM2->CNT = 0;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
sdt_bool bsp_pull_uart2_txd_cmp(void)
{
    sdt_int8u n_bytes;
    
    macro_pull_queueBasce_bytes(uart2_queue_txd,n_bytes);
    if(n_bytes)
    {
        return(sdt_false);
    }
    else
    {
        if(SET==USART_GetITStatus(USART2,USART_IT_TXE))
        {
            return(sdt_false);
        }
        else
        {
            return(sdt_true);
        }      
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void bps_uart2_into_receive(void)
{
    GPIO_ResetBits(GPIOA,GPIO_Pin_6);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void bps_uart2_into_transmit(void)
{
    GPIO_SetBits(GPIOA,GPIO_Pin_6);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void bsp_uart2_phyReCfg(sdt_int32u sys_fre,sdt_int32u baud_rate,sdt_int8u parity,sdt_int8u stop_bits)
{
    USART_InitTypeDef   USART2_INIT;

    USART_DeInit(USART2);
    USART2_INIT.USART_BaudRate=9600;
    USART2_INIT.USART_Parity=USART_Parity_No;
    USART2_INIT.USART_WordLength=USART_WordLength_8b;    

    USART2_INIT.USART_StopBits=USART_StopBits_1;
    USART2_INIT.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    USART2_INIT.USART_Mode=(USART_Mode_Rx+USART_Mode_Tx);  //收发使能
//-----------------------------------------------------------------------------
    USART_Init(USART2,&USART2_INIT);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*串口5*/
//uart5 txd--PC12  rxd--PD2
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
macro_creat_queueBasce(uart5_queue_rxd,64);
macro_creat_queueBasce(uart5_queue_txd,255);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//中断服务函数
//-----------------------------------------------------------------------------
void UART5_IRQHandler(void)
{
    if((SET==USART_GetFlagStatus(UART5,USART_FLAG_ORE))||\
       (SET==USART_GetFlagStatus(UART5,USART_FLAG_NE))||\
       (SET==USART_GetFlagStatus(UART5,USART_FLAG_FE)))
    {
        USART_GetFlagStatus(UART5,USART_FLAG_ORE);
        USART_ReceiveData(UART5);
    }
    else if(SET==USART_GetFlagStatus(UART5,USART_FLAG_PE))
    {
        USART_GetFlagStatus(UART5,USART_FLAG_PE);
        USART_ReceiveData(UART5);
    }
    else if(SET==USART_GetFlagStatus(UART5,USART_FLAG_RXNE))
    {
        sdt_int8u Read_reg;
        Read_reg = USART_ReceiveData(UART5);
       
        sdt_int8u n_bytes;
        macro_pull_queueBasce_bytes(uart5_queue_rxd,n_bytes);

        sdt_bool queunIsFull;
        macro_pull_queueBasce_full(uart5_queue_rxd,n_bytes,queunIsFull);
        if(queunIsFull)
        {
        }
        else
        {
            macro_push_queueBasce_data(uart5_queue_rxd,n_bytes,Read_reg);
        }
    }
    if(SET==USART_GetITStatus(UART5,USART_IT_TXE))
    {
        if(SET==USART_GetFlagStatus(UART5,USART_FLAG_TXE))
        {
            sdt_int8u n_bytes,rd_byte_details;
            macro_pull_queueBasce_bytes(uart5_queue_txd,n_bytes);
            if(n_bytes)
            {
                macro_pull_queueBasce_data(uart5_queue_txd,n_bytes,rd_byte_details);
                USART_SendData(UART5,rd_byte_details);
            }
            else
            {
                USART_ITConfig(UART5,USART_IT_TXE,DISABLE);
            }
        }
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++interface++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------
void bsp_uart5_cfg(void)
{
    GPIO_InitTypeDef    GPIO_USART5INIT; 
    USART_InitTypeDef   USART5_INIT;
    NVIC_InitTypeDef    USART5_NVIC_INIT;

 //-----------------------------------------------------------------------------   
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);  //开启串口时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);   //开启GPIOC口时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);   //开启GPIOB口时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);   //开启GPIOD口时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);   //开启GPIOA口时钟
//-----------------------------------------------------------------------------  
    GPIO_USART5INIT.GPIO_Pin=GPIO_Pin_12;                   //PC12 U5_TXD
    GPIO_USART5INIT.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_USART5INIT.GPIO_Mode=GPIO_Mode_AF_PP;            //输出复用推挽
    GPIO_Init(GPIOC,&GPIO_USART5INIT);
//-----------------------------------------------------------------------------     
    GPIO_USART5INIT.GPIO_Pin=GPIO_Pin_2;                   //PD2 U5_RXD
    GPIO_USART5INIT.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_USART5INIT.GPIO_Mode=GPIO_Mode_IPU;               //输入上拉
    GPIO_Init(GPIOD,&GPIO_USART5INIT);
//-----------------------------------------------------------------------------  
    GPIO_USART5INIT.GPIO_Pin=GPIO_Pin_3;                   //PB3 TRSEL
    GPIO_USART5INIT.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_USART5INIT.GPIO_Mode=GPIO_Mode_Out_PP;            //推挽输出
    GPIO_Init(GPIOB,&GPIO_USART5INIT);
    GPIO_ResetBits(GPIOB,GPIO_Pin_3);
//-----------------------------------------------------------------------------   
    USART_DeInit(UART5);
    USART5_INIT.USART_BaudRate=9600;
    USART5_INIT.USART_Parity=USART_Parity_No;
    USART5_INIT.USART_WordLength=USART_WordLength_8b;    

    USART5_INIT.USART_StopBits=USART_StopBits_1;
    USART5_INIT.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    USART5_INIT.USART_Mode=(USART_Mode_Rx+USART_Mode_Tx);  //收发使能
//-----------------------------------------------------------------------------
    USART_Init(UART5,&USART5_INIT);
    USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);           //使能接收中断
    USART_Cmd(UART5,ENABLE);
//-----------------------------------------------------------------------------
    USART5_NVIC_INIT.NVIC_IRQChannel=UART5_IRQn;
    USART5_NVIC_INIT.NVIC_IRQChannelPreemptionPriority=0;  //主优先级
    USART5_NVIC_INIT.NVIC_IRQChannelSubPriority=0;         //子优先级
    USART5_NVIC_INIT.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&USART5_NVIC_INIT);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;              //TIM4 3.5T的时序管理
//----------------------------------------------------------------------------- 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); 
//-----------------------------------------------------------------------------
    TIM_DeInit(TIM6);
    /* Time base configuration */    
    TIM_TimeBaseStructure.TIM_Prescaler =(72*2)-1;                    //72M 500k ,2us
    TIM_TimeBaseStructure.TIM_Period =0xffff  ;                      // 当定时器从0计数到999，即为1000次，为一个定时周期,1个unit 2us
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;        //设置时钟分频系数：不分频(这里用不到)
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     //向上计数模式
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
//-----------------------------------------------------------------------------
    TIM_Cmd(TIM6,ENABLE);                              //开启时钟  
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//name:获取一个字节的接收数据
//out: sdt_true 获取成功，sdt_falas 获取失败
//-----------------------------------------------------------------------------
sdt_bool bsp_pull_oneByte_uart5_rxd(sdt_int8u* out_byte_details)
{
    sdt_int8u n_bytes,rd_byte_details;
    __disable_interrupt();
    macro_pull_queueBasce_bytes(uart5_queue_rxd,n_bytes);
    __enable_interrupt();
    if(n_bytes)
    {
        __disable_interrupt();
        macro_pull_queueBasce_data(uart5_queue_rxd,n_bytes,rd_byte_details);
        __enable_interrupt();
        *out_byte_details = rd_byte_details;
        return(sdt_true);
    }
    else
    {
        
        return(sdt_false);
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//name:压入一个字节的数据到发送队列
//out: sdt_true 压入成功，sdt_false 压入失败
//-----------------------------------------------------------------------------
sdt_bool bsp_push_oneByte_uart5_txd(sdt_int8u in_byte_details)
{
    sdt_int8u n_bytes;
    __disable_interrupt();
    macro_pull_queueBasce_bytes(uart5_queue_txd,n_bytes);
    __enable_interrupt();
    sdt_bool queunIsFull;
    macro_pull_queueBasce_full(uart5_queue_txd,n_bytes,queunIsFull);
    if(queunIsFull)
    {
        return(sdt_false);
    }
    else
    {
        __disable_interrupt();
        macro_push_queueBasce_data(uart5_queue_txd,n_bytes,in_byte_details);
        __enable_interrupt();
        USART_ITConfig(UART5,USART_IT_TXE,ENABLE);
        return(sdt_true);
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// static sdt_int16u test[32];
// static sdt_int8u test_idx = 0;
//-----------------------------------------------------------------------------
sdt_bool bsp_uart5_busFree(sdt_int8u t_char_dis)
{
    //static sdt_int16u rd_rxd_depart_cnt;
    static  sdt_int16u rd_cnt;
    static  sdt_int16u free_cnt;

    rd_cnt = TIM6->CNT;
    free_cnt = rd_cnt;

    sdt_int16u limit_ft;
    
    limit_ft = 104*t_char_dis/2;  //57600 T == 17us
    if(free_cnt > limit_ft)      //9600 3.5T 1040*3.25/2
    {
        return(sdt_true);
    }
    else
    {
        return(sdt_false);
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void bsp_restart_tim6(void)
{
   TIM6->CNT = 0;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
sdt_bool bsp_pull_uart5_txd_cmp(void)
{
    sdt_int8u n_bytes;
    
    macro_pull_queueBasce_bytes(uart5_queue_txd,n_bytes);
    if(n_bytes)
    {
        return(sdt_false);
    }
    else
    {
        if(SET==USART_GetITStatus(UART5,USART_IT_TXE))
        {
            return(sdt_false);
        }
        else
        {
            return(sdt_true);
        }      
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void bps_uart5_into_receive(void)
{
    GPIO_ResetBits(GPIOB,GPIO_Pin_3);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void bps_uart5_into_transmit(void)
{
    GPIO_SetBits(GPIOB,GPIO_Pin_3);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void bsp_uart5_phyReCfg(sdt_int32u sys_fre,sdt_int32u baud_rate,sdt_int8u parity,sdt_int8u stop_bits)
{
    USART_InitTypeDef   USART5_INIT;

    USART_DeInit(UART5);
    USART5_INIT.USART_BaudRate=9600;
    USART5_INIT.USART_Parity=USART_Parity_No;
    USART5_INIT.USART_WordLength=USART_WordLength_8b;    

    USART5_INIT.USART_StopBits=USART_StopBits_1;
    USART5_INIT.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    USART5_INIT.USART_Mode=(USART_Mode_Rx+USART_Mode_Tx);  //收发使能
//-----------------------------------------------------------------------------
    USART_Init(UART5,&USART5_INIT);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
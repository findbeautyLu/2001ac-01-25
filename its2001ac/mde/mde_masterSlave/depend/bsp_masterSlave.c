//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "stm32f10x.h"
#include ".\snail_macro.h"
#include ".\snail_data_types.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*串口4*/
//uart4 txd--PC10  rxd--PC11
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
macro_creat_queueBasce(uart4_queue_rxd,100);
macro_creat_queueBasce(uart4_queue_txd,255);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//中断服务函数
//-----------------------------------------------------------------------------
void UART4_IRQHandler(void)
{
    if((SET==USART_GetFlagStatus(UART4,USART_FLAG_ORE))||\
       (SET==USART_GetFlagStatus(UART4,USART_FLAG_NE))||\
       (SET==USART_GetFlagStatus(UART4,USART_FLAG_FE)))
    {
        USART_GetFlagStatus(UART4,USART_FLAG_ORE);
        USART_ReceiveData(UART4);
    }
    else if(SET==USART_GetFlagStatus(UART4,USART_FLAG_PE))
    {
        USART_GetFlagStatus(UART4,USART_FLAG_PE);
        USART_ReceiveData(UART4);
    }
    else if(SET==USART_GetFlagStatus(UART4,USART_FLAG_RXNE))
    {
        sdt_int8u Read_reg;
        Read_reg = USART_ReceiveData(UART4);
       
        sdt_int8u n_bytes;
        macro_pull_queueBasce_bytes(uart4_queue_rxd,n_bytes);

        sdt_bool queunIsFull;
        macro_pull_queueBasce_full(uart4_queue_rxd,n_bytes,queunIsFull);
        if(queunIsFull)
        {
        }
        else
        {
            macro_push_queueBasce_data(uart4_queue_rxd,n_bytes,Read_reg);
        }
    }
    if(SET==USART_GetITStatus(UART4,USART_IT_TXE))
    {
        if(SET==USART_GetFlagStatus(UART4,USART_FLAG_TXE))
        {
            sdt_int8u n_bytes,rd_byte_details;
            macro_pull_queueBasce_bytes(uart4_queue_txd,n_bytes);
            if(n_bytes)
            {
                macro_pull_queueBasce_data(uart4_queue_txd,n_bytes,rd_byte_details);
                USART_SendData(UART4,rd_byte_details);
            }
            else
            {
                USART_ITConfig(UART4,USART_IT_TXE,DISABLE);
            }
        }
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++interface++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------
void bsp_uart4_cfg(void)
{
    GPIO_InitTypeDef    GPIO_USART4INIT; 
    USART_InitTypeDef   USART4_INIT;
    NVIC_InitTypeDef    USART4_NVIC_INIT;

 //-----------------------------------------------------------------------------   
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);  //开启串口时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);   //开启GPIOC口时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);   //开启GPIOC口时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);   //开启GPIOA口时钟
//-----------------------------------------------------------------------------  
    GPIO_USART4INIT.GPIO_Pin=GPIO_Pin_10;                   //PC10 U4_TXD
    GPIO_USART4INIT.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_USART4INIT.GPIO_Mode=GPIO_Mode_AF_PP;            //输出复用推挽
    GPIO_Init(GPIOC,&GPIO_USART4INIT);
    GPIO_SetBits(GPIOC,GPIO_Pin_10);
//-----------------------------------------------------------------------------     
    GPIO_USART4INIT.GPIO_Pin=GPIO_Pin_11;                  //PC11 U4_RXD
    GPIO_USART4INIT.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_USART4INIT.GPIO_Mode=GPIO_Mode_IPU;               //输入上拉
    GPIO_Init(GPIOC,&GPIO_USART4INIT);
//----------------------------------------------------------------------------- 
    GPIO_USART4INIT.GPIO_Pin=GPIO_Pin_15;                   //PA15 U4_CTROL
   // GPIO_USART4INIT.GPIO_Speed=GPIO_Speed_2MHz;
   // GPIO_USART4INIT.GPIO_Mode=GPIO_Mode_Out_PP;            //输出推挽
    GPIO_USART4INIT.GPIO_Mode=GPIO_Mode_IN_FLOATING; 
    GPIO_Init(GPIOA,&GPIO_USART4INIT);
    GPIO_ResetBits(GPIOA,GPIO_Pin_15);

      /*pc7*/
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    

    GPIO_EXTILineConfig( GPIO_PortSourceGPIOB,  GPIO_PinSource5); 
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line =  EXTI_Line5;
 
      /* EXTI为中断模式 */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
     /* 上升沿下降沿中断 */
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//EXTI_Trigger_Rising;//EXTI_Trigger_Rising_Falling
    /* 使能中断 */
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    
    NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 配置NVIC为优先级组1 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* 配置中断源：按键1 */
    NVIC_InitStructure.NVIC_IRQChannel =  EXTI9_5_IRQn;
  /* 配置抢占优先级 */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* 配置子优先级 */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* 使能中断通道 */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
//-----------------------------------------------------------------------------   
    USART_DeInit(UART4);
    USART4_INIT.USART_BaudRate=9600;
    USART4_INIT.USART_Parity=USART_Parity_No;
    USART4_INIT.USART_WordLength=USART_WordLength_8b;    

    USART4_INIT.USART_StopBits=USART_StopBits_1;
    USART4_INIT.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    USART4_INIT.USART_Mode=(USART_Mode_Rx+USART_Mode_Tx);  //收发使能
//-----------------------------------------------------------------------------
    USART_Init(UART4,&USART4_INIT);
    USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);           //使能接收中断
    USART_Cmd(UART4,ENABLE);
//-----------------------------------------------------------------------------
    USART4_NVIC_INIT.NVIC_IRQChannel=UART4_IRQn;
    USART4_NVIC_INIT.NVIC_IRQChannelPreemptionPriority=0;  //主优先级
    USART4_NVIC_INIT.NVIC_IRQChannelSubPriority=0;         //子优先级
    USART4_NVIC_INIT.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&USART4_NVIC_INIT);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;              //TIM3 3.5T的时序管理
//----------------------------------------------------------------------------- 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 
//-----------------------------------------------------------------------------
    TIM_DeInit(TIM3);
    /* Time base configuration */    
    TIM_TimeBaseStructure.TIM_Prescaler =(720)-1;                    //72M 500k ,2us
    TIM_TimeBaseStructure.TIM_Period =0xffff  ;                      // 当定时器从0计数到999，即为1000次，为一个定时周期,1个unit 2us
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;        //设置时钟分频系数：不分频(这里用不到)
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     //向上计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
//-----------------------------------------------------------------------------
    TIM_Cmd(TIM3,ENABLE);                              //开启时钟  
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//name:获取一个字节的接收数据
//out: sdt_true 获取成功，sdt_falas 获取失败
//-----------------------------------------------------------------------------
sdt_bool bsp_pull_oneByte_uart4_rxd(sdt_int8u* out_byte_details)
{
    sdt_int8u n_bytes,rd_byte_details;
    __disable_interrupt();
    macro_pull_queueBasce_bytes(uart4_queue_rxd,n_bytes);
    __enable_interrupt();
    if(n_bytes)
    {
        __disable_interrupt();
        macro_pull_queueBasce_data(uart4_queue_rxd,n_bytes,rd_byte_details);
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
sdt_bool bsp_push_oneByte_uart4_txd(sdt_int8u in_byte_details)
{
    sdt_int8u n_bytes;
    __disable_interrupt();
    macro_pull_queueBasce_bytes(uart4_queue_txd,n_bytes);
    __enable_interrupt();
    sdt_bool queunIsFull;
    macro_pull_queueBasce_full(uart4_queue_txd,n_bytes,queunIsFull);
    if(queunIsFull)
    {
        return(sdt_false);
    }
    else
    {
        __disable_interrupt();
        macro_push_queueBasce_data(uart4_queue_txd,n_bytes,in_byte_details);
        __enable_interrupt();
        USART_ITConfig(UART4,USART_IT_TXE,ENABLE);
        return(sdt_true);
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------
sdt_bool bsp_uart4_busFree(void)
{
    sdt_int16u free_cnt;
    free_cnt = TIM3->CNT;
    if(free_cnt > 200)      //9600 3.5T 1040*3.25/2
    {
        return(sdt_true);
    }
    else
    {
        return(sdt_false);
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void bsp_restart_tim3(void)
{
   TIM3->CNT = 0;
}
sdt_bool  phyBusBusyFlag4 = sdt_false;

void bsp_ms_check_uart4_phy_bus(void)
{
    if(phyBusBusyFlag4)
    {
        if(bsp_uart4_busFree())
        {
            phyBusBusyFlag4 = sdt_false;
        }
    }
}

sdt_bool bsp_ms_pull_uart4_phyBus_status(void)
{
   return phyBusBusyFlag4;
}
sdt_int8u bsp_ms_pull_uart4_rxd_num(void)
{
    sdt_int8u n_bytes;
    macro_pull_queueBasce_bytes(uart4_queue_rxd,n_bytes);
    return n_bytes;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
sdt_bool bsp_pull_uart4_txd_cmp(void)
{
    sdt_int8u n_bytes;
    
    macro_pull_queueBasce_bytes(uart4_queue_txd,n_bytes);
    if(n_bytes)
    {
        return(sdt_false);
    }
    else
    {
        if(SET==USART_GetITStatus(UART4,USART_IT_TXE))
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
void bps_uart4_into_receive(void)
{
  //  GPIO_ResetBits(GPIOA,GPIO_Pin_15);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING; 
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    GPIO_ResetBits(GPIOA,GPIO_Pin_15);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void bps_uart4_into_transmit(sdt_bool in_weekPullUp)
{
    // if(in_weekPullUp)
//    {
//      
//    }
//    else
//    {
//        GPIO_InitTypeDef GPIO_InitStructure;
//        GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15;                   //PB3 TRSEL
//        GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
//        GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;            //推挽输出
//        GPIO_Init(GPIOA,&GPIO_InitStructure);
//        GPIO_SetBits(GPIOA,GPIO_Pin_15);
//    }
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*串口5*/
//uart5 txd--PC12  rxd--PD2
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
macro_creat_queueBasce(uart5_queue_rxd,100);
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
sdt_bool bsp_ms_tim7_busFree(void)
{
    sdt_int16u free_cnt;
    free_cnt = TIM7->CNT;
    if(free_cnt > 200)      //9600 3.5T 1040*3.25/2
    {
        return(sdt_true);
    }
    else
    {
        return(sdt_false);
    }
}

void bsp_ms_restart_tim7(void)
{
   TIM7->CNT = 0;
}

sdt_bool  phyBusBusyFlag = sdt_false;
void EXTI9_5_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line7) != RESET) 
    {
        EXTI_ClearITPendingBit(EXTI_Line7);
        phyBusBusyFlag = sdt_true;
        bsp_ms_restart_tim7();     
    }
    if(EXTI_GetITStatus(EXTI_Line5) != RESET) 
    {
        EXTI_ClearITPendingBit(EXTI_Line5);
        phyBusBusyFlag4 = sdt_true;
        bsp_restart_tim3();     
    }
}

void bsp_ms_check_uart5_phy_bus(void)
{
    if(phyBusBusyFlag)
    {
        if(bsp_ms_tim7_busFree())
        {
            phyBusBusyFlag = sdt_false;
        }
    }
}

sdt_bool bsp_ms_pull_uart5_phyBus_status(void)
{
   return phyBusBusyFlag;
}
sdt_int8u bsp_ms_pull_uart5_rxd_num(void)
{
    sdt_int8u n_bytes;
    macro_pull_queueBasce_bytes(uart5_queue_rxd,n_bytes);
    return n_bytes;
}
//void TIM7_IRQHandler(void)
//{
//    static sdt_bool flag = sdt_false;
//    TIM_ClearFlag(TIM7,TIM_FLAG_Update);
//    if(flag)
//    {
//        flag = sdt_false;
//        GPIO_ResetBits(GPIOC,GPIO_Pin_7); 
//    }
//    else
//    {
//        flag = sdt_true;
//        GPIO_SetBits(GPIOC,GPIO_Pin_7); 
//    }
//}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++interface++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------
void bsp_ms_uart5_cfg(void)
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
 //   GPIO_USART5INIT.GPIO_Speed=GPIO_Speed_2MHz;
  //  GPIO_USART5INIT.GPIO_Mode=GPIO_Mode_Out_PP;            //推挽输出
     GPIO_USART5INIT.GPIO_Mode=GPIO_Mode_IN_FLOATING; 
    GPIO_Init(GPIOB,&GPIO_USART5INIT);
    GPIO_ResetBits(GPIOB,GPIO_Pin_3);
    
     /*pc7*/
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
   // GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
   // GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;            //推挽输出
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
//     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
//    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
//    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;            //推挽输出
//   // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
//    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_EXTILineConfig( GPIO_PortSourceGPIOC,  GPIO_PinSource7); 
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line =  EXTI_Line7;
 
      /* EXTI为中断模式 */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
     /* 上升沿下降沿中断 */
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//EXTI_Trigger_Rising;//EXTI_Trigger_Rising_Falling
    /* 使能中断 */
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    
    NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 配置NVIC为优先级组1 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* 配置中断源：按键1 */
    NVIC_InitStructure.NVIC_IRQChannel =  EXTI9_5_IRQn;
  /* 配置抢占优先级 */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* 配置子优先级 */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* 使能中断通道 */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
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
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); 
    TIM_DeInit(TIM7);
    /* Time base configuration */    
    TIM_TimeBaseStructure.TIM_Prescaler =(720)-1;                    //72M 100k ,10us
    TIM_TimeBaseStructure.TIM_Period =0xffff;                      // 当定时器从0计数到999，即为1000次，为一个定时周期,1个unit 10us
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;        //设置时钟分频系数：不分频(这里用不到)
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     //向上计数模式
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
//-----------------------------------------------------------------------------
    TIM_Cmd(TIM7,ENABLE);                              //开启时钟  
    
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);  
//    NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;       
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;   
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
//    NVIC_Init(&NVIC_InitStructure);  
//    TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
//    TIM_ClearFlag(TIM7,TIM_FLAG_Update);               //清除溢出中断标志  
//    TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE);  
//    TIM_Cmd(TIM7,ENABLE);                              //开启时钟  
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//name:获取一个字节的接收数据
//out: sdt_true 获取成功，sdt_falas 获取失败
//-----------------------------------------------------------------------------
sdt_bool bsp_ms_pull_oneByte_uart5_rxd(sdt_int8u* out_byte_details)
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
sdt_bool bsp_ms_push_oneByte_uart5_txd(sdt_int8u in_byte_details)
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
sdt_bool bsp_ms_uart5_busFree(sdt_int8u t_char_dis)
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
void bsp_ms_restart_tim6(void)
{
   TIM6->CNT = 0;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
sdt_bool bsp_ms_pull_uart5_txd_cmp(void)
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
void bps_ms_uart5_into_receive(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING; 
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    GPIO_ResetBits(GPIOB,GPIO_Pin_3);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void bps_ms_uart5_into_transmit(sdt_bool in_weekPullUp)
{
//    if(in_weekPullUp)
//    {
//      
//    }
//    else
//    {
//        GPIO_InitTypeDef GPIO_InitStructure;
//        GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;                   //PB3 TRSEL
//        GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
//        GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;            //推挽输出
//        GPIO_Init(GPIOB,&GPIO_InitStructure);
//        GPIO_SetBits(GPIOB,GPIO_Pin_3);
//    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void bsp_ms_uart5_phyReCfg(sdt_int32u sys_fre,sdt_int32u baud_rate,sdt_int8u parity,sdt_int8u stop_bits)
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
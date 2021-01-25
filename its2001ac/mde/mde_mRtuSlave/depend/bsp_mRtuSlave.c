//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "stm32f10x.h"
#include ".\snail_macro.h"
#include ".\snail_data_types.h"
#include "..\..\mde_boughLink\mde_boughLink.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//macro_creat_queueBasce(uart4_queue_rxd,64);
//macro_creat_queueBasce(uart4_queue_txd,255);
////+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
////+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
////中断服务函数
////-----------------------------------------------------------------------------
//void UART4_IRQHandler(void)
//{
//    if((SET==USART_GetFlagStatus(UART4,USART_FLAG_ORE))||\
//       (SET==USART_GetFlagStatus(UART4,USART_FLAG_NE))||\
//       (SET==USART_GetFlagStatus(UART4,USART_FLAG_FE)))
//    {
//        USART_GetFlagStatus(UART4,USART_FLAG_ORE);
//        USART_ReceiveData(UART4);
//    }
//    else if(SET==USART_GetFlagStatus(UART4,USART_FLAG_PE))
//    {
//        USART_GetFlagStatus(UART4,USART_FLAG_PE);
//        USART_ReceiveData(UART4);
//    }
//    else if(SET==USART_GetFlagStatus(UART4,USART_FLAG_RXNE))
//    {
//        sdt_int8u Read_reg;
//        Read_reg = USART_ReceiveData(UART4);
//       
//        sdt_int8u n_bytes;
//        macro_pull_queueBasce_bytes(uart4_queue_rxd,n_bytes);
//
//        sdt_bool queunIsFull;
//        macro_pull_queueBasce_full(uart4_queue_rxd,n_bytes,queunIsFull);
//        if(queunIsFull)
//        {
//        }
//        else
//        {
//            macro_push_queueBasce_data(uart4_queue_rxd,n_bytes,Read_reg);
//        }
//    }
//    if(SET==USART_GetITStatus(UART4,USART_IT_TXE))
//    {
//        if(SET==USART_GetFlagStatus(UART4,USART_FLAG_TXE))
//        {
//            sdt_int8u n_bytes,rd_byte_details;
//            macro_pull_queueBasce_bytes(uart4_queue_txd,n_bytes);
//            if(n_bytes)
//            {
//                macro_pull_queueBasce_data(uart4_queue_txd,n_bytes,rd_byte_details);
//                USART_SendData(UART4,rd_byte_details);
//            }
//            else
//            {
//                USART_ITConfig(UART4,USART_IT_TXE,DISABLE);
//            }
//        }
//    }
//}
////+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
////+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
////++++++++++++++++++++++++++++++++interface++++++++++++++++++++++++++++++++++++
////-----------------------------------------------------------------------------
//void bsp_uart4_cfg(void)
//{
//    GPIO_InitTypeDef    GPIO_USART4INIT; 
//    USART_InitTypeDef   USART4_INIT;
//    NVIC_InitTypeDef    USART4_NVIC_INIT;
//
// //-----------------------------------------------------------------------------   
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);  //开启串口时钟
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);   //开启GPIOC口时钟
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);   //开启GPIOC口时钟
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
//    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);   //开启GPIOA口时钟
////-----------------------------------------------------------------------------  
//    GPIO_USART4INIT.GPIO_Pin=GPIO_Pin_10;                   //PC10 U4_TXD
//    GPIO_USART4INIT.GPIO_Speed=GPIO_Speed_2MHz;
//    GPIO_USART4INIT.GPIO_Mode=GPIO_Mode_AF_PP;            //输出复用推挽
//    GPIO_Init(GPIOC,&GPIO_USART4INIT);
//    GPIO_SetBits(GPIOC,GPIO_Pin_10);
////-----------------------------------------------------------------------------     
//    GPIO_USART4INIT.GPIO_Pin=GPIO_Pin_11;                  //PC11 U4_RXD
//    GPIO_USART4INIT.GPIO_Speed=GPIO_Speed_2MHz;
//    GPIO_USART4INIT.GPIO_Mode=GPIO_Mode_IPU;               //输入上拉
//    GPIO_Init(GPIOC,&GPIO_USART4INIT);
////----------------------------------------------------------------------------- 
//    GPIO_USART4INIT.GPIO_Pin=GPIO_Pin_15;                   //PA15 U4_CTROL
//    GPIO_USART4INIT.GPIO_Speed=GPIO_Speed_2MHz;
//    GPIO_USART4INIT.GPIO_Mode=GPIO_Mode_Out_PP;            //输出推挽
//    GPIO_Init(GPIOA,&GPIO_USART4INIT);
//    GPIO_ResetBits(GPIOA,GPIO_Pin_15);
////-----------------------------------------------------------------------------   
//    USART_DeInit(UART4);
//    USART4_INIT.USART_BaudRate=9600;
//    USART4_INIT.USART_Parity=USART_Parity_No;
//    USART4_INIT.USART_WordLength=USART_WordLength_8b;    
//
//    USART4_INIT.USART_StopBits=USART_StopBits_1;
//    USART4_INIT.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
//    USART4_INIT.USART_Mode=(USART_Mode_Rx+USART_Mode_Tx);  //收发使能
////-----------------------------------------------------------------------------
//    USART_Init(UART4,&USART4_INIT);
//    USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);           //使能接收中断
//    USART_Cmd(UART4,ENABLE);
////-----------------------------------------------------------------------------
//    USART4_NVIC_INIT.NVIC_IRQChannel=UART4_IRQn;
//    USART4_NVIC_INIT.NVIC_IRQChannelPreemptionPriority=0;  //主优先级
//    USART4_NVIC_INIT.NVIC_IRQChannelSubPriority=0;         //子优先级
//    USART4_NVIC_INIT.NVIC_IRQChannelCmd=ENABLE;
//    NVIC_Init(&USART4_NVIC_INIT);
////+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;              //TIM3 3.5T的时序管理
////----------------------------------------------------------------------------- 
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 
////-----------------------------------------------------------------------------
//    TIM_DeInit(TIM3);
//    /* Time base configuration */    
//    TIM_TimeBaseStructure.TIM_Prescaler =(72*2)-1;                    //72M 500k ,2us
//    TIM_TimeBaseStructure.TIM_Period =0xffff  ;                      // 当定时器从0计数到999，即为1000次，为一个定时周期,1个unit 2us
//    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;        //设置时钟分频系数：不分频(这里用不到)
//    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     //向上计数模式
//    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
////-----------------------------------------------------------------------------
//    TIM_Cmd(TIM3,ENABLE);                              //开启时钟  
//}
////+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
////name:获取一个字节的接收数据
////out: sdt_true 获取成功，sdt_falas 获取失败
////-----------------------------------------------------------------------------
//sdt_bool bsp_pull_oneByte_uart4_rxd(sdt_int8u* out_byte_details)
//{
//    sdt_int8u n_bytes,rd_byte_details;
//    __disable_interrupt();
//    macro_pull_queueBasce_bytes(uart4_queue_rxd,n_bytes);
//    __enable_interrupt();
//    if(n_bytes)
//    {
//        __disable_interrupt();
//        macro_pull_queueBasce_data(uart4_queue_rxd,n_bytes,rd_byte_details);
//        __enable_interrupt();
//        *out_byte_details = rd_byte_details;
//        Push_Remote_OneByteToBoughLink(rd_byte_details);
//        return(sdt_true);
//    }
//    else
//    {
//        
//        return(sdt_false);
//    }
//}
////+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
////name:压入一个字节的数据到发送队列
////out: sdt_true 压入成功，sdt_false 压入失败
////-----------------------------------------------------------------------------
//sdt_bool bsp_push_oneByte_uart4_txd(sdt_int8u in_byte_details)
//{
//    sdt_int8u n_bytes;
//    __disable_interrupt();
//    macro_pull_queueBasce_bytes(uart4_queue_txd,n_bytes);
//    __enable_interrupt();
//    sdt_bool queunIsFull;
//    macro_pull_queueBasce_full(uart4_queue_txd,n_bytes,queunIsFull);
//    if(queunIsFull)
//    {
//        return(sdt_false);
//    }
//    else
//    {
//        __disable_interrupt();
//        macro_push_queueBasce_data(uart4_queue_txd,n_bytes,in_byte_details);
//        __enable_interrupt();
//        USART_ITConfig(UART4,USART_IT_TXE,ENABLE);
//        return(sdt_true);
//    }
//}
////+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
////-----------------------------------------------------------------------------
//sdt_bool bsp_uart4_busFree(sdt_int8u t_char_dis)
//{
//    //static sdt_int16u rd_rxd_depart_cnt;
//    static  sdt_int16u rd_cnt;
//    static  sdt_int16u free_cnt;
//
//    rd_cnt = TIM3->CNT;
//    free_cnt = rd_cnt;
//
//    sdt_int16u limit_ft;
//    
//    limit_ft = 104*t_char_dis/2;  //9600 T == 104us
//    if(free_cnt > limit_ft)  //9600 3.5T 1040*3.25/2
//    {
//        return(sdt_true);
//    }
//    else
//    {
//        return(sdt_false);
//    }
//}
////+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//void bsp_restart_tim3(void)
//{
//   TIM3->CNT = 0;
//}
////+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//sdt_bool bsp_pull_uart4_txd_cmp(void)
//{
//    sdt_int8u n_bytes;
//    
//    macro_pull_queueBasce_bytes(uart4_queue_txd,n_bytes);
//    if(n_bytes)
//    {
//        return(sdt_false);
//    }
//    else
//    {
//        if(SET==USART_GetITStatus(UART4,USART_IT_TXE))
//        {
//            return(sdt_false);
//        }
//        else
//        {
//            return(sdt_true);
//        }      
//    }
//}
////+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//void bps_uart4_into_receive(void)
//{
//    GPIO_ResetBits(GPIOA,GPIO_Pin_15);
//}
////+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//void bps_uart4_into_transmit(void)
//{
//    GPIO_SetBits(GPIOA,GPIO_Pin_15);
//}
////+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//void bsp_uart_phyReCfg(sdt_int32u sys_fre,sdt_int32u baud_rate,sdt_int8u parity,sdt_int8u stop_bits)
//{
//    //static sdt_int32u bak_sys_fre = 0,bak_baud_rate = 0;
//    //static sdt_int8u bak_parity = 0,bak_stop_bits = 0;
//    USART_InitTypeDef   USART4_INIT;
//
//   // if()
//
//    USART_DeInit(UART4);
//    USART4_INIT.USART_BaudRate=9600;
//    USART4_INIT.USART_Parity=USART_Parity_No;
//    USART4_INIT.USART_WordLength=USART_WordLength_8b;    
//
//    USART4_INIT.USART_StopBits=USART_StopBits_1;
//    USART4_INIT.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
//    USART4_INIT.USART_Mode=(USART_Mode_Rx+USART_Mode_Tx);  //收发使能
////-----------------------------------------------------------------------------
//    USART_Init(UART4,&USART4_INIT);
//}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//void get_queue_data(sdt_int8u* out_inx,sdt_int8u* out_otx,sdt_int8u* out_nbyte)
//{
//    __disable_interrupt();
//    *out_inx = uart4_queue_rxd_in_idx;
//    *out_otx = uart4_queue_rxd_out_idx;
//    *out_nbyte = uart4_queue_rxd_bytes;
//    __enable_interrupt();
//}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*串口3配置*/
macro_creat_queueBasce(uart3_queue_rxd,64);
macro_creat_queueBasce(uart3_queue_txd,255);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//中断服务函数
//-----------------------------------------------------------------------------
void USART3_IRQHandler(void)
{
    if((SET==USART_GetFlagStatus(USART3,USART_FLAG_ORE))||\
       (SET==USART_GetFlagStatus(USART3,USART_FLAG_NE))||\
       (SET==USART_GetFlagStatus(USART3,USART_FLAG_FE)))
    {
        USART_GetFlagStatus(USART3,USART_FLAG_ORE);
        USART_ReceiveData(USART3);
    }
    else if(SET==USART_GetFlagStatus(USART3,USART_FLAG_PE))
    {
        USART_GetFlagStatus(USART3,USART_FLAG_PE);
        USART_ReceiveData(USART3);
    }
    else if(SET==USART_GetFlagStatus(USART3,USART_FLAG_RXNE))
    {
        sdt_int8u Read_reg;
        Read_reg = USART_ReceiveData(USART3);
       
        sdt_int8u n_bytes;
        macro_pull_queueBasce_bytes(uart3_queue_rxd,n_bytes);

        sdt_bool queunIsFull;
        macro_pull_queueBasce_full(uart3_queue_rxd,n_bytes,queunIsFull);
        if(queunIsFull)
        {
        }
        else
        {
            macro_push_queueBasce_data(uart3_queue_rxd,n_bytes,Read_reg);
        }
    }
    if(SET==USART_GetITStatus(USART3,USART_IT_TXE))
    {
        if(SET==USART_GetFlagStatus(USART3,USART_FLAG_TXE))
        {
            sdt_int8u n_bytes,rd_byte_details;
            macro_pull_queueBasce_bytes(uart3_queue_txd,n_bytes);
            if(n_bytes)
            {
                macro_pull_queueBasce_data(uart3_queue_txd,n_bytes,rd_byte_details);
                USART_SendData(USART3,rd_byte_details);
            }
            else
            {
                USART_ITConfig(USART3,USART_IT_TXE,DISABLE);
            }
        }
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++interface++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------
void bsp_uart3_cfg(void)
{
    GPIO_InitTypeDef    GPIO_USART3INIT; 
    USART_InitTypeDef   USART3_INIT;
    NVIC_InitTypeDef    USART3_NVIC_INIT;

 //-----------------------------------------------------------------------------   
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);  //开启串口时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);   //开启GPIOB口时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
   // GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
//-----------------------------------------------------------------------------  
    GPIO_USART3INIT.GPIO_Pin=GPIO_Pin_10;                   //PB10 U3_TXD
    GPIO_USART3INIT.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_USART3INIT.GPIO_Mode=GPIO_Mode_AF_PP;            //输出复用推挽
    GPIO_Init(GPIOB,&GPIO_USART3INIT);
    GPIO_SetBits(GPIOB,GPIO_Pin_10);
//-----------------------------------------------------------------------------     
    GPIO_USART3INIT.GPIO_Pin=GPIO_Pin_11;                  //PB11 U3_RXD
    GPIO_USART3INIT.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_USART3INIT.GPIO_Mode=GPIO_Mode_IPU;               //输入上拉
    GPIO_Init(GPIOB,&GPIO_USART3INIT);
//-----------------------------------------------------------------------------  
    GPIO_USART3INIT.GPIO_Pin=GPIO_Pin_1;                   //PB1 U3_CTROL
    GPIO_USART3INIT.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_USART3INIT.GPIO_Mode=GPIO_Mode_Out_PP;            //输出推挽
    GPIO_Init(GPIOB,&GPIO_USART3INIT);
    GPIO_ResetBits(GPIOB,GPIO_Pin_1);

//    GPIO_USART3INIT.GPIO_Pin=GPIO_Pin_12;                   //PB12 TR Control
//    GPIO_USART3INIT.GPIO_Speed=GPIO_Speed_2MHz;
//    GPIO_USART3INIT.GPIO_Mode=GPIO_Mode_Out_OD;            //输出开漏
//    GPIO_Init(GPIOB,&GPIO_USART3INIT);
//    GPIO_ResetBits(GPIOB,GPIO_Pin_12); 
//-----------------------------------------------------------------------------   
    USART_DeInit(USART3);
    USART3_INIT.USART_BaudRate=9600;
    USART3_INIT.USART_Parity=USART_Parity_No;
    USART3_INIT.USART_WordLength=USART_WordLength_8b;    

    USART3_INIT.USART_StopBits=USART_StopBits_1;
    USART3_INIT.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    USART3_INIT.USART_Mode=(USART_Mode_Rx+USART_Mode_Tx);  //收发使能
//-----------------------------------------------------------------------------
    USART_Init(USART3,&USART3_INIT);
    USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);           //使能接收中断
    USART_Cmd(USART3,ENABLE);
//-----------------------------------------------------------------------------
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    USART3_NVIC_INIT.NVIC_IRQChannel=USART3_IRQn;
    USART3_NVIC_INIT.NVIC_IRQChannelPreemptionPriority=0;  //主优先级
    USART3_NVIC_INIT.NVIC_IRQChannelSubPriority=0;         //子优先级
    USART3_NVIC_INIT.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&USART3_NVIC_INIT);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;              //TIM3 3.5T的时序管理
//----------------------------------------------------------------------------- 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 
//-----------------------------------------------------------------------------
    TIM_DeInit(TIM4);
    /* Time base configuration */    
    TIM_TimeBaseStructure.TIM_Prescaler =(72*2)-1;                    //72M 500k ,2us
    TIM_TimeBaseStructure.TIM_Period =0xffff;                      // 当定时器从0计数到999，即为1000次，为一个定时周期,1个unit 2us
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;        //设置时钟分频系数：不分频(这里用不到)
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     //向上计数模式
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
//-----------------------------------------------------------------------------
    TIM_Cmd(TIM4,ENABLE);                              //开启时钟  
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//name:获取一个字节的接收数据
//out: sdt_true 获取成功，sdt_falas 获取失败
//-----------------------------------------------------------------------------
sdt_bool bsp_pull_oneByte_uart3_rxd(sdt_int8u* out_byte_details)
{
    sdt_int8u n_bytes,rd_byte_details;
    __disable_interrupt();
    macro_pull_queueBasce_bytes(uart3_queue_rxd,n_bytes);
    __enable_interrupt();
    if(n_bytes)
    {
        __disable_interrupt();
        macro_pull_queueBasce_data(uart3_queue_rxd,n_bytes,rd_byte_details);
        __enable_interrupt();
        *out_byte_details = rd_byte_details;
       // Push_Remote_OneByteToBoughLink(rd_byte_details);
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
sdt_bool bsp_push_oneByte_uart3_txd(sdt_int8u in_byte_details)
{
    sdt_int8u n_bytes;
    __disable_interrupt();
    macro_pull_queueBasce_bytes(uart3_queue_txd,n_bytes);
    __enable_interrupt();
    sdt_bool queunIsFull;
    macro_pull_queueBasce_full(uart3_queue_txd,n_bytes,queunIsFull);
    if(queunIsFull)
    {
        return(sdt_false);
    }
    else
    {
        __disable_interrupt();
        macro_push_queueBasce_data(uart3_queue_txd,n_bytes,in_byte_details);
        __enable_interrupt();
        USART_ITConfig(USART3,USART_IT_TXE,ENABLE);
        return(sdt_true);
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------
sdt_bool bsp_uart3_busFree(sdt_int8u t_char_dis)
{
    static  sdt_int16u rd_cnt;
    static  sdt_int16u free_cnt;

    rd_cnt = TIM4->CNT;
    free_cnt = rd_cnt;

    sdt_int16u limit_ft;
    
    limit_ft = 104*t_char_dis/2;  //9600 T == 104us
    if(free_cnt > limit_ft)  //9600 3.5T 1040*3.25/2
    {
        return(sdt_true);
    }
    else
    {
        return(sdt_false);
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void bsp_restart_tim4(void)
{
   TIM4->CNT = 0;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
sdt_bool bsp_pull_uart3_txd_cmp(void)
{
    sdt_int8u n_bytes;
    
    macro_pull_queueBasce_bytes(uart3_queue_txd,n_bytes);
    if(n_bytes)
    {
        return(sdt_false);
    }
    else
    {
        if(SET==USART_GetITStatus(USART3,USART_IT_TXE))
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
void bps_uart3_into_receive(void)
{
 //   USART_ITConfig(USART3,USART_IT_RXNE,ENABLE); 
    GPIO_ResetBits(GPIOB,GPIO_Pin_1);
   // GPIO_ResetBits(GPIOB,GPIO_Pin_12);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void bps_uart3_into_transmit(void)
{
  //  USART_ITConfig(USART3,USART_IT_RXNE,DISABLE); 
    GPIO_SetBits(GPIOB,GPIO_Pin_1);
   // GPIO_SetBits(GPIOB,GPIO_Pin_12);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void bsp_uart3_phyReCfg(sdt_int32u sys_fre,sdt_int32u baud_rate,sdt_int8u parity,sdt_int8u stop_bits)
{
    USART_InitTypeDef   USART3_INIT;
    USART_DeInit(USART3);
    USART3_INIT.USART_BaudRate=9600;
    USART3_INIT.USART_Parity=USART_Parity_No;
    USART3_INIT.USART_WordLength=USART_WordLength_8b;    

    USART3_INIT.USART_StopBits=USART_StopBits_1;
    USART3_INIT.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    USART3_INIT.USART_Mode=(USART_Mode_Rx+USART_Mode_Tx);  //收发使能
//-----------------------------------------------------------------------------
    USART_Init(USART3,&USART3_INIT);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void get_queue3_data(sdt_int8u* out_inx,sdt_int8u* out_otx,sdt_int8u* out_nbyte)
{
    __disable_interrupt();
    *out_inx = uart3_queue_rxd_in_idx;
    *out_otx = uart3_queue_rxd_out_idx;
    *out_nbyte = uart3_queue_rxd_bytes;
    __enable_interrupt();
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*串口1配置*/
macro_creat_queueBasce(uart1_queue_rxd,64);
macro_creat_queueBasce(uart1_queue_txd,255);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//中断服务函数
//-----------------------------------------------------------------------------
void USART1_IRQHandler(void)
{
    if((SET==USART_GetFlagStatus(USART1,USART_FLAG_ORE))||\
       (SET==USART_GetFlagStatus(USART1,USART_FLAG_NE))||\
       (SET==USART_GetFlagStatus(USART1,USART_FLAG_FE)))
    {
        USART_GetFlagStatus(USART1,USART_FLAG_ORE);
        USART_ReceiveData(USART1);
    }
    else if(SET==USART_GetFlagStatus(USART1,USART_FLAG_PE))
    {
        USART_GetFlagStatus(USART1,USART_FLAG_PE);
        USART_ReceiveData(USART1);
    }
    else if(SET==USART_GetFlagStatus(USART1,USART_FLAG_RXNE))
    {
        sdt_int8u Read_reg;
        Read_reg = USART_ReceiveData(USART1);
       
        sdt_int8u n_bytes;
        macro_pull_queueBasce_bytes(uart1_queue_rxd,n_bytes);

        sdt_bool queunIsFull;
        macro_pull_queueBasce_full(uart1_queue_rxd,n_bytes,queunIsFull);
        if(queunIsFull)
        {
        }
        else
        {
            macro_push_queueBasce_data(uart1_queue_rxd,n_bytes,Read_reg);
        }
    }
    if(SET==USART_GetITStatus(USART1,USART_IT_TXE))
    {
        if(SET==USART_GetFlagStatus(USART1,USART_FLAG_TXE))
        {
            sdt_int8u n_bytes,rd_byte_details;
            macro_pull_queueBasce_bytes(uart1_queue_txd,n_bytes);
            if(n_bytes)
            {
                macro_pull_queueBasce_data(uart1_queue_txd,n_bytes,rd_byte_details);
                USART_SendData(USART1,rd_byte_details);
            }
            else
            {
                USART_ITConfig(USART1,USART_IT_TXE,DISABLE);
            }
        }
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++interface++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------
void bsp_uart1_cfg(void)
{
    GPIO_InitTypeDef    GPIO_USART1INIT; 
    USART_InitTypeDef   USART1_INIT;
    NVIC_InitTypeDef    USART1_NVIC_INIT;

 //-----------------------------------------------------------------------------   
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);  //开启串口时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);   //开启GPIOA口时钟
   // RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
//-----------------------------------------------------------------------------  
    GPIO_USART1INIT.GPIO_Pin=GPIO_Pin_9;                   //PA9 U1_TXD
    GPIO_USART1INIT.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_USART1INIT.GPIO_Mode=GPIO_Mode_AF_PP;            //输出复用推挽
    GPIO_Init(GPIOA,&GPIO_USART1INIT);
    GPIO_SetBits(GPIOA,GPIO_Pin_9);
//-----------------------------------------------------------------------------     
    GPIO_USART1INIT.GPIO_Pin=GPIO_Pin_10;                  //PA10 U1_RXD
    GPIO_USART1INIT.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_USART1INIT.GPIO_Mode=GPIO_Mode_IPU;               //输入上拉
    GPIO_Init(GPIOA,&GPIO_USART1INIT);
//-----------------------------------------------------------------------------  
    GPIO_USART1INIT.GPIO_Pin=GPIO_Pin_8;                   //PA8 U1_CTROL
    GPIO_USART1INIT.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_USART1INIT.GPIO_Mode=GPIO_Mode_Out_OD;            //开漏输出
    GPIO_Init(GPIOA,&GPIO_USART1INIT);
    GPIO_ResetBits(GPIOA,GPIO_Pin_8);
//-----------------------------------------------------------------------------   
    USART_DeInit(USART1);
    USART1_INIT.USART_BaudRate=19200;
    USART1_INIT.USART_Parity=USART_Parity_No;
    USART1_INIT.USART_WordLength=USART_WordLength_8b;    

    USART1_INIT.USART_StopBits=USART_StopBits_1;
    USART1_INIT.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    USART1_INIT.USART_Mode=(USART_Mode_Rx+USART_Mode_Tx);  //收发使能
//-----------------------------------------------------------------------------
    USART_Init(USART1,&USART1_INIT);
    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);           //使能接收中断
    USART_Cmd(USART1,ENABLE);
//-----------------------------------------------------------------------------
    USART1_NVIC_INIT.NVIC_IRQChannel=USART1_IRQn;
    USART1_NVIC_INIT.NVIC_IRQChannelPreemptionPriority=0;  //主优先级
    USART1_NVIC_INIT.NVIC_IRQChannelSubPriority=0;         //子优先级
    USART1_NVIC_INIT.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&USART1_NVIC_INIT);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;              //TIM1 3.5T的时序管理
//----------------------------------------------------------------------------- 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); 
//-----------------------------------------------------------------------------
    TIM_DeInit(TIM1);
    /* Time base configuration */    
    TIM_TimeBaseStructure.TIM_Prescaler =(72*2)-1;                    //72M 500k ,2us
    TIM_TimeBaseStructure.TIM_Period =0xffff;                      // 当定时器从0计数到999，即为1000次，为一个定时周期,1个unit 2us
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;        //设置时钟分频系数：不分频(这里用不到)
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     //向上计数模式
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
//-----------------------------------------------------------------------------
    TIM_Cmd(TIM1,ENABLE);                              //开启时钟  
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//name:获取一个字节的接收数据
//out: sdt_true 获取成功，sdt_falas 获取失败
//-----------------------------------------------------------------------------
sdt_bool bsp_pull_oneByte_uart1_rxd(sdt_int8u* out_byte_details)
{
    sdt_int8u n_bytes,rd_byte_details;
    __disable_interrupt();
    macro_pull_queueBasce_bytes(uart1_queue_rxd,n_bytes);
    __enable_interrupt();
    if(n_bytes)
    {
        __disable_interrupt();
        macro_pull_queueBasce_data(uart1_queue_rxd,n_bytes,rd_byte_details);
        __enable_interrupt();
        *out_byte_details = rd_byte_details;
        //Push_Remote_OneByteToBoughLink(rd_byte_details);
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
sdt_bool bsp_push_oneByte_uart1_txd(sdt_int8u in_byte_details)
{
    sdt_int8u n_bytes;
    __disable_interrupt();
    macro_pull_queueBasce_bytes(uart1_queue_txd,n_bytes);
    __enable_interrupt();
    sdt_bool queunIsFull;
    macro_pull_queueBasce_full(uart1_queue_txd,n_bytes,queunIsFull);
    if(queunIsFull)
    {
        return(sdt_false);
    }
    else
    {
        __disable_interrupt();
        macro_push_queueBasce_data(uart1_queue_txd,n_bytes,in_byte_details);
        __enable_interrupt();
        USART_ITConfig(USART1,USART_IT_TXE,ENABLE);
        return(sdt_true);
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------
sdt_bool bsp_uart1_busFree(sdt_int8u t_char_dis)
{
    //static sdt_int16u rd_rxd_depart_cnt;
    static  sdt_int16u rd_cnt;
    static  sdt_int16u free_cnt;

    rd_cnt = TIM1->CNT;
    free_cnt = rd_cnt;

    sdt_int16u limit_ft;
    
    limit_ft = 104*t_char_dis/2;  //9600 T == 104us
    if(free_cnt > limit_ft)  //9600 3.5T 1040*3.25/2
    {
        return(sdt_true);
    }
    else
    {
        return(sdt_false);
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void bsp_restart_tim1(void)
{
   TIM1->CNT = 0;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
sdt_bool bsp_pull_uart1_txd_cmp(void)
{
    sdt_int8u n_bytes;
    
    macro_pull_queueBasce_bytes(uart1_queue_txd,n_bytes);
    if(n_bytes)
    {
        return(sdt_false);
    }
    else
    {
        if(SET==USART_GetITStatus(USART1,USART_IT_TXE))
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
void bps_uart1_into_receive(void)
{
    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
    GPIO_ResetBits(GPIOA,GPIO_Pin_8);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void bps_uart1_into_transmit(void)
{
    USART_ITConfig(USART1,USART_IT_RXNE,DISABLE); 
    GPIO_SetBits(GPIOA,GPIO_Pin_8);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void bsp_uart1_phyReCfg(sdt_int32u sys_fre,sdt_int32u baud_rate,sdt_int8u parity,sdt_int8u stop_bits)
{
    USART_InitTypeDef   USART1_INIT;
    USART_DeInit(USART1);
    USART1_INIT.USART_BaudRate=baud_rate;
    USART1_INIT.USART_Parity=USART_Parity_No;
    USART1_INIT.USART_WordLength=USART_WordLength_8b;    

    USART1_INIT.USART_StopBits=USART_StopBits_1;
    USART1_INIT.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    USART1_INIT.USART_Mode=(USART_Mode_Rx+USART_Mode_Tx);  //收发使能
//-----------------------------------------------------------------------------
    USART_Init(USART1,&USART1_INIT);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void get_queue1_data(sdt_int8u* out_inx,sdt_int8u* out_otx,sdt_int8u* out_nbyte)
{
    __disable_interrupt();
    *out_inx = uart1_queue_rxd_in_idx;
    *out_otx = uart1_queue_rxd_out_idx;
    *out_nbyte = uart1_queue_rxd_bytes;
    __enable_interrupt();
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
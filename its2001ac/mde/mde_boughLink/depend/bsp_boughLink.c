//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef snail_data_types
    #include ".\snail_data_types.h"
#endif
#include "stm32f10x.h"
#include ".\Fifo_Macros.h"
#include "intrinsics.h"
//-------------------------------------------------------------------------------------------------
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
_Creat_my_fifo(Uart_Rxd,16);
_Creat_my_fifo(Uart_Txd,16);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//PHY use uart2 PA3--U2RX  PA2--U2TX
//tr_select  PB1
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//input floating
#define macro_trsCfg    //PG_DDR |= (0x04); PG_CR1 &= (~0x04); PG_CR2 &= (~0x04); PG_ODR &= (~0x04)
#define macro_trs_receive       GPIO_ResetBits(GPIOA,GPIO_Pin_6)
#define macro_trs_transmit      GPIO_SetBits(GPIOA,GPIO_Pin_6)
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Recfg_Buadrate_USART3(sdt_int32u ClockFreqValue)
{

}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//UART3口的配置
//PG0--U3RX  PG1--U3TX
//波特率:9600,无校验
//-------------------------------------------------------------------------------------------------
void Uart2_Configure(void)
{
    GPIO_InitTypeDef    GPIO_USART_INIT; 
    USART_InitTypeDef   USART_INIT;
    NVIC_InitTypeDef    USART_NVIC_INIT;

 //-----------------------------------------------------------------------------   
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);  //开启串口时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);   //开启GPIOC口时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);   //开启GPIOC口时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
//-----------------------------------------------------------------------------  
    GPIO_USART_INIT.GPIO_Pin=GPIO_Pin_2;                   //PA2 U2_TXD
    GPIO_USART_INIT.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_USART_INIT.GPIO_Mode=GPIO_Mode_AF_PP;            //输出复用推挽
    GPIO_Init(GPIOA,&GPIO_USART_INIT);
    GPIO_SetBits(GPIOA,GPIO_Pin_2);
//-----------------------------------------------------------------------------     
    GPIO_USART_INIT.GPIO_Pin=GPIO_Pin_3;                  //PA3 U2_RXD
    GPIO_USART_INIT.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_USART_INIT.GPIO_Mode=GPIO_Mode_IPU;               //输入上拉
    GPIO_Init(GPIOA,&GPIO_USART_INIT);
//-----------------------------------------------------------------------------  
    GPIO_USART_INIT.GPIO_Pin=GPIO_Pin_6;                   //PA15 TRSEL
    GPIO_USART_INIT.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_USART_INIT.GPIO_Mode=GPIO_Mode_Out_PP;            //推挽输出
    GPIO_Init(GPIOA,&GPIO_USART_INIT);
    GPIO_ResetBits(GPIOA,GPIO_Pin_6);
//-----------------------------------------------------------------------------   
    USART_DeInit(USART2);
    USART_INIT.USART_BaudRate = 9600;
    USART_INIT.USART_Parity = USART_Parity_No;
    USART_INIT.USART_WordLength = USART_WordLength_8b;    

    USART_INIT.USART_StopBits = USART_StopBits_1;
    USART_INIT.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_INIT.USART_Mode = (USART_Mode_Rx+USART_Mode_Tx);  //收发使能
//-----------------------------------------------------------------------------
    USART_Init(USART2,&USART_INIT);
    USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);           //使能接收中断
    USART_Cmd(USART2,ENABLE);
//-----------------------------------------------------------------------------
    USART_NVIC_INIT.NVIC_IRQChannel=USART2_IRQn;
    USART_NVIC_INIT.NVIC_IRQChannelPreemptionPriority=0;  //主优先级
    USART_NVIC_INIT.NVIC_IRQChannelSubPriority=0;         //子优先级
    USART_NVIC_INIT.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&USART_NVIC_INIT);
//-------------------------------------------------------------------------------------------------
    _Init_my_fifo(Uart_Rxd);
    _Init_my_fifo(Uart_Txd);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
static sdt_bool Txd_Finish;
//-------------------------------------------------------------------------------------------------
//void USART2_IRQHandler(void)
//{
//  
//      if((SET==USART_GetFlagStatus(USART2,USART_FLAG_ORE))||\
//       (SET==USART_GetFlagStatus(USART2,USART_FLAG_NE))||\
//       (SET==USART_GetFlagStatus(USART2,USART_FLAG_FE)))
//    {
//        USART_GetFlagStatus(USART2,USART_FLAG_ORE);
//        USART_ReceiveData(USART2);
//    }
//    else if(SET==USART_GetFlagStatus(USART2,USART_FLAG_PE))
//    {
//        USART_GetFlagStatus(USART2,USART_FLAG_PE);
//        USART_ReceiveData(USART2);
//    }
//    else if(SET==USART_GetFlagStatus(USART2,USART_FLAG_RXNE))
//    {
//        sdt_int8u Read_reg;
//        Read_reg = USART_ReceiveData(USART2);
//        _In_my_fifo(Uart_Rxd,Read_reg);
//    }
//    if(SET==USART_GetITStatus(USART2,USART_IT_TXE))
//    {
//        if(SET==USART_GetFlagStatus(USART2,USART_FLAG_TXE))
//        {
//            sdt_int8u n_bytes,rd_byte_details;
//            
//            _Get_my_fifo_byte(Uart_Txd,n_bytes);
//            if(n_bytes)
//            {
//                _Out_my_fifo(Uart_Txd,rd_byte_details);
//                USART_SendData(USART2,rd_byte_details);
//            }
//            else
//            {
//                USART_ITConfig(USART2,USART_IT_TXE,DISABLE); //禁用发生中断
//            }
//        }
//    }
//    else if(SET==USART_GetITStatus(USART2,USART_IT_TC))
//    {
//        USART_ITConfig(USART2,USART_IT_TC,DISABLE);
//        Txd_Finish = sdt_true;
//    }
//}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//*********************************************************
//发送中断
////---------------------------------------------------------

////---------------------------------------------------------
//#pragma vector = USART3_T_TXE_vector//USART3_T_TC_vector
//__interrupt void ISR_Uart3_Txd(void)
//{
//    sdt_int8u read_reg,nbytes;    
//    
//    if(USART3_CR2_bit.TIEN)
//    {
//        if(USART3_SR_bit.TXE) 
//        {
//            _Get_my_fifo_byte(Uart_Txd,nbytes);
//            if(nbytes)
//            {
//                _Out_my_fifo(Uart_Txd,read_reg);
//                USART3_DR=read_reg;
//            }
//            else
//            {
//                USART3_CR2_bit.TIEN=0;   //禁用发生中断
//            }
//        }        
//    }
//    else if(USART3_CR2_bit.TCIEN)
//    {
//        if(USART3_SR_bit.TC)
//        {
//            USART3_CR2_bit.TCIEN = 0;  //发送完毕
//            Txd_Finish = sdt_true;
//        }
//    }
//}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//链路进入接收模式
//-------------------------------------------------------------------------------------------------
void Enter_ReceiveMode_PHY(void)
{
    USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);  
    macro_trs_receive;
}
//-------------------------------------------------------------------------------------------------
//链路进入发送模式
//-------------------------------------------------------------------------------------------------
void Enter_TransmitMode_PHY(void)
{
    USART_ITConfig(USART2,USART_IT_RXNE,DISABLE); 
    macro_trs_transmit;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//发送完毕状态获取,sdt_true -- 发送完毕
//-------------------------------------------------------------------------------------------------
sdt_bool Pull_PHYTxd_Finish(void)
{
    sdt_bool Read_Finish;
    
    __disable_interrupt();
    Read_Finish = Txd_Finish;
    Txd_Finish = sdt_false;
    __enable_interrupt();
    return(Read_Finish);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//启动发送硬件
//-------------------------------------------------------------------------------------------------
void Activate_PHYTxd(void)
{
    if(SET != USART_GetITStatus(USART2,USART_IT_TXE))
    {
        USART_ITConfig(USART2,USART_IT_TXE,ENABLE); 
        USART_ITConfig(USART2,USART_IT_TC,ENABLE);        
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
sdt_bool Pull_OneByteFromPHY(sdt_int8u* _Out_Byte)
{
    sdt_int8u Read_Byte,NBytes;
    
    __disable_interrupt();
    _Get_my_fifo_byte(Uart_Rxd,NBytes);
    __enable_interrupt();
    if(NBytes)
    {
        __disable_interrupt();
        _Out_my_fifo(Uart_Rxd,Read_Byte);
        __enable_interrupt();
        
        *_Out_Byte = Read_Byte;
        return(sdt_true);
    }
    return(sdt_false);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
sdt_bool Pull_TransmitPHY_IsReady(void)
{
    sdt_int8u Fifo_Status;
    
    __disable_interrupt();
    _Get_my_fifo_state(Uart_Txd,Fifo_Status);
    __enable_interrupt();
    if(FIFO_FULL == Fifo_Status)
    {
        return(sdt_false);
    }
    return(sdt_true);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Push_OneByteToPHY(sdt_int8u _In_Byte)
{
    __disable_interrupt();
    _In_my_fifo(Uart_Txd,_In_Byte);
    __enable_interrupt();
    Activate_PHYTxd();
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
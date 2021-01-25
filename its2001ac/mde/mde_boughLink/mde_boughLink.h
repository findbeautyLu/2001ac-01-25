//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//Bough Procotol Link Layer
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef mde_boughLink_H
#define mde_boughLink_H
//------------------------------------------------------------------------------
#ifndef snail_data_types
    #include ".\depend\snail_data_types.h"
#endif
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//定义最大传输数据
#define  MAX_PAYLOAD_LEN     128+6
#define  PREABBLE_LEN        3//7
#define  MAX_REV_BUFFER      (MAX_PAYLOAD_LEN+18)
#define  MAX_TRN_BUFFER      (MAX_PAYLOAD_LEN+PREABBLE_LEN+1+18)
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef struct
{
    sdt_int16u ProcotolType;    //协议类型
    sdt_int16u PayloadLength;   //数据长度  
    union
    {
        struct
        {
            sdt_int8u  LinkDstAddr[6];  //目标地址
            sdt_int8u  LinkSrcAddr[6];  //源地址
            sdt_int8u  Payload[MAX_PAYLOAD_LEN];    //数据
        };
        sdt_int8u raw_payload[MAX_PAYLOAD_LEN+12];
    };
}BoughLinkCommData_Def;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//Task
//名称: 本地链路任务
//功能: 由本地端口支持PHY功能，本模块BSP支持独占UART口为PHY
//入口: 
//      
//出口: 
void Mde_Local_BoughLink_Task(void);
//------------------------------------------------------------------------------
//名称: 远程链路任务
//功能:
//入口: 
//      
//出口: BN_TRUE,校准一次波特率
sdt_bool Mde_Remote_BoughLink_Task(void);
//------------------------------------------------------------------------------
//Event
//------------------------------------------------------------------------------
//Status
//------------------------------------------------------------------------------
//Function
//名称: 获取本地链路一个报文
//功能: 
//入口: 
//      _Out_HaveMessage  是否有报文产生 BN_TRUE--有报文, ---->>
//出口: 报文数据结构体指针

BoughLinkCommData_Def* Pull_Local_MessageFromBoughLink(sdt_bool* _Out_HaveMessage);
//------------------------------------------------------------------------------
//名称: 获取本地链路发送链路是否准备完毕
//功能: 
//入口: 
//      
//出口: BN_TRUE--准备完毕,可以推入报文,

sdt_bool Pull_Local_TransmitIsReady(void);
//------------------------------------------------------------------------------
//名称: 推入一个发送报文到链路层
//功能: 
//入口: 
//      _In_pTransmitData  需要发送数据的结构体指针  <<----
//出口: 

void Push_Local_TransmitMessageToBoughLink(BoughLinkCommData_Def* _In_pTransmitData);
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//名称: 推入一个字节的数据到链路层
//功能: 
//入口: 
//      _In_ReceiveByte  推入的接收字节数据  <<----
//出口: 

void Push_Remote_OneByteToBoughLink(sdt_int8u _In_ReceiveByte);
//------------------------------------------------------------------------------
//名称: 从链路层获取一个字节的发送数据
//功能: 
//入口: 
//      _Out_TransmitByte  获取发送数据的指针  ---->>
//出口: BN_TRUE---获取数据成功
sdt_bool Pull_Remote_TransmitByteFromBoughLink(sdt_int8u* _Out_TransmitByte);
//------------------------------------------------------------------------------
//名称: 获取远程链路一个报文
//功能: 
//入口: 
//      _Out_HaveMessage  是否有报文产生 BN_TRUE--有报文, ---->>
//出口: 报文数据结构体指针
BoughLinkCommData_Def* Pull_Remote_MessageFromBoughLink(sdt_bool* _Out_HaveMessage);
//------------------------------------------------------------------------------
//名称: 推入一个发送报文到链路层
//功能: 
//入口: 
//      _In_pTransmitData  需要发送数据的结构体指针  <<----
//出口: 
void Push_Remote_TransmitMessageToBoughLink(BoughLinkCommData_Def* _In_pTransmitData);
//------------------------------------------------------------------------------
//名称: 获取远程链路发送链路是否准备完毕
//功能: 
//入口: 
//      
//出口: BN_TRUE--准备完毕,可以推入报文,
sdt_bool Pull_Remote_TransmitIsReady(void);
//------------------------------------------------------------------------------
#endif
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

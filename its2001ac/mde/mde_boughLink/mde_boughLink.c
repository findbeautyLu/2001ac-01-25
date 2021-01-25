//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-------------------------------------------------------------------------------------------------
#include ".\depend\bsp_boughLink.h"
#include ".\mde_boughLink.h"
#include "..\..\pbc\pbc_sysTick\pbc_sysTick.h"
#include "..\mde_sysClock\mde_system_clock.h"
#include "..\mde_mRtuSlave\mde_mRtuSlave.h"
#include "..\..\pbc\pbc_crc16\pbc_crc16.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-------------------------------------------------------------------------------------------------
#define SYB_PREAMBLE    0x55
#define SYB_SFD         0xAB
//-------------------------------------------------------------------------------------------------
#define TIMEOUTV        1000
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//Bough 协议链路层处理
//支持多模块化数据处理
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef enum
{
    BLKS_Idle            = 0x00,  //
    BLKS_Preamble        = 0x01,  //接收到头
    RLYS_SFD             = 0x02,  //接收界定符
    RLYS_LenghtHi        = 0x03,  //数据包长度
    RLYS_LenghtLo        = 0x04,
    RLYS_RevingData      = 0x05,  //数据包
    RLYS_RevFinish       = 0x06,  //完成
}BoughLinkStatus_Def;
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define BgLinkBits_NewMessage    0x0001 //新报文
#define BgLinkBits_RevedByte     0x0002 //收一字节数据
#define BgLinkBits_Transmiting   0x0004 //正在发送数据
//-------------------------------------------------------------------------------------------------
//链路层参数结构体
typedef struct
{
    sdt_int16u BgLinkBits;
    sdt_int8u  ReceivedByte; 
    sdt_int8u  ReceiveBuffer[MAX_REV_BUFFER];
    sdt_int16u ReceiveLength;
    sdt_int8u  Rev_Index;
    BoughLinkStatus_Def BoughLinkStatus; 
    BoughLinkCommData_Def  BoughLinkRevData;
    timerClock_def Timer_RevTimeout;
    sdt_int8u TransmistBuffer[MAX_TRN_BUFFER];
    sdt_int8u TransmistLength;
    sdt_int8u TransmistIndex;
    timerClock_def Timer_TransmitTimeout;
}BoughLinkParameter_Def;
//-------------------------------------------------------------------------------------------------
//Bough链路独立处理函数
//-------------------------------------------------------------------------------------------------
static void Alone_BoughLink_Task(BoughLinkParameter_Def* _In_TheLinkParameter)
{
    BoughLinkStatus_Def BoughLinkStatus_Read;
    
    pbc_timerMillRun_task(&_In_TheLinkParameter->Timer_RevTimeout);
    pbc_timerMillRun_task(&_In_TheLinkParameter->Timer_TransmitTimeout);
    
    if(pbc_pull_timerIsOnceTriggered(&_In_TheLinkParameter->Timer_TransmitTimeout))
    {
        _In_TheLinkParameter->TransmistLength = 0;  //发送超时
        _In_TheLinkParameter->TransmistIndex = 0;
    }       
    
    if((BLKS_Idle == _In_TheLinkParameter->BoughLinkStatus) || (BLKS_Preamble == _In_TheLinkParameter->BoughLinkStatus))
    {
    }
    else
    {
        if(pbc_pull_timerIsCompleted(&_In_TheLinkParameter->Timer_RevTimeout))
        {
            _In_TheLinkParameter->BoughLinkStatus = BLKS_Idle;
        }
    }
//---------------------------------------------------------------------------------------------------
    do
    {
        BoughLinkStatus_Read = _In_TheLinkParameter->BoughLinkStatus;

        switch(_In_TheLinkParameter->BoughLinkStatus)
        {
            case BLKS_Idle:
            {
                break;
            }
            case BLKS_Preamble:
            {
                if(BgLinkBits_RevedByte & _In_TheLinkParameter->BgLinkBits)
                {
                    _In_TheLinkParameter->BgLinkBits &= ~BgLinkBits_RevedByte;
                    if(SYB_PREAMBLE == _In_TheLinkParameter->ReceivedByte)
                    {
                        _In_TheLinkParameter->BoughLinkStatus = RLYS_SFD;
                        pbc_reload_timerClock(&_In_TheLinkParameter->Timer_RevTimeout,TIMEOUTV);
                    }                        
                }
                break;
            }
            case RLYS_SFD:
            {
                if(BgLinkBits_RevedByte & _In_TheLinkParameter->BgLinkBits)
                {
                    _In_TheLinkParameter->BgLinkBits &= ~BgLinkBits_RevedByte;
                    if(SYB_SFD == _In_TheLinkParameter->ReceivedByte)
                    {
                        _In_TheLinkParameter->BoughLinkStatus = RLYS_LenghtHi;
                        pbc_reload_timerClock(&_In_TheLinkParameter->Timer_RevTimeout,TIMEOUTV);
                    }
                }
                break;
            }
            case RLYS_LenghtHi:
            {
                if(BgLinkBits_RevedByte & _In_TheLinkParameter->BgLinkBits)
                {
                    _In_TheLinkParameter->BgLinkBits &= ~BgLinkBits_RevedByte;
                    
                    _In_TheLinkParameter->ReceiveLength = ((sdt_int16u)_In_TheLinkParameter->ReceivedByte);
                    _In_TheLinkParameter->ReceiveLength = _In_TheLinkParameter->ReceiveLength<<8;
                    _In_TheLinkParameter->BoughLinkStatus = RLYS_LenghtLo;
                    _In_TheLinkParameter->ReceiveBuffer[0] = _In_TheLinkParameter->ReceivedByte;                    
                }
                break;
            }
            case RLYS_LenghtLo:
            {
                if(BgLinkBits_RevedByte & _In_TheLinkParameter->BgLinkBits)
                {
                    _In_TheLinkParameter->BgLinkBits &= ~BgLinkBits_RevedByte;
                    
                    _In_TheLinkParameter->ReceiveLength |= ((sdt_int16u)_In_TheLinkParameter->ReceivedByte)&0x00FF;
                    
                    if(_In_TheLinkParameter->ReceiveLength > MAX_REV_BUFFER)  //长度溢出
                    {
                        _In_TheLinkParameter->BoughLinkStatus = BLKS_Idle;
                    }
                    else
                    {
                        _In_TheLinkParameter->BoughLinkStatus = RLYS_RevingData;
                        _In_TheLinkParameter->ReceiveBuffer[1] = _In_TheLinkParameter->ReceivedByte;
                        _In_TheLinkParameter->Rev_Index = 2;                           
                    }
             
                }
                break;
            }
            case RLYS_RevingData:
            {
                if(BgLinkBits_RevedByte & _In_TheLinkParameter->BgLinkBits)
                {
                    _In_TheLinkParameter->BgLinkBits &= ~BgLinkBits_RevedByte;
                    
                    _In_TheLinkParameter->ReceiveBuffer[_In_TheLinkParameter->Rev_Index] = _In_TheLinkParameter->ReceivedByte;
                    if((_In_TheLinkParameter->Rev_Index + 1) >= _In_TheLinkParameter->ReceiveLength)
                    {
                        _In_TheLinkParameter->BoughLinkStatus = RLYS_RevFinish;
                    }                    
                    _In_TheLinkParameter->Rev_Index++;              
                }
                break;
            }
            case RLYS_RevFinish:
            {
                if(_In_TheLinkParameter->ReceiveLength < 18)  //最少18个字节
                {
                    _In_TheLinkParameter->BoughLinkStatus = BLKS_Idle;
                }
                else
                {//CRC校验,生产一个数据包
                    sdt_int8u Calculate_CRC[2];
                    
                    Crc16CalculateOfByte(&_In_TheLinkParameter->ReceiveBuffer[0],(_In_TheLinkParameter->ReceiveLength-2),&Calculate_CRC[0]);
                    if((Calculate_CRC[0] == _In_TheLinkParameter->ReceiveBuffer[_In_TheLinkParameter->ReceiveLength-2]) &&\
                       (Calculate_CRC[1] == _In_TheLinkParameter->ReceiveBuffer[_In_TheLinkParameter->ReceiveLength-1]))
                    {
                        _In_TheLinkParameter->BgLinkBits |= BgLinkBits_NewMessage;
                        
                        _In_TheLinkParameter->BoughLinkRevData.ProcotolType = _In_TheLinkParameter->ReceiveBuffer[2];
                        _In_TheLinkParameter->BoughLinkRevData.ProcotolType = _In_TheLinkParameter->BoughLinkRevData.ProcotolType <<8;
                        _In_TheLinkParameter->BoughLinkRevData.ProcotolType |= (sdt_int16u)_In_TheLinkParameter->ReceiveBuffer[3] & 0x00FF;
                        
                        _In_TheLinkParameter->BoughLinkRevData.LinkDstAddr[0] =  _In_TheLinkParameter->ReceiveBuffer[4];
                        _In_TheLinkParameter->BoughLinkRevData.LinkDstAddr[1] =  _In_TheLinkParameter->ReceiveBuffer[5];
                        _In_TheLinkParameter->BoughLinkRevData.LinkDstAddr[2] =  _In_TheLinkParameter->ReceiveBuffer[6];
                        _In_TheLinkParameter->BoughLinkRevData.LinkDstAddr[3] =  _In_TheLinkParameter->ReceiveBuffer[7];
                        _In_TheLinkParameter->BoughLinkRevData.LinkDstAddr[4] =  _In_TheLinkParameter->ReceiveBuffer[8];
                        _In_TheLinkParameter->BoughLinkRevData.LinkDstAddr[5] =  _In_TheLinkParameter->ReceiveBuffer[9];
                        
                        _In_TheLinkParameter->BoughLinkRevData.LinkSrcAddr[0] =  _In_TheLinkParameter->ReceiveBuffer[10];
                        _In_TheLinkParameter->BoughLinkRevData.LinkSrcAddr[1] =  _In_TheLinkParameter->ReceiveBuffer[11];
                        _In_TheLinkParameter->BoughLinkRevData.LinkSrcAddr[2] =  _In_TheLinkParameter->ReceiveBuffer[12];
                        _In_TheLinkParameter->BoughLinkRevData.LinkSrcAddr[3] =  _In_TheLinkParameter->ReceiveBuffer[13];
                        _In_TheLinkParameter->BoughLinkRevData.LinkSrcAddr[4] =  _In_TheLinkParameter->ReceiveBuffer[14];
                        _In_TheLinkParameter->BoughLinkRevData.LinkSrcAddr[5] =  _In_TheLinkParameter->ReceiveBuffer[15];
                        
                        _In_TheLinkParameter->BoughLinkRevData.PayloadLength = _In_TheLinkParameter->ReceiveLength-18;
                        sdt_int16u i;
                        for(i = 0;i < (_In_TheLinkParameter->BoughLinkRevData.PayloadLength);i++)
                        {
                            _In_TheLinkParameter->BoughLinkRevData.Payload[i] = _In_TheLinkParameter->ReceiveBuffer[i+16];
                        }
                        
                    }
                    _In_TheLinkParameter->BoughLinkStatus = BLKS_Idle;
                }
                break;
            }
            default:
            {
                break;
            }
        }
    }while(BoughLinkStatus_Read != _In_TheLinkParameter->BoughLinkStatus);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//名称: 压入一个接收字节到链路模块
//功能: 
//入口: 
//      _In_TheLinkParameter   链路模块地址     <<----
//     _Out_Byte               需要发送的数据   ---->>
//出口: sdt_true--有数据传出,BN_FASLE--无数据传出
//-------------------------------------------------------------------------------------------------
static void Push_Alone_OneByteToBoughLink(BoughLinkParameter_Def* _In_TheLinkParameter,sdt_int8u _In_Byte)
{
    _In_TheLinkParameter->BgLinkBits |= BgLinkBits_RevedByte;
    _In_TheLinkParameter->ReceivedByte = _In_Byte;
    Alone_BoughLink_Task(_In_TheLinkParameter);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//名称: 获取接收报文
//功能: 
//入口: 
//      _In_TheLinkParameter      链路模块地址                      <<----
//      _Out_HaveMessage         sdt_true--有报文,BN_FASLE--无报文   ---->>
//出口: 接收到数据的地址指针
//-------------------------------------------------------------------------------------------------
static BoughLinkCommData_Def* Pull_Alone_MessageFromBoughLink(BoughLinkParameter_Def* _In_TheLinkParameter,sdt_bool* _Out_HaveMessage)
{
    if(BgLinkBits_NewMessage & _In_TheLinkParameter->BgLinkBits)
    {
        _In_TheLinkParameter->BgLinkBits &= ~BgLinkBits_NewMessage;
        *_Out_HaveMessage = sdt_true;
    }
    else
    {
        *_Out_HaveMessage  =sdt_false;
    }
    return(&_In_TheLinkParameter->BoughLinkRevData);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
static void Push_Alone_TransmitMessageToBoughLink(BoughLinkParameter_Def* _In_TheLinkParameter,BoughLinkCommData_Def* _In_pTransmitData)
{
    sdt_int16u Data_Length;
    
    if(_In_pTransmitData->PayloadLength > MAX_PAYLOAD_LEN)
    {
        while(1);
    }
    else if(_In_TheLinkParameter->TransmistIndex < _In_TheLinkParameter->TransmistLength)
    {//上次数据未处理完毕,不再处理本次报文
    }
    else
    {
        sdt_int16u PandSFD_Index = 0;

        while(PandSFD_Index < (PREABBLE_LEN+1))
        {
            if(PandSFD_Index == PREABBLE_LEN)
            {
                _In_TheLinkParameter->TransmistBuffer[PandSFD_Index] = SYB_SFD;
            }
            else
            {
                _In_TheLinkParameter->TransmistBuffer[PandSFD_Index] = SYB_PREAMBLE;
            }
            PandSFD_Index++;
        }

        Data_Length = _In_pTransmitData->PayloadLength + 18;
        _In_TheLinkParameter->TransmistBuffer[PandSFD_Index+0] = Data_Length>>8;
        _In_TheLinkParameter->TransmistBuffer[PandSFD_Index+1] = Data_Length;
        
        _In_TheLinkParameter->TransmistBuffer[PandSFD_Index+2] = _In_pTransmitData->ProcotolType>>8;
        _In_TheLinkParameter->TransmistBuffer[PandSFD_Index+3] = _In_pTransmitData->ProcotolType;
        
        _In_TheLinkParameter->TransmistBuffer[PandSFD_Index+4] = _In_pTransmitData->LinkDstAddr[0];
        _In_TheLinkParameter->TransmistBuffer[PandSFD_Index+5] = _In_pTransmitData->LinkDstAddr[1];
        _In_TheLinkParameter->TransmistBuffer[PandSFD_Index+6] = _In_pTransmitData->LinkDstAddr[2];
        _In_TheLinkParameter->TransmistBuffer[PandSFD_Index+7] = _In_pTransmitData->LinkDstAddr[3];
        _In_TheLinkParameter->TransmistBuffer[PandSFD_Index+8] = _In_pTransmitData->LinkDstAddr[4];
        _In_TheLinkParameter->TransmistBuffer[PandSFD_Index+9] = _In_pTransmitData->LinkDstAddr[5];
        
        _In_TheLinkParameter->TransmistBuffer[PandSFD_Index+10] = _In_pTransmitData->LinkSrcAddr[0];
        _In_TheLinkParameter->TransmistBuffer[PandSFD_Index+11] = _In_pTransmitData->LinkSrcAddr[1];
        _In_TheLinkParameter->TransmistBuffer[PandSFD_Index+12] = _In_pTransmitData->LinkSrcAddr[2];
        _In_TheLinkParameter->TransmistBuffer[PandSFD_Index+13] = _In_pTransmitData->LinkSrcAddr[3];
        _In_TheLinkParameter->TransmistBuffer[PandSFD_Index+14] = _In_pTransmitData->LinkSrcAddr[4];
        _In_TheLinkParameter->TransmistBuffer[PandSFD_Index+15] = _In_pTransmitData->LinkSrcAddr[5];
        
        sdt_int16u i;
        for(i = 0; i < _In_pTransmitData->PayloadLength;i++)
        {
            _In_TheLinkParameter->TransmistBuffer[(PandSFD_Index+16)+i] = _In_pTransmitData->Payload[i];
        }
        Crc16CalculateOfByte(&_In_TheLinkParameter->TransmistBuffer[PandSFD_Index+0],(Data_Length-2),&_In_TheLinkParameter->TransmistBuffer[(PandSFD_Index+16)+_In_pTransmitData->PayloadLength]);
        
        _In_TheLinkParameter->TransmistLength = PandSFD_Index+Data_Length;  //需要传送的字节数
        _In_TheLinkParameter->TransmistIndex = 0;
        pbc_reload_timerClock(&_In_TheLinkParameter->Timer_TransmitTimeout,TIMEOUTV);

    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//名称: 从链路模块取出一个待发送的字节数据
//功能: 
//入口: 
//      _In_TheLinkParameter   链路模块地址     <<----
//     _Out_Byte               需要发送的数据   ---->>
//出口: sdt_true--有数据传出,BN_FASLE--无数据传出
//-------------------------------------------------------------------------------------------------
static sdt_bool Pull_Alone_TransmitByteFromBoughLink(BoughLinkParameter_Def* _In_TheLinkParameter,sdt_int8u* _Out_Byte)
{
    if(_In_TheLinkParameter->TransmistIndex < _In_TheLinkParameter->TransmistLength)
    {
        *_Out_Byte = _In_TheLinkParameter->TransmistBuffer[_In_TheLinkParameter->TransmistIndex];
        _In_TheLinkParameter->TransmistIndex++;
        return(sdt_true);
    }
    else
    {       
        return(sdt_false);
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
static BoughLinkParameter_Def LocalBoughLink_Solid;   //创建本地实体,由本模块的BSP支持PHY功能
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void Mde_Local_BoughLink_Task(void)
{
    macro_createTimer(Timer_RcfBR_s,timerType_second,5);
    static sdt_bool Cfged = sdt_false;
  
    if(Cfged)
    {
        Alone_BoughLink_Task(&LocalBoughLink_Solid);
        if(BLKS_Idle == LocalBoughLink_Solid.BoughLinkStatus)
        {
            Recfg_Buadrate_USART3(mde_pull_hisFrequency());//校准波特率
            pbc_reload_timerClock(&Timer_RcfBR_s,60);
            LocalBoughLink_Solid.BoughLinkStatus = BLKS_Preamble;  //启动接收
        }
        
        pbc_timerClockRun_task(&Timer_RcfBR_s);
        if(pbc_pull_timerIsCompleted(&Timer_RcfBR_s))
        {
            Recfg_Buadrate_USART3(mde_pull_hisFrequency());//校准波特率mde_pull_hisFrequency
            pbc_reload_timerClock(&Timer_RcfBR_s,60);;
        }
        
        while(1)
        {
            sdt_int8u ReceiveByte;
            
            if(Pull_OneByteFromPHY(&ReceiveByte))
            {
                Push_Alone_OneByteToBoughLink(&LocalBoughLink_Solid,ReceiveByte);  //接收处理
            } 
            else
            {
                break;
            }
        }

        if(BgLinkBits_Transmiting & LocalBoughLink_Solid.BgLinkBits)
        {
            sdt_int8u TransmitByte;
            
            while(1)
            {
                if(Pull_TransmitPHY_IsReady())
                {
                    if(Pull_Alone_TransmitByteFromBoughLink(&LocalBoughLink_Solid,&TransmitByte))
                    {
                        Push_OneByteToPHY(TransmitByte);
                    }
                    else
                    {
                        if(Pull_PHYTxd_Finish()) //发送完毕,转接收
                        {
                            LocalBoughLink_Solid.BgLinkBits &= ~BgLinkBits_Transmiting;
                            Enter_ReceiveMode_PHY();
                        }
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
        }
    }
    else
    {
        Uart2_Configure();
        Cfged = sdt_true;
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-------------------------------------------------------------------------------------------------
BoughLinkCommData_Def* Pull_Local_MessageFromBoughLink(sdt_bool* _Out_HaveMessage)
{
    return(Pull_Alone_MessageFromBoughLink(&LocalBoughLink_Solid,_Out_HaveMessage));
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-------------------------------------------------------------------------------------------------
void Push_Local_TransmitMessageToBoughLink(BoughLinkCommData_Def* _In_pTransmitData)
{
    Push_Alone_TransmitMessageToBoughLink(&LocalBoughLink_Solid,_In_pTransmitData);
    LocalBoughLink_Solid.BgLinkBits |= BgLinkBits_Transmiting;
    Enter_TransmitMode_PHY();
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-------------------------------------------------------------------------------------------------
sdt_bool Pull_Local_TransmitIsReady(void)
{
    if(LocalBoughLink_Solid.TransmistIndex < LocalBoughLink_Solid.TransmistLength)
    {
        return(sdt_false);
    }
    else
    {
        return(sdt_true);
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
static BoughLinkParameter_Def Remote_BoughLink_Solid;  //远程链路实体，和其他模块共用PHY硬件端口
//-------------------------------------------------------------------------------------------------
//sdt_true --校准一次波特率
//-------------------------------------------------------------------------------------------------
sdt_bool Mde_Remote_BoughLink_Task(void)
{
    Alone_BoughLink_Task(&Remote_BoughLink_Solid);
    
    if(BLKS_Idle == Remote_BoughLink_Solid.BoughLinkStatus)
    {
        Remote_BoughLink_Solid.BoughLinkStatus = BLKS_Preamble;  //启动接收
        return(sdt_true);                                         //校准一次波特率
    }
    else
    {
        return(sdt_false);
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//压入一个数据到远程链路模块
//-------------------------------------------------------------------------------------------------
void Push_Remote_OneByteToBoughLink(sdt_int8u _In_ReceiveByte)
{
    Push_Alone_OneByteToBoughLink(&Remote_BoughLink_Solid,_In_ReceiveByte); 
}
//-------------------------------------------------------------------------------------------------
//从链路读取一个发送的数据
//-------------------------------------------------------------------------------------------------
sdt_bool Pull_Remote_TransmitByteFromBoughLink(sdt_int8u* _Out_TransmitByte)
{
    return(Pull_Alone_TransmitByteFromBoughLink(&Remote_BoughLink_Solid,_Out_TransmitByte));
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-------------------------------------------------------------------------------------------------
BoughLinkCommData_Def* Pull_Remote_MessageFromBoughLink(sdt_bool* _Out_HaveMessage)
{
    return(Pull_Alone_MessageFromBoughLink(&Remote_BoughLink_Solid,_Out_HaveMessage));
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-------------------------------------------------------------------------------------------------
void Push_Remote_TransmitMessageToBoughLink(BoughLinkCommData_Def* _In_pTransmitData)
{
    Push_Alone_TransmitMessageToBoughLink(&Remote_BoughLink_Solid,_In_pTransmitData);
    Remote_BoughLink_Solid.TransmistIndex =  Remote_BoughLink_Solid.TransmistLength;
    mRtu_remote_answer_event(MODBUS_REMOTE,Remote_BoughLink_Solid.TransmistBuffer,Remote_BoughLink_Solid.TransmistLength);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-------------------------------------------------------------------------------------------------
sdt_bool Pull_Remote_TransmitIsReady(void)
{
    if(Remote_BoughLink_Solid.TransmistIndex < Remote_BoughLink_Solid.TransmistLength)
    {
        return(sdt_false);
    }
    else
    {
        return(sdt_true);
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

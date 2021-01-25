//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include ".\mde_masterSlave.h"
#include "..\..\pbc\pbc_dataConvert\pbc_dataConvert.h"
#include "..\..\pbc\pbc_crc16\pbc_crc16.h"
#include "..\..\pbc\pbc_sysTick\pbc_sysTick.h"
#include ".\depend\bsp_masterSlave.h"
#include "stdlib.h"
//-----------------------------------------------------------------------------
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//modbus 主机模块,负责报文的发送和接收控制
//支持命令: 0x03 0x06 0x10 0x17
//支持多个实例
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-------------------------------------------------------------------------------------------------
#define MS_PREAMBLE    0x55
#define MS_SFD         0xAB
//-------------------------------------------------------------------------------------------------
#define REV_TIMEOUTV        1000
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//主从对等协议链路层处理
//支持多模块化数据处理
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef enum
{
    MS_Idle            = 0x00,  //
    MS_Preamble        = 0x01,  //接收到头
    MS_Sfd             = 0x02,  //接收界定符
    MS_LenghtHi        = 0x03,  //数据包长度
    MS_LenghtLo        = 0x04,
    MS_RevingData      = 0x05,  //数据包
    MS_RevFinish       = 0x06,  //完成
}ms_link_status_def;
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define MSLinkBits_NewMessage      0x0001 //新报文
#define MSLinkBits_CheckMessage    0x0008 //侦测报文
#define MSLinkBits_RevedByte       0x0002 //收一字节数据
#define MSLinkBits_Transmiting     0x0004 //正在发送数据
//-------------------------------------------------------------------------------------------------
//modbus操作结构体
//-----------------------------------------------------------------------------
typedef struct
{
    mRtu_master_slave_status_def      mmRtu_status;//接收狀態
    modbus_master_slave_runState_def  mmoo_runStutus;//運行狀態
    ms_link_comm_data_def msLinkRevData;
    ms_link_status_def    msLinkStatus;
    sdt_int16u msLinkBits;
    sdt_int8u  receivedByte; 
    sdt_int8u  localID[6];
    sdt_int8u  receive_buff[MAX_BUFF_SIZE];
    sdt_int8u  rev_index;
    sdt_int16u receiveLength;
    sdt_int8u  transmit_buff[MAX_BUFF_SIZE];
    sdt_int8u  transmit_length;
    sdt_int8u  transmit_index;
    timerClock_def revConflictDealy; //冲突延时(非本地ID，冲突检测帧，发送冲突)
    timerClock_def  transmitWaitDealy;//发送等待
    timerClock_def timer_revTimeOut;
    timerClock_def timer_transmitTimeout;
    sdt_bool    weekPullUpFlag;//弱上拉
    sdt_bool    responseFlag;//响应标志位
    void (*phy_into_receive)(void);
    sdt_bool (*pull_receive_byte)(sdt_int8u* out_rByte);
    void (*bsp_ms_check_phy_bus)(void);
    sdt_bool(*bsp_ms_pull_phyBus_status)(void);
    sdt_int8u (*bsp_ms_pull_rxd_num)(void);
    void (*phy_into_transmit_status)(sdt_bool in_weekPullUp);
    sdt_bool (*push_transmit_byte)(sdt_int8u in_tByte);
    sdt_bool (*pull_transmit_complete)(void);
}modbus_master_slave_oper_def;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//sdt_bool modbus_master_slave_receive_protocol(modbus_master_slave_oper_def* mix_mm_oper);
//--------------------------------------------------------------------------------------------------------------------------
sdt_int16u pull_random_time(void)
{//20-3020ms的随机数
    sdt_int16u result = 0;
 //   result = (rand()&0x64)*20+ 500;/* random factor 0-47*/     
    result = (rand()&0x0190)*20+ 300;/* random factor 0-47*/      
    return  result;
}
//-------------------------------------------------------------------------------------------------
//主从对等链路独立处理函数
//-------------------------------------------------------------------------------------------------
static void Alone_msLink_Task(modbus_master_slave_oper_def* in_theLinkParameter)
{
    ms_link_status_def msLinkStatus_Read;
    
    pbc_timerMillRun_task(&in_theLinkParameter->timer_revTimeOut);
    pbc_timerMillRun_task(&in_theLinkParameter->timer_transmitTimeout);
    
    if(pbc_pull_timerIsOnceTriggered(&in_theLinkParameter->timer_transmitTimeout))
    {
        in_theLinkParameter->transmit_length = 0;  //发送超时
        in_theLinkParameter->transmit_index = 0;
    }       
    
    if((MS_Idle == in_theLinkParameter->msLinkStatus) || (MS_Preamble == in_theLinkParameter->msLinkStatus))
    {
    }
    else
    {
        if(pbc_pull_timerIsCompleted(&in_theLinkParameter->timer_revTimeOut))
        {
            in_theLinkParameter->msLinkStatus = MS_Idle;
        }
    }
//---------------------------------------------------------------------------------------------------
    do
    {
        msLinkStatus_Read = in_theLinkParameter->msLinkStatus;

        switch(in_theLinkParameter->msLinkStatus)
        {
            case MS_Idle:
            {
                break;
            }
            case MS_Preamble:
            {
                if(MSLinkBits_RevedByte & in_theLinkParameter->msLinkBits)
                {
                    in_theLinkParameter->msLinkBits &= ~MSLinkBits_RevedByte;
                    if(MS_PREAMBLE == in_theLinkParameter->receivedByte)
                    {
                        in_theLinkParameter->msLinkStatus = MS_Sfd;
                        pbc_reload_timerClock(&in_theLinkParameter->timer_revTimeOut,REV_TIMEOUTV);
                    }                        
                }
                break;
            }
            case MS_Sfd:
            {
                if(MSLinkBits_RevedByte & in_theLinkParameter->msLinkBits)
                {
                    in_theLinkParameter->msLinkBits &= ~MSLinkBits_RevedByte;
                    if(MS_SFD == in_theLinkParameter->receivedByte)
                    {
                        in_theLinkParameter->msLinkStatus = MS_LenghtHi;
                        pbc_reload_timerClock(&in_theLinkParameter->timer_revTimeOut,REV_TIMEOUTV);
                    }
                }
                break;
            }
            case MS_LenghtHi:
            {
                if(MSLinkBits_RevedByte & in_theLinkParameter->msLinkBits)
                {
                    in_theLinkParameter->msLinkBits &= ~MSLinkBits_RevedByte;
                    
                    in_theLinkParameter->receiveLength = ((sdt_int16u)in_theLinkParameter->receivedByte);
                    in_theLinkParameter->receiveLength = in_theLinkParameter->receiveLength<<8;
                    in_theLinkParameter->msLinkStatus = MS_LenghtLo;
                    in_theLinkParameter->receive_buff[0] = in_theLinkParameter->receivedByte;                    
                }
                break;
            }
            case MS_LenghtLo:
            {
                if(MSLinkBits_RevedByte & in_theLinkParameter->msLinkBits)
                {
                    in_theLinkParameter->msLinkBits &= ~MSLinkBits_RevedByte;
                    
                    in_theLinkParameter->receiveLength |= ((sdt_int16u)in_theLinkParameter->receivedByte)&0x00FF;
                    
                    if(in_theLinkParameter->receiveLength > MAX_BUFF_SIZE)  //长度溢出
                    {
                        in_theLinkParameter->msLinkBits = MS_Idle;
                    }
                    else
                    {
                        in_theLinkParameter->msLinkStatus = MS_RevingData;
                        in_theLinkParameter->receive_buff[1] = in_theLinkParameter->receivedByte;
                        in_theLinkParameter->rev_index = 2;                           
                    }
             
                }
                break;
            }
            case MS_RevingData:
            {
                if(MSLinkBits_RevedByte & in_theLinkParameter->msLinkBits)
                {
                    in_theLinkParameter->msLinkBits &= ~MSLinkBits_RevedByte;
                    
                    in_theLinkParameter->receive_buff[in_theLinkParameter->rev_index] = in_theLinkParameter->receivedByte;
                    if((in_theLinkParameter->rev_index + 1) >= in_theLinkParameter->receiveLength)
                    {
                        in_theLinkParameter->msLinkStatus = MS_RevFinish;
                    }                    
                    in_theLinkParameter->rev_index++;              
                }
                break;
            }
            case MS_RevFinish:
            {
                if(in_theLinkParameter->receiveLength < 10)  //最少10个字节
                {
                    in_theLinkParameter->msLinkStatus = MS_Idle;
                }
                else
                {//CRC校验,生产一个数据包
                    sdt_int8u Calculate_CRC[2];
                    
                    Crc16CalculateOfByte(&in_theLinkParameter->receive_buff[0],(in_theLinkParameter->receiveLength-2),&Calculate_CRC[0]);
                    if((Calculate_CRC[1] == in_theLinkParameter->receive_buff[in_theLinkParameter->receiveLength-2]) &&\
                       (Calculate_CRC[0] == in_theLinkParameter->receive_buff[in_theLinkParameter->receiveLength-1]))
                    {
                        if(in_theLinkParameter->receiveLength > 18)
                        {                                                                            
                            in_theLinkParameter->msLinkRevData.LinkDstAddr[0] =  in_theLinkParameter->receive_buff[4];
                            in_theLinkParameter->msLinkRevData.LinkDstAddr[1] =  in_theLinkParameter->receive_buff[5];
                            in_theLinkParameter->msLinkRevData.LinkDstAddr[2] =  in_theLinkParameter->receive_buff[6];
                            in_theLinkParameter->msLinkRevData.LinkDstAddr[3] =  in_theLinkParameter->receive_buff[7];
                            in_theLinkParameter->msLinkRevData.LinkDstAddr[4] =  in_theLinkParameter->receive_buff[8];
                            in_theLinkParameter->msLinkRevData.LinkDstAddr[5] =  in_theLinkParameter->receive_buff[9];
                            
                            
                            if(((in_theLinkParameter->msLinkRevData.LinkDstAddr[0] == in_theLinkParameter->localID[0])&&
                            (in_theLinkParameter->msLinkRevData.LinkDstAddr[1] == in_theLinkParameter->localID[1])&&
                            (in_theLinkParameter->msLinkRevData.LinkDstAddr[2] == in_theLinkParameter->localID[2])&&
                            (in_theLinkParameter->msLinkRevData.LinkDstAddr[3] == in_theLinkParameter->localID[3])&&
                            (in_theLinkParameter->msLinkRevData.LinkDstAddr[4] == in_theLinkParameter->localID[4])&&
                            (in_theLinkParameter->msLinkRevData.LinkDstAddr[5] == in_theLinkParameter->localID[5])) ||
                             ((in_theLinkParameter->msLinkRevData.LinkDstAddr[0] == 0xff)&&
                            (in_theLinkParameter->msLinkRevData.LinkDstAddr[1] == 0xff)&&
                            (in_theLinkParameter->msLinkRevData.LinkDstAddr[2] == 0xff)&&
                            (in_theLinkParameter->msLinkRevData.LinkDstAddr[3] == 0xff)&&
                            (in_theLinkParameter->msLinkRevData.LinkDstAddr[4] == 0xff)&&
                            (in_theLinkParameter->msLinkRevData.LinkDstAddr[5] == 0xff)) )
                            {//自己ID或者广播帧认为收到了一帧数据
                                in_theLinkParameter->msLinkBits |= MSLinkBits_NewMessage;    
                                in_theLinkParameter->msLinkRevData.ProcotolType = in_theLinkParameter->receive_buff[2];
                                in_theLinkParameter->msLinkRevData.ProcotolType = in_theLinkParameter->msLinkRevData.ProcotolType <<8;
                                in_theLinkParameter->msLinkRevData.ProcotolType |= (sdt_int16u)in_theLinkParameter->receive_buff[3] & 0x00FF;
                                in_theLinkParameter->msLinkRevData.LinkSrcAddr[0] =  in_theLinkParameter->receive_buff[10];
                                in_theLinkParameter->msLinkRevData.LinkSrcAddr[1] =  in_theLinkParameter->receive_buff[11];
                                in_theLinkParameter->msLinkRevData.LinkSrcAddr[2] =  in_theLinkParameter->receive_buff[12];
                                in_theLinkParameter->msLinkRevData.LinkSrcAddr[3] =  in_theLinkParameter->receive_buff[13];
                                in_theLinkParameter->msLinkRevData.LinkSrcAddr[4] =  in_theLinkParameter->receive_buff[14];
                                in_theLinkParameter->msLinkRevData.LinkSrcAddr[5] =  in_theLinkParameter->receive_buff[15]; 
                                in_theLinkParameter->msLinkRevData.PayloadLength = in_theLinkParameter->receiveLength-18;
                                sdt_int16u i;
                                for(i = 0;i < (in_theLinkParameter->msLinkRevData.PayloadLength);i++)
                                {
                                    in_theLinkParameter->msLinkRevData.Payload[i] = in_theLinkParameter->receive_buff[i+16];
                                }
                            }
                            else
                            {
                                in_theLinkParameter->mmoo_runStutus = msRunS_conflict_wait;
                                pbc_reload_timerClock(&in_theLinkParameter->revConflictDealy,pull_random_time());
                            }
                            
                        }
                        else
                        {
                            in_theLinkParameter->msLinkBits |= MSLinkBits_CheckMessage;
                            in_theLinkParameter->msLinkRevData.ProcotolType = in_theLinkParameter->receive_buff[2];
                            in_theLinkParameter->msLinkRevData.ProcotolType = in_theLinkParameter->msLinkRevData.ProcotolType <<8;
                            in_theLinkParameter->msLinkRevData.ProcotolType |= (sdt_int16u)in_theLinkParameter->receive_buff[3] & 0x00FF;
                            if(in_theLinkParameter->msLinkRevData.ProcotolType == PROTOL_CHECK_CONFICT)
                            {//冲突检测帧
                                in_theLinkParameter->mmoo_runStutus = msRunS_conflict_wait;
                                pbc_reload_timerClock(&in_theLinkParameter->revConflictDealy,pull_random_time());
                            }
                        }
                    }
                    else
                    {
                        in_theLinkParameter->mmRtu_status = mRtuS_master_slave_poterr;
                    }
                    
                    in_theLinkParameter->msLinkStatus = MS_Idle;
                }
                break;
            }
            default:
            {
                break;
            }
        }
    }while(msLinkStatus_Read != in_theLinkParameter->msLinkStatus);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//名称: 压入一个接收字节到链路模块
//功能: 
//入口: 
//      _In_TheLinkParameter   链路模块地址     <<----
//     _Out_Byte               需要发送的数据   ---->>
//出口: sdt_true--有数据传出,BN_FASLE--无数据传出
//-------------------------------------------------------------------------------------------------
static void Push_Alone_OneByteToMSLink(modbus_master_slave_oper_def* _In_TheLinkParameter,sdt_int8u _In_Byte)
{
    _In_TheLinkParameter->msLinkBits |= MSLinkBits_RevedByte;
    _In_TheLinkParameter->receivedByte = _In_Byte;
    Alone_msLink_Task(_In_TheLinkParameter);
}
//主收发任务模块
//--------------------------------------------------------------------------------------------------------------------------
static void modbus_master_slave_oop_task(modbus_master_slave_oper_def* mix_mm_oper)
{
    sdt_int8u receive_byte;
    pbc_timerMillRun_task(&mix_mm_oper->timer_revTimeOut);  
    pbc_timerMillRun_task(&mix_mm_oper->revConflictDealy);  
    pbc_timerMillRun_task(&mix_mm_oper->transmitWaitDealy);  
    mix_mm_oper->bsp_ms_check_phy_bus();//下降沿检测冲突

    if((mix_mm_oper->mmoo_runStutus == msRunS_transmit_data) ||
         (mix_mm_oper->mmoo_runStutus == msRunS_transmit_stop) ||
           (mix_mm_oper->mmoo_runStutus == msRunS_transmit_end))
    {//发送冲突检测
        
        sdt_int8u receive_num;
        receive_num = mix_mm_oper->bsp_ms_pull_rxd_num();
        while(receive_num)
        {
            if(mix_mm_oper->pull_receive_byte(&receive_byte))
            {
                if(mix_mm_oper->transmit_buff[mix_mm_oper->rev_index] != receive_byte)
                {
                    mix_mm_oper->phy_into_receive();//转入接收状态
                    mix_mm_oper->rev_index = 0;
                    mix_mm_oper->mmoo_runStutus = msRunS_conflict_wait; // 冲突等待
                    pbc_reload_timerClock(&mix_mm_oper->revConflictDealy,pull_random_time());
                }
                mix_mm_oper->rev_index++;
                receive_num--;
            }
        }
    }

    switch(mix_mm_oper->mmoo_runStutus)
    {
        case msRunS_idle:
        case msRunS_conflict_wait:   
        {
            if(mix_mm_oper->pull_receive_byte(&receive_byte))
            {
                if(mix_mm_oper->msLinkStatus == MS_Idle)
                {
                    mix_mm_oper->msLinkStatus = MS_Preamble;//启动接收
                }
                Push_Alone_OneByteToMSLink(mix_mm_oper,receive_byte);
                mix_mm_oper->mmoo_runStutus = msRunS_receive_wait;//转入接收等待，设置超时
                pbc_reload_timerClock(&mix_mm_oper->timer_revTimeOut,100); //100ms timeout
            }
            break;
        }

//        case msRunS_conflict_wait:
//        {
//            if(pbc_pull_timerIsCompleted(&mix_mm_oper->revConflictDealy))
//            {
//                mix_mm_oper->mmoo_runStutus = msRunS_idle;
//            }
//            break;
//        }
        case msRunS_transmit_respose_str:
        {
            mix_mm_oper->phy_into_transmit_status(mix_mm_oper->weekPullUpFlag);
            mix_mm_oper->mmoo_runStutus = msRunS_transmit_wait;
            pbc_reload_timerClock(&mix_mm_oper->transmitWaitDealy,35);
            break;
        }
        case msRunS_transmit_str:
        {
            if(pbc_pull_timerIsCompleted(&mix_mm_oper->revConflictDealy))
            {
                mix_mm_oper->phy_into_transmit_status(mix_mm_oper->weekPullUpFlag);
                mix_mm_oper->mmoo_runStutus = msRunS_transmit_wait;
                pbc_reload_timerClock(&mix_mm_oper->transmitWaitDealy,35);
            }
            break;
        }
        case msRunS_transmit_wait:
        {
            if(pbc_pull_timerIsCompleted(&mix_mm_oper->transmitWaitDealy))
            {
                mix_mm_oper->mmoo_runStutus = msRunS_transmit_data;
                mix_mm_oper->transmit_index = 0;
                mix_mm_oper->rev_index = 0;
            }
            break;
        }
        case msRunS_transmit_data:
        {
            if(mix_mm_oper->transmit_length != 0)
            {
                sdt_bool push_succeed;
                while(1)
                {
                    push_succeed = mix_mm_oper->push_transmit_byte(mix_mm_oper->transmit_buff[mix_mm_oper->transmit_index]);
                    if(push_succeed)
                    {
                        mix_mm_oper->transmit_index ++;
                        mix_mm_oper->transmit_length --;
                        if(0 == mix_mm_oper->transmit_length)
                        {
                            mix_mm_oper->mmoo_runStutus = msRunS_transmit_stop;
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }
            break;
        }
        case msRunS_transmit_stop:
        {
            if(mix_mm_oper->pull_transmit_complete())
            {
                mix_mm_oper->mmoo_runStutus = msRunS_transmit_end;
                pbc_reload_timerClock(&mix_mm_oper->transmitWaitDealy,25);
            }
            break;
        }
        case msRunS_transmit_end:
        {
            if(pbc_pull_timerIsCompleted(&mix_mm_oper->transmitWaitDealy))
            {
                mix_mm_oper->phy_into_receive();
                if((mix_mm_oper->weekPullUpFlag) ||(mix_mm_oper->responseFlag == sdt_false) )
                {
                    mix_mm_oper->mmoo_runStutus = msRunS_idle;
                }
                else
                {
                    mix_mm_oper->mmoo_runStutus = msRunS_receive_wait;//转入接收等待，设置超时
                    pbc_reload_timerClock(&mix_mm_oper->timer_revTimeOut,200); //100ms timeout
                }
                         
            }
            break;
        }
        case msRunS_receive_wait:
        {           
            if(mix_mm_oper->pull_receive_byte(&receive_byte))
            {
                if(mix_mm_oper->msLinkStatus == MS_Idle)
                {
                    mix_mm_oper->msLinkStatus = MS_Preamble;//启动接收
                }
                Push_Alone_OneByteToMSLink(mix_mm_oper,receive_byte);
                mix_mm_oper->mmoo_runStutus = msRunS_receive_data;
            }
            else if(pbc_pull_timerIsCompleted(&mix_mm_oper->timer_revTimeOut))
            {
                mix_mm_oper->mmRtu_status = mRtuS_master_slave_timeout;  //超时
                mix_mm_oper->mmoo_runStutus = msRunS_idle;
            }
            break;
        }
        case msRunS_receive_data:
        {            
            while(1)
            {
                sdt_int8u receive_byte;
                if(mix_mm_oper->pull_receive_byte(&receive_byte))
                {
                    Push_Alone_OneByteToMSLink(mix_mm_oper,receive_byte);
                    pbc_reload_timerClock(&mix_mm_oper->timer_revTimeOut,100); //100ms timeout
                }
                else
                {
                    if(mix_mm_oper->msLinkBits & MSLinkBits_NewMessage)
                    {
                        mix_mm_oper->mmRtu_status = mRtuS_master_slave_complete;
                        mix_mm_oper->mmoo_runStutus = msRunS_receive_end;//接收结束
                        break;
                    }                  
                    else if(mix_mm_oper->msLinkBits & MSLinkBits_CheckMessage)
                    {
                        mix_mm_oper->mmRtu_status = mRtuS_master_slave_complete;
                        mix_mm_oper->msLinkBits &=  (~MSLinkBits_CheckMessage);
                        break;
                    }
                    else if(pbc_pull_timerIsCompleted(&mix_mm_oper->timer_revTimeOut))
                    {
                        mix_mm_oper->mmRtu_status = mRtuS_master_slave_timeout;  //超时
                        mix_mm_oper->mmoo_runStutus = msRunS_idle;
                        break;
                    }
                }
            }
            break;
        }
        case msRunS_receive_end:
        {
            break;
        }
        default:
        {
            mix_mm_oper->mmoo_runStutus = msRunS_idle;
            break;
        }
    }
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
static void append_crc_to_message(modbus_master_slave_oper_def* mix_mm_oper)
{
    sdt_int8u crc_value[2];
        
    Crc16CalculateOfByte(&mix_mm_oper->transmit_buff[2],(sdt_int16u)(mix_mm_oper->transmit_length-2),&crc_value[0]);//crc
    mix_mm_oper->transmit_buff[mix_mm_oper->transmit_length] = crc_value[1];
    mix_mm_oper->transmit_buff[mix_mm_oper->transmit_length+1] = crc_value[0];
    mix_mm_oper->transmit_length += 2;
}



#include ".\sod_masterSlave.h"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++interface++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//name:mRtu任务
//--------------------------------------------------------------------------------------------------------------------------
void mde_rtu_master_slave_task(void)
{
    static sdt_bool cfged = sdt_false;

    if(cfged)
    {
        sdt_int8u i;
        for(i = 0;i < max_solid;i ++)
        {
            modbus_master_slave_oop_task(&modbus_master_slave_solid[i]);
        }
    }
    else
    {
        cfged = sdt_true;
        modbus_master_slave_solid_cfg();
    }
}



/*fix length 18*/
void mde_master_slave_link_message_transmit(sdt_int8u in_solidNum,sdt_int8u *in_dest_addr,sdt_int16u in_protolType,sdt_bool resposeFlag)
{
    modbus_master_slave_solid[in_solidNum].transmit_buff[0] = MS_PREAMBLE;
    modbus_master_slave_solid[in_solidNum].transmit_buff[1] = MS_SFD;
    modbus_master_slave_solid[in_solidNum].transmit_buff[4] = (sdt_int8u)(in_protolType>>8);
    modbus_master_slave_solid[in_solidNum].transmit_buff[5] = (sdt_int8u)(in_protolType);
    modbus_master_slave_solid[in_solidNum].transmit_buff[6] = in_dest_addr[0];
    modbus_master_slave_solid[in_solidNum].transmit_buff[7] = in_dest_addr[1];
    modbus_master_slave_solid[in_solidNum].transmit_buff[8] = in_dest_addr[2];
    modbus_master_slave_solid[in_solidNum].transmit_buff[9] = in_dest_addr[3];
    modbus_master_slave_solid[in_solidNum].transmit_buff[10] = in_dest_addr[4];
    modbus_master_slave_solid[in_solidNum].transmit_buff[11] = in_dest_addr[5];
    
    modbus_master_slave_solid[in_solidNum].transmit_buff[12] = modbus_master_slave_solid[in_solidNum].localID[0];
    modbus_master_slave_solid[in_solidNum].transmit_buff[13] = modbus_master_slave_solid[in_solidNum].localID[1];
    modbus_master_slave_solid[in_solidNum].transmit_buff[14] = modbus_master_slave_solid[in_solidNum].localID[2];
    modbus_master_slave_solid[in_solidNum].transmit_buff[15] = modbus_master_slave_solid[in_solidNum].localID[3];
    modbus_master_slave_solid[in_solidNum].transmit_buff[16] = modbus_master_slave_solid[in_solidNum].localID[4];
    modbus_master_slave_solid[in_solidNum].transmit_buff[17] = modbus_master_slave_solid[in_solidNum].localID[5];
    modbus_master_slave_solid[in_solidNum].responseFlag = resposeFlag;
}
/*fix length 12*/
void mde_master_slave_link_message_check_conflict(sdt_int8u in_solidNum,sdt_int16u inType)
{
    modbus_master_slave_solid[in_solidNum].transmit_buff[0] = MS_PREAMBLE;
    modbus_master_slave_solid[in_solidNum].transmit_buff[1] = MS_SFD;
    modbus_master_slave_solid[in_solidNum].transmit_buff[2] = 0x00;
    modbus_master_slave_solid[in_solidNum].transmit_buff[3] = 0x0a;
    modbus_master_slave_solid[in_solidNum].transmit_buff[4] = (sdt_int8u)(inType>>8);
    modbus_master_slave_solid[in_solidNum].transmit_buff[5] = (sdt_int8u)(inType);
    sdt_int16u result;
    result = rand();
    modbus_master_slave_solid[in_solidNum].transmit_buff[6] = (sdt_int8u)(result>>8);
    modbus_master_slave_solid[in_solidNum].transmit_buff[7] = (sdt_int8u)(result);
    result = rand();
    modbus_master_slave_solid[in_solidNum].transmit_buff[8] = (sdt_int8u)(result>>8);
    modbus_master_slave_solid[in_solidNum].transmit_buff[9] = (sdt_int8u)(result);
    modbus_master_slave_solid[in_solidNum].transmit_length = 10;
    modbus_master_slave_solid[in_solidNum].weekPullUpFlag = sdt_true;
    append_crc_to_message(&modbus_master_slave_solid[in_solidNum]);
    modbus_master_slave_solid[in_solidNum].mmoo_runStutus = msRunS_transmit_str;
}

mRtu_master_slave_status_def mde_msRtu_master_slave_reveive_status(sdt_int8u in_solidNum)
{
    if(in_solidNum < max_solid)
    {
         return modbus_master_slave_solid[in_solidNum].mmRtu_status; 
    }
    else
    {
        while(1);
    }
}

modbus_master_slave_runState_def mde_msRtu_master_slave_run_status(sdt_int8u in_solidNum)
{
    if(in_solidNum < max_solid)
    {
        if(modbus_master_slave_solid[in_solidNum].bsp_ms_pull_phyBus_status())
        {//物理冲突
             return msRunS_phyBusy;
        }
        else if(modbus_master_slave_solid[in_solidNum].mmoo_runStutus == msRunS_conflict_wait)
        {
             modbus_master_slave_solid[in_solidNum].mmoo_runStutus =  msRunS_idle;
             return msRunS_conflict_wait;
        }
        return modbus_master_slave_solid[in_solidNum].mmoo_runStutus; 
    }
    else
    {
        while(1);
    }
}

sdt_bool mde_msRtu_master_slave_phy_idle_status(sdt_int8u in_solidNum)
{
    if(in_solidNum < max_solid)
    {
         if( modbus_master_slave_solid[in_solidNum].bsp_ms_pull_phyBus_status())
         {
              return sdt_false;
         }
         return sdt_true;
    }
    else
    {
        while(1);
    }
}

void mde_master_slave_push_link_onemessage(sdt_int8u in_solidNum,ms_link_comm_data_def* in_pTransmitData)
{
    sdt_int8u i;
    if(in_solidNum < max_solid)
    {
        for(i = 0; i < (in_pTransmitData->PayloadLength);i++)
        {
            modbus_master_slave_solid[in_solidNum].transmit_buff[i+18] = in_pTransmitData->Payload[i];           
        }
        modbus_master_slave_solid[in_solidNum].transmit_length = (18+in_pTransmitData->PayloadLength);
        modbus_master_slave_solid[in_solidNum].transmit_buff[2] = (sdt_int8u)(modbus_master_slave_solid[in_solidNum].transmit_length>>8);
        modbus_master_slave_solid[in_solidNum].transmit_buff[3] =  (sdt_int8u)modbus_master_slave_solid[in_solidNum].transmit_length;
        append_crc_to_message(&modbus_master_slave_solid[in_solidNum]);
        modbus_master_slave_solid[in_solidNum].weekPullUpFlag = sdt_false;
        modbus_master_slave_solid[in_solidNum].mmRtu_status = mRtuS_master_slave_none;
        if(in_pTransmitData->responseFlag)
        {
            modbus_master_slave_solid[in_solidNum].mmoo_runStutus = msRunS_transmit_respose_str;
        }
        else
        {
            modbus_master_slave_solid[in_solidNum].mmoo_runStutus = msRunS_transmit_str;
        }
    }
    else
    {
        while(1);
    }
}

ms_link_comm_data_def mde_pull_msRtu_onemessage(sdt_int8u in_solidNum,sdt_bool* out_HaveMessage)
{
    if(in_solidNum < max_solid)
    {
        if(modbus_master_slave_solid[in_solidNum].msLinkBits & MSLinkBits_NewMessage)
        {
            modbus_master_slave_solid[in_solidNum].msLinkBits &= (~MSLinkBits_NewMessage);
            *out_HaveMessage =  sdt_true;
            modbus_master_slave_solid[in_solidNum].mmoo_runStutus = msRunS_idle;  
        } 
        else if(modbus_master_slave_solid[in_solidNum].msLinkBits & MSLinkBits_CheckMessage)
        {
            modbus_master_slave_solid[in_solidNum].msLinkBits &= (~MSLinkBits_CheckMessage);
            *out_HaveMessage =  sdt_false;
            //modbus_master_slave_solid[in_solidNum].mmoo_runStutus = msRunS_idle;  
        } 
        else
        {
            *out_HaveMessage =  sdt_false;
        }       
    }
    else
    {
        while(1);
    }
    return (modbus_master_slave_solid[in_solidNum].msLinkRevData); 
}

//void mde_msRtu_master_slave_push_idle_status(sdt_int8u in_solidNum)
//{
//    modbus_master_slave_solid[in_solidNum].mmoo_runStutus = msRunS_idle;  
//}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//name:设置本地ID
//-----------------------------------------------------------------------------
void mde_msRtu_master_slave_push_mRtu_local_id(sdt_int8u in_solidNum,sdt_int8u* in_id)
{
    if(in_solidNum < max_solid)
    {
        modbus_master_slave_solid[in_solidNum].localID[0] = in_id[0];
        modbus_master_slave_solid[in_solidNum].localID[1] = in_id[1];
        modbus_master_slave_solid[in_solidNum].localID[2] = in_id[2];
        modbus_master_slave_solid[in_solidNum].localID[3] = in_id[3];
        modbus_master_slave_solid[in_solidNum].localID[4] = in_id[4];
        modbus_master_slave_solid[in_solidNum].localID[5] = in_id[5];
    }
    else
    {
        while(1);
    }
}
//sdt_bool mde_push_msRtu_master_slave_sendReg(sdt_int8u in_solidNum,sdt_int16u in_reg_addr,sdt_int16u in_RegDetails)
//{
//    sdt_bool complete = sdt_false;
//
//    if(in_solidNum < max_solid)
//    {
//        if(make_sendReg_buff(&modbus_master_slave_solid[in_solidNum],in_reg_addr,in_RegDetails))
//        {
//            complete = sdt_true;
//        }
//        else
//        {
//            while(1);
//        } 
//    }
//    else
//    {
//        while(1);
//    }
//
//    return(complete);
//}

//sdt_bool mde_push_msRtu_master_sendReg(sdt_int8u in_solidNum,sdt_int16u in_reg_addr,sdt_int16u in_RegDetails)
//{
//    sdt_bool complete = sdt_false;
//
//    if(in_solidNum < max_solid)
//    {
//        if(make_master_sendReg_buff(&modbus_master_slave_solid[in_solidNum],in_reg_addr,in_RegDetails))
//        {
//            complete = sdt_true;
//        }
//        else
//        {
//            while(1);
//        } 
//    }
//    else
//    {
//        while(1);
//    }
//
//    return(complete);
//}

//sdt_bool mde_pull_msRtu_receiveReg(sdt_int8u in_solidNum,sdt_int16u in_reg_addr,sdt_int16u* out_pRregDetails)
//{
//    sdt_bool complete = sdt_false;
//
//    if(in_solidNum < max_solid)
//    {
//        if(get_receiveReg_buff(&modbus_master_slave_solid[in_solidNum],in_reg_addr,out_pRregDetails))
//        {
//            complete = sdt_true;
//        }
//    }
//    else
//    {
//        while(1);
//    }
//
//    return(complete);
//}

//mRtu_cmd_status_def mde_pull_msRtu_register_add_len(sdt_int8u in_solidNum,sdt_int16u* out_reg_addr,sdt_int16u* out_length)
//{
//    mRtu_cmd_status_def cmdStatus;
//    if(in_solidNum < max_solid)
//    {
//        cmdStatus = modbus_master_slave_solid[in_solidNum].cmd_status;
//        if((modbus_master_slave_solid[in_solidNum].cmd_status == msRtu_cmd_03) || 
//        (modbus_master_slave_solid[in_solidNum].cmd_status == msRtu_cmd_17))
//        {
//            *out_reg_addr = modbus_master_slave_solid[in_solidNum].readReg_addr;
//            *out_length = modbus_master_slave_solid[in_solidNum].readReg_length;
//        }
//        else if((modbus_master_slave_solid[in_solidNum].cmd_status == msRtu_cmd_06) || 
//        (modbus_master_slave_solid[in_solidNum].cmd_status == msRtu_cmd_10))
//        {
//            *out_reg_addr = modbus_master_slave_solid[in_solidNum].writeReg_addr;
//            *out_length = modbus_master_slave_solid[in_solidNum].writeReg_length;
//        }
//        return cmdStatus;
//    }
//    else
//    {
//        while(1);
//    }
//}
//void mde_pull_msRtu_cmd17_write_add_len(sdt_int8u in_solidNum,sdt_int16u* out_reg_addr,sdt_int16u* out_length)
//{
//    if(in_solidNum < max_solid)
//    {
//        *out_reg_addr = modbus_master_slave_solid[in_solidNum].writeReg_addr;
//        *out_length = modbus_master_slave_solid[in_solidNum].writeReg_length;
//    }
//    else
//    {
//        while(1);
//    }
//}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//void mde_mRtu_master_slave_cmd0x03_transmit(sdt_int8u in_solidNum,sdt_bool resposeFlag,\
//                                      sdt_int16u in_reg_addr,sdt_int16u in_reg_length)
//{
//    modbus_master_slave_solid[in_solidNum].transmit_buff[18] = 0x01;   
//    modbus_master_slave_solid[in_solidNum].transmit_buff[20] = 0xff;
//    modbus_master_slave_solid[in_solidNum].transmit_buff[21] = 0x03;
//    sdt_int16u length = 0;
//    if(resposeFlag)
//    {
//        modbus_master_slave_solid[in_solidNum].transmit_buff[19] = 0x81; 
//        modbus_master_slave_solid[in_solidNum].transmit_buff[22] = in_reg_addr;
//        modbus_master_slave_solid[in_solidNum].transmit_buff[23] = in_reg_length<<1;
//        modbus_master_slave_solid[in_solidNum].transmit_length += 24;
//        length = modbus_master_slave_solid[in_solidNum].transmit_length;
//        modbus_master_slave_solid[in_solidNum].transmit_buff[2] = (sdt_int8u)(length>>8);   
//        modbus_master_slave_solid[in_solidNum].transmit_buff[3] = (sdt_int8u)length;
//    }
//    else
//    {
//        modbus_master_slave_solid[in_solidNum].readReg_addr = in_reg_addr;
//        modbus_master_slave_solid[in_solidNum].readReg_length = in_reg_length;
//        modbus_master_slave_solid[in_solidNum].transmit_buff[19] = 0x01;
//        modbus_master_slave_solid[in_solidNum].transmit_buff[22] = in_reg_addr;
//        modbus_master_slave_solid[in_solidNum].transmit_buff[23] = in_reg_length;
//        modbus_master_slave_solid[in_solidNum].transmit_length = 24;
//        length = 24;
//        modbus_master_slave_solid[in_solidNum].transmit_buff[2] = (sdt_int8u)(length>>8);   
//        modbus_master_slave_solid[in_solidNum].transmit_buff[3] = (sdt_int8u)length;
//    }  
//    append_crc_to_message(&modbus_master_slave_solid[in_solidNum]);
//    modbus_master_slave_solid[in_solidNum].mmoo_runStutus = msRunS_transmit_str;
//}
//
//void mde_mRtu_master_slave_cmd0x06_transmit(sdt_int8u in_solidNum,sdt_bool resposeFlag,\
//                                      sdt_int16u in_reg_addr,sdt_int16u in_reg_data)
//{
//    modbus_master_slave_solid[in_solidNum].transmit_buff[18] = 0x01;   
//    modbus_master_slave_solid[in_solidNum].transmit_buff[20] = 0xff;
//    modbus_master_slave_solid[in_solidNum].transmit_buff[21] = 0x06;
//    sdt_int16u length = 0;
//    if(resposeFlag)
//    {
//        modbus_master_slave_solid[in_solidNum].transmit_buff[19] = 0x81; 
//        modbus_master_slave_solid[in_solidNum].transmit_buff[22] = (sdt_int8u)(in_reg_addr);
//        modbus_master_slave_solid[in_solidNum].transmit_buff[23] = (sdt_int8u)(in_reg_data>>8);
//        modbus_master_slave_solid[in_solidNum].transmit_buff[24] = (sdt_int8u)(in_reg_data);
//        modbus_master_slave_solid[in_solidNum].transmit_length = 25;
//        length = 25;
//        modbus_master_slave_solid[in_solidNum].transmit_buff[2] = (sdt_int8u)(length>>8);   
//        modbus_master_slave_solid[in_solidNum].transmit_buff[3] = (sdt_int8u)length;
//    }
//    else
//    {
//        modbus_master_slave_solid[in_solidNum].transmit_buff[19] = 0x01;      
//        modbus_master_slave_solid[in_solidNum].transmit_buff[22] = (sdt_int8u)(in_reg_addr);
//        modbus_master_slave_solid[in_solidNum].transmit_buff[23] = (sdt_int8u)(in_reg_data>>8);
//        modbus_master_slave_solid[in_solidNum].transmit_buff[24] = (sdt_int8u)(in_reg_data);
//        modbus_master_slave_solid[in_solidNum].transmit_length = 25;
//        length = 25;
//        modbus_master_slave_solid[in_solidNum].transmit_buff[2] = (sdt_int8u)(length>>8);   
//        modbus_master_slave_solid[in_solidNum].transmit_buff[3] = (sdt_int8u)length;
//    }  
//    append_crc_to_message(&modbus_master_slave_solid[in_solidNum]);
//    modbus_master_slave_solid[in_solidNum].mmoo_runStutus = msRunS_transmit_str;
//}                                      
//
//void mde_mRtu_master_slave_cmd0x10_transmit(sdt_int8u in_solidNum,sdt_bool resposeFlag,\
//                                      sdt_int16u in_reg_addr,sdt_int16u in_reg_length)
//{   
//    modbus_master_slave_solid[in_solidNum].transmit_buff[18] = 0x01;   
//    modbus_master_slave_solid[in_solidNum].transmit_buff[20] = 0xff;
//    modbus_master_slave_solid[in_solidNum].transmit_buff[21] = 0x10;
//    sdt_int16u length = 0;
//    if(resposeFlag)
//    {
//        modbus_master_slave_solid[in_solidNum].transmit_buff[19] = 0x81; 
//        modbus_master_slave_solid[in_solidNum].transmit_buff[22] = (sdt_int8u)(in_reg_addr);
//        modbus_master_slave_solid[in_solidNum].transmit_buff[23] = (sdt_int8u)(in_reg_length);
//        modbus_master_slave_solid[in_solidNum].transmit_length = 24;
//        length = 24;
//        modbus_master_slave_solid[in_solidNum].transmit_buff[2] = (sdt_int8u)(length>>8);   
//        modbus_master_slave_solid[in_solidNum].transmit_buff[3] = (sdt_int8u)length;
//        append_crc_to_message(&modbus_master_slave_solid[in_solidNum]);
//        modbus_master_slave_solid[in_solidNum].mmoo_runStutus = msRunS_transmit_str;
//    }
//    else
//    {
//        modbus_master_slave_solid[in_solidNum].cmd_status = msRtu_cmd_10;
//        modbus_master_slave_solid[in_solidNum].writeReg_addr = in_reg_addr;
//        modbus_master_slave_solid[in_solidNum].writeReg_length = in_reg_length;
//        modbus_master_slave_solid[in_solidNum].transmit_buff[19] = 0x01;
//        modbus_master_slave_solid[in_solidNum].transmit_buff[22] = (sdt_int8u)(in_reg_addr);
//        modbus_master_slave_solid[in_solidNum].transmit_buff[23] = (sdt_int8u)(in_reg_length);
//        modbus_master_slave_solid[in_solidNum].transmit_buff[24] = ((sdt_int8u)(in_reg_length) << 1);
//        modbus_master_slave_solid[in_solidNum].transmit_length = 25;
//        length = (modbus_master_slave_solid[in_solidNum].transmit_buff[24] + modbus_master_slave_solid[in_solidNum].transmit_length);
//        modbus_master_slave_solid[in_solidNum].transmit_buff[2] = (sdt_int8u)(length>>8);   
//        modbus_master_slave_solid[in_solidNum].transmit_buff[3] = (sdt_int8u)length;
//    }  
//   
//}
//
//void mde_mRtu_master_slave_cmd0x17_transmit(sdt_int8u in_solidNum,sdt_bool resposeFlag,\
//                                      sdt_int16u in_reg_read_addr,sdt_int16u in_reg_read_length,\
//                                      sdt_int16u in_reg_write_addr,sdt_int16u in_reg_write_length)
//{
//    modbus_master_slave_solid[in_solidNum].transmit_buff[18] = 0x01;   
//    modbus_master_slave_solid[in_solidNum].transmit_buff[20] = 0xff;
//    modbus_master_slave_solid[in_solidNum].transmit_buff[21] = 0x17;
//    sdt_int16u length = 0;
//    if(resposeFlag)
//    {
//        modbus_master_slave_solid[in_solidNum].transmit_buff[19] = 0x81; 
//        modbus_master_slave_solid[in_solidNum].transmit_buff[22] = (sdt_int8u)(in_reg_read_addr);
//        modbus_master_slave_solid[in_solidNum].transmit_buff[23] = ((sdt_int8u)(in_reg_read_length) << 1);
//        modbus_master_slave_solid[in_solidNum].transmit_length += 24;
//        length = modbus_master_slave_solid[in_solidNum].transmit_length;
//        modbus_master_slave_solid[in_solidNum].transmit_buff[2] = (sdt_int8u)(length>>8);   
//        modbus_master_slave_solid[in_solidNum].transmit_buff[3] = (sdt_int8u)length;
//        append_crc_to_message(&modbus_master_slave_solid[in_solidNum]);
//        modbus_master_slave_solid[in_solidNum].mmoo_runStutus = msRunS_transmit_str;
//    }
//    else
//    {
//        modbus_master_slave_solid[in_solidNum].cmd_status = msRtu_cmd_17;
//        modbus_master_slave_solid[in_solidNum].writeReg_addr = in_reg_write_addr;
//        modbus_master_slave_solid[in_solidNum].writeReg_length = in_reg_write_length;
//        modbus_master_slave_solid[in_solidNum].transmit_buff[19] = 0x01;
//        modbus_master_slave_solid[in_solidNum].transmit_buff[22] = (sdt_int8u)(in_reg_read_addr);
//        modbus_master_slave_solid[in_solidNum].transmit_buff[23] = (sdt_int8u)(in_reg_read_length);
//        modbus_master_slave_solid[in_solidNum].transmit_buff[24] = (sdt_int8u)(in_reg_write_addr);
//        modbus_master_slave_solid[in_solidNum].transmit_buff[25] = (sdt_int8u)(in_reg_write_length);
//        modbus_master_slave_solid[in_solidNum].transmit_buff[26] = (in_reg_write_length<<1);
//        modbus_master_slave_solid[in_solidNum].transmit_length = 27;
//        length = (modbus_master_slave_solid[in_solidNum].transmit_length + modbus_master_slave_solid[in_solidNum].transmit_buff[26]);
//        modbus_master_slave_solid[in_solidNum].transmit_buff[2] = (sdt_int8u)(length>>8);   
//        modbus_master_slave_solid[in_solidNum].transmit_buff[3] = (sdt_int8u)length;
//    }  
//   
//}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//name:写入一个寄存器的数据到缓存
//-----------------------------------------------------------------------------
//static sdt_bool make_sendReg_buff(modbus_master_slave_oper_def* mix_oper,sdt_int16u in_reg_addr,sdt_int16u in_regDetails)
//{
//    if(in_reg_addr < (mix_oper->readReg_addr))
//    {
//        return(sdt_false);
//    }
//    else
//    {
//        sdt_int8u buff_index;
//        if(0x03 == mix_oper->receive_buff[19])
//        {
//            buff_index = 24;
//        }
//        else if (0x17 == mix_oper->receive_buff[19])
//        {
//            buff_index = 24;
//        }
//        else
//        {
//            while(1);
//        }
//        
//        sdt_int16u dis_r = in_reg_addr - (mix_oper->readReg_addr);
//        while(dis_r)
//        {
//            buff_index = buff_index + 2;
//            dis_r--;
//        }
//        mix_oper->transmit_length += 2;
//        mix_oper->transmit_buff[buff_index] = in_regDetails >> 8;
//        mix_oper->transmit_buff[buff_index+1] = in_regDetails;
//        return(sdt_true);
//    }
//}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//name:写入一个寄存器的数据到缓存
//-----------------------------------------------------------------------------
//static sdt_bool make_master_sendReg_buff(modbus_master_slave_oper_def* mix_oper,sdt_int16u in_reg_addr,sdt_int16u in_regDetails)
//{
//    if(in_reg_addr < (mix_oper->writeReg_addr))
//    {
//        return(sdt_false);
//    }
//    else
//    {
//        sdt_int8u buff_index;
//        if(msRtu_cmd_10 == mix_oper->cmd_status)
//        {
//            buff_index = 25;
//        }
//        else if (msRtu_cmd_17 == mix_oper->cmd_status)
//        {
//            buff_index = 27;
//        }
//        else
//        {
//            while(1);
//        }
//        
//        sdt_int16u dis_r = in_reg_addr - (mix_oper->writeReg_addr);
//        while(dis_r)
//        {
//            buff_index = buff_index + 2;
//            dis_r--;
//        }
//        mix_oper->transmit_length += 2;
//        mix_oper->transmit_buff[buff_index] = in_regDetails >> 8;
//        mix_oper->transmit_buff[buff_index+1] = in_regDetails;
//        if((in_reg_addr - (mix_oper->writeReg_addr)) >= (mix_oper->writeReg_length-1))
//        {
//            append_crc_to_message(mix_oper);
//            mix_oper->mmoo_runStutus = msRunS_transmit_str;
//        }
//        return(sdt_true);
//    }
//}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//name:获取写寄存的的内容
//-----------------------------------------------------------------------------
//static sdt_bool get_receiveReg_buff(modbus_master_slave_oper_def* mix_oper,sdt_int16u in_reg_addr,sdt_int16u* out_regDetails)
//{
//    if(in_reg_addr < (mix_oper->writeReg_addr))
//    {
//        return(sdt_false);
//    }
//    else
//    {
//        sdt_int8u buff_index;
//        if(0x06 == mix_oper->receive_buff[19])
//        {
//            buff_index = 21;
//        }
//        else if(0x10 == mix_oper->receive_buff[19])
//        {
//            buff_index = 23;
//        }
//        else if(0x17 == mix_oper->receive_buff[19])
//        {
//            buff_index = 25;
//        }
//        else
//        {
//            return(sdt_false);
//        }
//
//        sdt_int16u dis_r = in_reg_addr - (mix_oper->writeReg_addr);
//        while(dis_r)
//        {
//            buff_index = buff_index + 2;
//            dis_r--;
//        }
//        *out_regDetails = pbc_arrayToInt16u_bigEndian(&mix_oper->receive_buff[buff_index]);
//        return(sdt_true);
//    }
//
//}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//接收协议处理
//--------------------------------------------------------------------------------------------------------------------------
//sdt_bool modbus_master_slave_receive_protocol(modbus_master_slave_oper_def* mix_mm_oper)
//{
//    sdt_int8u rd_length;
//
//    rd_length = mix_mm_oper->rev_index;
//    if(rd_length < 6)
//    {
//        return(sdt_false);
//    }
//    else
//    { 
//        sdt_int8u crc_value[2];
//        Crc16CalculateOfByte(&mix_mm_oper->receive_buff[0],(rd_length-2),&crc_value[0]);
//        if((crc_value[1] == mix_mm_oper->receive_buff[rd_length-2]) && (crc_value[0] == mix_mm_oper->receive_buff[rd_length-1]))//crc is ok
//        {        
//            if(0x03 == mix_mm_oper->receive_buff[19])
//            {
//                mix_mm_oper->readReg_addr = mix_mm_oper->receive_buff[20];
//                mix_mm_oper->readReg_length = mix_mm_oper->receive_buff[21];
//                mix_mm_oper->cmd_status = msRtu_cmd_03;
//            }
//            else if(0x06 == mix_mm_oper->receive_buff[19])
//            {
//                mix_mm_oper->writeReg_addr = mix_mm_oper->receive_buff[20];
//                mix_mm_oper->writeReg_length = 1;
//                mix_mm_oper->cmd_status = msRtu_cmd_06;
//            }
//            else if(0x10 == mix_mm_oper->receive_buff[19])
//            {
//                mix_mm_oper->writeReg_addr = mix_mm_oper->receive_buff[20];
//                mix_mm_oper->writeReg_length = mix_mm_oper->receive_buff[21];
//                mix_mm_oper->cmd_status = msRtu_cmd_10;
//            }
//            else if(0x17 == mix_mm_oper->receive_buff[19])
//            {
//                mix_mm_oper->readReg_addr = mix_mm_oper->receive_buff[20];
//                mix_mm_oper->readReg_length = mix_mm_oper->receive_buff[21];
//                mix_mm_oper->writeReg_addr = mix_mm_oper->receive_buff[22];
//                mix_mm_oper->writeReg_length = mix_mm_oper->receive_buff[23];
//                mix_mm_oper->cmd_status = msRtu_cmd_17;
//            }
//            else
//            {
//                
//            }
//            return(sdt_true);
//        }
//    }
//    return(sdt_false);
//}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include ".\app_cfg.h"
#include "stdlib.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef enum
{
    STATUS_IDLE                = 0x00,
    STATUS_SEND_RTC,  
    STATUS_RECEIVE_RTC, 
	STATUS_TIMINGSEND,  
    STATUS_TIMINGRECEIVE,      
    STATUS_CHECK_CONFLICT, 
    STATUS_CONFLICT_RESULT, 
	STATUS_OCCPYSEND,           
	STATUS_OCCPYRECEIVE,         
	STATUS_RESEND,             
	STATUS_RERECEIVE ,  
    STATUS_SEND_UPDATA_LIST, 
    STATUS_RECEIVE_UPDATA_LIST, 
    STATUS_DISPOSE_DATA,
}synStatus_def;

#define  SEND_RTC_BIT          0x01
#define  SEND_MESSAGE_BIT      0x02
#define  MIN_RTC_ADDR          0X2000
#define  MAX_RTC_ADDR          0X200F
#define  MIN_NEED_ADDR         0X2C40
#define  MAX_NEDD_ADDR         0X2C7F
#define  MIN_OWN_ADDR          0X2C00
#define  MAX_OWN_ADDR          0X2C3F

uint32_t        rtcStamp;           //時鐘時間戳
uint32_t        ownStamp;           //自己的时间戳
uint32_t        needStamp;           //需要的时间戳
typedef struct
{
    synStatus_def   msSynStatsus;       //同步状态
    uint16_t        sendWord;           //发送字
    uint16_t        occpyWord;          //抢占字
    uint16_t        updataWord;         //更新字
    timerClock_def  send_wait_delay;    //主动发送等待
    timerClock_def  timing_send_dalay;  //定时发送数据
    timerClock_def  timing_rtc_dalay;   //定时更新时间
}msComm_t;


msComm_t appModbusSyn[MAX_MODBUS_NUM];


void app_master_slave_push_occpy(uint8_t in_solidNum,uint16_t in_occpyBit)
{
    appModbusSyn[in_solidNum].occpyWord |= in_occpyBit;
    appModbusSyn[in_solidNum].msSynStatsus = STATUS_CHECK_CONFLICT;
}

void app_push_own_stamp(uint32_t  in_stamp)
{
    ownStamp = in_stamp;
}

void app_push_need_stamp(uint32_t  in_stamp)
{
    needStamp = in_stamp;
}
void app_push_rtc_stamp(uint32_t  in_stamp)
{
    rtcStamp = in_stamp;
}
void app_pull_receive_reg_485(uint16_t in_regAddr,uint8_t in_regLength,uint8_t *in_buff)
{
    uint16_t regDetails;
    uint8_t i = 0;
    while(in_regLength)
    {
        regDetails = (((uint16_t)in_buff[i] << 8) |  in_buff[i+1]);
        app_modebus_write_reg_data(in_regAddr,regDetails);
        i+=2;
        in_regAddr++;
        in_regLength--;
    }
}

static void app_push_send_reg_485(uint16_t in_regAddr,uint8_t in_regLength,uint8_t *out_buff)
{
    uint16_t regDetails;
    uint8_t i = 0;
    while(in_regLength)
    {
        regDetails = app_modbus_read_reg_data(in_regAddr);
        out_buff[i] = (uint8_t)(regDetails>>8);
        out_buff[i+1] = (uint8_t)(regDetails);
        i+=2;
        in_regAddr++;
        in_regLength--;
    }
}


void app_master_slave_send_one_block(uint8_t in_solidNum,uint8_t *in_dest_addr,uint16_t in_regAddr,uint8_t in_regLength,bool responseFlag)
{//主动发起
    ms_link_comm_data_def tempLinkData;
    uint32_t stamp;
    mde_master_slave_link_message_transmit(in_solidNum,in_dest_addr,PROTOL_SYN,responseFlag); 
    tempLinkData.Payload[0] = PROTOL_VERSION;
    tempLinkData.Payload[1] = 0x00;
    if(responseFlag)
    {
        tempLinkData.Payload[1] = 0x01; 
    }
    tempLinkData.Payload[2] = 0x00;//预留
    tempLinkData.Payload[3] = 0x01;//块数量
    if((in_regAddr>=MIN_RTC_ADDR) && (in_regAddr <= MAX_RTC_ADDR))
    {
        stamp = rtcStamp;
    }
    else if((in_regAddr>=MIN_NEED_ADDR) && (in_regAddr <= MAX_NEDD_ADDR))
    {
        stamp = needStamp;
    }
    else if((in_regAddr>=MIN_OWN_ADDR) && (in_regAddr <= MAX_OWN_ADDR))
    {
        stamp = ownStamp;
    }
    tempLinkData.Payload[4] = (uint8_t)(stamp>>24);//时间戳
    tempLinkData.Payload[5] =(uint8_t)(stamp>>16);
    tempLinkData.Payload[6] =(uint8_t)(stamp>>8);
    tempLinkData.Payload[7] =(uint8_t)(stamp);
    tempLinkData.Payload[8] = ((in_regLength<<1) + 3);
    tempLinkData.Payload[9] = (uint8_t)(in_regAddr>>8);
    tempLinkData.Payload[10] = (uint8_t)(in_regAddr);
    app_push_send_reg_485(in_regAddr,in_regLength,&tempLinkData.Payload[11]);
    tempLinkData.PayloadLength = (11+(in_regLength<<1));
    tempLinkData.responseFlag = false;
    mde_master_slave_push_link_onemessage(in_solidNum,&tempLinkData);
    ownStamp = pbc_timeStamp_get_stamp(); //最新时间戳
    rtcStamp = pbc_timeStamp_get_stamp(); //最新时间戳
}

void app_master_slave_response_one_block(uint8_t in_solidNum,uint8_t *in_dest_addr,uint16_t in_regAddr,uint8_t in_regLength,bool in_noDataFlag)
{//响应数据
    ms_link_comm_data_def tempLinkData;
    uint32_t stamp;
    mde_master_slave_link_message_transmit(in_solidNum,in_dest_addr,PROTOL_SYN,false); 
    tempLinkData.Payload[0] = PROTOL_VERSION;  
    tempLinkData.Payload[2] = 0x00;//预留
    tempLinkData.Payload[3] = 0x01;//块数量
    if((in_regAddr>=MIN_RTC_ADDR) && (in_regAddr <= MAX_RTC_ADDR))
    {
        stamp = rtcStamp;
    }
    else if((in_regAddr>=MIN_NEED_ADDR) && (in_regAddr <= MAX_NEDD_ADDR))
    {
        stamp = needStamp;
    }
    else if((in_regAddr>=MIN_OWN_ADDR) && (in_regAddr <= MAX_OWN_ADDR))
    {
        stamp = ownStamp;
    }
    tempLinkData.Payload[4] = (uint8_t)(stamp>>24);//时间戳
    tempLinkData.Payload[5] = (uint8_t)(stamp>>16);
    tempLinkData.Payload[6] = (uint8_t)(stamp>>8);
    tempLinkData.Payload[7] = (uint8_t)(stamp);
    if(in_noDataFlag)
    {//不带数据位
        tempLinkData.Payload[1] = 0x41;//应答控制不带数据应答
        tempLinkData.Payload[8] = 3;//数据长度
        tempLinkData.Payload[9] = (uint8_t)(in_regAddr>>8);
        tempLinkData.Payload[10] = (uint8_t)(in_regAddr);   
        tempLinkData.PayloadLength = 11;//总长度
    }
    else
    {
        tempLinkData.Payload[1] = 0x81;//应答控制带数据应答
        tempLinkData.Payload[8] = (3+(in_regLength<<1));//数据长度
        tempLinkData.Payload[9] = (uint8_t)(in_regAddr>>8);
        tempLinkData.Payload[10] = (uint8_t)(in_regAddr);   
        app_push_send_reg_485(in_regAddr,in_regLength,&tempLinkData.Payload[11]);
        tempLinkData.PayloadLength = (11+(in_regLength<<1));//总长度
    }   
    tempLinkData.responseFlag = true;    
    mde_master_slave_push_link_onemessage(in_solidNum,&tempLinkData);
}


void app_master_slave_dispose_syn_data(uint8_t in_solidNum,ms_link_comm_data_def *in_rev_data)
{//同步数据
    uint16_t regAdd = 0;
    uint8_t regLength = 0;
    uint8_t lengthIndex = 0;
    uint8_t blockLen = 0;
    uint8_t i = 0;
    uint8_t blockNum = 0;
    uint32_t sendStamp = 0;
    appModbusSyn[in_solidNum].msSynStatsus = STATUS_DISPOSE_DATA;
    if(pull_local_id(&in_rev_data->LinkDstAddr[0])) 
    {
        sendStamp = pbc_arrayToInt32u_bigEndian(&in_rev_data->Payload[4]);//发起方时间戳
        regAdd =  (((uint16_t)in_rev_data->Payload[lengthIndex+1]<<8) | in_rev_data->Payload[lengthIndex+2]);
        if((regAdd>=MIN_RTC_ADDR) && (regAdd <= MAX_RTC_ADDR))
        {//RTC区
            if(sendStamp >= rtcStamp)
            {//发起方的数据为最新数据则更新数据，需要应答时不带数据应答
            /*更新数据*/
                rtcStamp = sendStamp;
                blockNum = in_rev_data->Payload[3];
                for(i = 0;i < blockNum;i++)
                {
                    lengthIndex = (4 + (i+1)*4);
                    if(i != 0)
                    {
                        lengthIndex += blockLen;
                    }
                    
                    regLength = ((in_rev_data->Payload[lengthIndex]-3)>>1);            
                    app_pull_receive_reg_485(regAdd,regLength,&in_rev_data->Payload[lengthIndex+3]);            
                    blockLen += in_rev_data->Payload[lengthIndex];            
                }
                if(app_general_pull_system_parameter_change())
                {//数据有变化更新发送列表
                    uint8_t i = 0;
                    for(i = 0; i < MAX_MODBUS_NUM;i++)
                    {
                        app_master_slave_push_occpy(i,SET_POWER);
                    }  
                }
                /*判定需不需要应答*/
                if(in_rev_data->Payload[1] == 0x01)
                {//不带数据
                    app_master_slave_response_one_block(in_solidNum,&in_rev_data->LinkSrcAddr[0],regAdd,regLength,true);
                }
            }
            else
            {//自己的数据为最新数据，需要应答时带数据位应答
                /*判定需不需要应答*/
                if(in_rev_data->Payload[1] == 0x01)
                {//带数据
                    app_master_slave_response_one_block(in_solidNum,&in_rev_data->LinkSrcAddr[0],regAdd,regLength,false);
                }
            }         
        }
        else if((regAdd>=MIN_NEED_ADDR) && (regAdd <= MAX_NEDD_ADDR))
        {//需要的数据区
            if(sendStamp >= needStamp)
            {//发起方的数据为最新数据则更新数据，需要应答时不带数据应答
            /*更新数据*/
                needStamp = sendStamp;
                blockNum = in_rev_data->Payload[3];
                for(i = 0;i < blockNum;i++)
                {
                    lengthIndex = (4 + (i+1)*4);
                    if(i != 0)
                    {
                        lengthIndex += blockLen;
                    }
                    
                    regLength = ((in_rev_data->Payload[lengthIndex]-3)>>1);            
                    app_pull_receive_reg_485(regAdd,regLength,&in_rev_data->Payload[lengthIndex+3]);            
                    blockLen += in_rev_data->Payload[lengthIndex];            
                }
                if(app_general_pull_system_parameter_change())
                {//数据有变化更新发送列表
                    uint8_t i = 0;
                    for(i = 0; i < MAX_MODBUS_NUM;i++)
                    {
                        app_master_slave_push_occpy(i,SET_POWER);
                    }  
                }
                /*判定需不需要应答*/
                if(in_rev_data->Payload[1] == 0x01)
                {//不带数据
                    app_master_slave_response_one_block(in_solidNum,&in_rev_data->LinkSrcAddr[0],regAdd,regLength,true);
                }
            }
            else
            {//自己的数据为最新数据，需要应答时带数据位应答
                /*判定需不需要应答*/
                if(in_rev_data->Payload[1] == 0x01)
                {//带数据
                    app_master_slave_response_one_block(in_solidNum,&in_rev_data->LinkSrcAddr[0],regAdd,regLength,false);
                }
            }         
        }
        else if((regAdd>=MIN_OWN_ADDR) && (regAdd <= MAX_OWN_ADDR))
        {//自己的数据
            if(sendStamp >= ownStamp)
            {//发起方的数据为最新数据则更新数据，需要应答时不带数据应答
            /*更新数据*/
                ownStamp = sendStamp;
                blockNum = in_rev_data->Payload[3];
                for(i = 0;i < blockNum;i++)
                {
                    lengthIndex = (4 + (i+1)*4);
                    if(i != 0)
                    {
                        lengthIndex += blockLen;
                    }
                    
                    regLength = ((in_rev_data->Payload[lengthIndex]-3)>>1);            
                    app_pull_receive_reg_485(regAdd,regLength,&in_rev_data->Payload[lengthIndex+3]);            
                    blockLen += in_rev_data->Payload[lengthIndex];            
                }
                if(app_general_pull_system_parameter_change())
                {//数据有变化更新发送列表
                    uint8_t i = 0;
                    for(i = 0; i < MAX_MODBUS_NUM;i++)
                    {
                        app_master_slave_push_occpy(i,SET_POWER);
                    }  
                }
                /*判定需不需要应答*/
                if(in_rev_data->Payload[1] == 0x01)
                {//不带数据
                    app_master_slave_response_one_block(in_solidNum,&in_rev_data->LinkSrcAddr[0],regAdd,regLength,true);
                }
            }
            else
            {//自己的数据为最新数据，需要应答时带数据位应答
                /*判定需不需要应答*/
                if(in_rev_data->Payload[1] == 0x01)
                {//带数据
                    app_master_slave_response_one_block(in_solidNum,&in_rev_data->LinkSrcAddr[0],regAdd,regLength,false);
                }
            }         
        }
    }   
}

void app_master_slave_syn_task(uint8_t in_solidNum)
{
    ms_link_comm_data_def  pReceiveData;
    uint8_t regeditNum = 0;
    uint8_t i; 
    bool CCOnlineFlag = false; 
    sdt_bool ReceiveOneMessageFlag;
    pbc_timerMillRun_task(&appModbusSyn[in_solidNum].send_wait_delay);//发送等待
    pbc_timerClockRun_task(&appModbusSyn[in_solidNum].send_wait_delay);//定时发送等待
    pbc_timerClockRun_task(&appModbusSyn[in_solidNum].timing_rtc_dalay);//定时校时等待
    switch (appModbusSyn[in_solidNum].msSynStatsus)
    {
        case STATUS_IDLE:
        {       
            pReceiveData = mde_pull_msRtu_onemessage(in_solidNum,&ReceiveOneMessageFlag);
            if(ReceiveOneMessageFlag)
            {//收到一帧数据
                if(pReceiveData.ProcotolType == PROTOL_SYN) 
                {//同步帧
                    app_master_slave_dispose_syn_data(in_solidNum,&pReceiveData);
                }   
                else if(pReceiveData.ProcotolType == PROTOL_REGEDIT) 
                {//注册帧
                    app_master_slave_dispose_regedit_data(in_solidNum,&pReceiveData);
                }                             
            } 
            else if(pbc_pull_timerIsCompleted(&appModbusSyn[in_solidNum].timing_rtc_dalay))
            {//定时更新时间
                pbc_reload_timerClock(&appModbusSyn[in_solidNum].timing_rtc_dalay,1800);//半小时
                appModbusSyn[in_solidNum].sendWord |= SEND_RTC_BIT;
                appModbusSyn[in_solidNum].msSynStatsus = STATUS_CHECK_CONFLICT;
            }
            else if(pbc_pull_timerIsCompleted(&appModbusSyn[in_solidNum].timing_send_dalay))
            {//定时更新数据
                pbc_reload_timerClock(&appModbusSyn[in_solidNum].timing_send_dalay,600);//10分钟
             //   appModbusSyn[in_solidNum].sendWord |= SEND_MESSAGE_BIT;
                appModbusSyn[in_solidNum].msSynStatsus = STATUS_CHECK_CONFLICT;
            }
            else if(pbc_pull_timerIsCompleted(&appModbusSyn[in_solidNum].send_wait_delay))
            {
                appModbusSyn[in_solidNum].msSynStatsus = STATUS_CHECK_CONFLICT;
            }
            else
            {
                if(appModbusSyn[in_solidNum].updataWord)
                {
                    appModbusSyn[in_solidNum].msSynStatsus = STATUS_CHECK_CONFLICT;   
                }
            }
            break;
        }
        case STATUS_CHECK_CONFLICT:
        {
            if(msRunS_idle == mde_msRtu_master_slave_run_status(in_solidNum))
            {
                mde_master_slave_link_message_check_conflict(in_solidNum,PROTOL_CHECK_CONFICT);
                appModbusSyn[in_solidNum].msSynStatsus = STATUS_CONFLICT_RESULT; 
            }
            break;
        }      
        case STATUS_CONFLICT_RESULT:
        {             
            if(msRunS_conflict_wait == mde_msRtu_master_slave_run_status(in_solidNum))
            {//冲突则延时重发
                pbc_reload_timerClock(&appModbusSyn[in_solidNum].send_wait_delay,GetRandomDelayTime());
                appModbusSyn[in_solidNum].msSynStatsus = STATUS_IDLE; 
            } 
            else if(msRunS_idle == mde_msRtu_master_slave_run_status(in_solidNum))
            {
                if(appModbusSyn[in_solidNum].sendWord&SEND_RTC_BIT)
                {//发送RTC
                    appModbusSyn[in_solidNum].msSynStatsus = STATUS_SEND_RTC;
                }
                else if(appModbusSyn[in_solidNum].sendWord&SEND_MESSAGE_BIT)
                {
                    appModbusSyn[in_solidNum].msSynStatsus = STATUS_TIMINGSEND;
                }    
                else if(appModbusSyn[in_solidNum].updataWord)
                {//更新字
                    appModbusSyn[in_solidNum].msSynStatsus = STATUS_SEND_UPDATA_LIST;
                }
                else if(appModbusSyn[in_solidNum].occpyWord)
                {//抢占字
                    appModbusSyn[in_solidNum].msSynStatsus = STATUS_OCCPYSEND;
                }                     
            }            
            break;
        }
        case STATUS_SEND_RTC:
        {
            if(msRunS_idle == mde_msRtu_master_slave_run_status(in_solidNum))
            {
                regeditNum = app_master_slave_pull_device_list_num(in_solidNum);   
                for(i = 0;i < regeditNum;i++)
                {   
                    if(pull_device_type(in_solidNum,i) == DEVICE_TYPE_CC)
                    {
                        appModbusSyn[in_solidNum].msSynStatsus = STATUS_RECEIVE_RTC;
                        app_master_slave_send_one_block(in_solidNum,pull_device_id(in_solidNum,i),MRegaddr_SecondAndWeek,4,true);  
                        CCOnlineFlag = true;     
                    }
                }
                if(CCOnlineFlag == false)
                {//找不到输配则直接跳回空闲状态
                    appModbusSyn[in_solidNum].msSynStatsus = STATUS_IDLE;
                    appModbusSyn[in_solidNum].sendWord &= ~SEND_RTC_BIT;
                }
            }
            break;
        }
        case STATUS_RECEIVE_RTC:
        {
            if(mRtuS_master_slave_complete == mde_msRtu_master_slave_reveive_status(in_solidNum))
            {//这一帧数据符合要求
                appModbusSyn[in_solidNum].sendWord &= (~SEND_RTC_BIT);
                //appModbusSyn[in_solidNum].sendWord |=SEND_MESSAGE_BIT;
                appModbusSyn[in_solidNum].msSynStatsus = STATUS_IDLE;
            }
            else if((mRtuS_master_slave_timeout | mRtuS_master_slave_poterr) & mde_msRtu_master_slave_reveive_status(in_solidNum))
            {
                appModbusSyn[in_solidNum].sendWord &= (~SEND_RTC_BIT);
              //  appModbusSyn[in_solidNum].sendWord |=SEND_MESSAGE_BIT;
                appModbusSyn[in_solidNum].msSynStatsus = STATUS_IDLE;
            } 
            else
            {
                if(msRunS_conflict_wait == mde_msRtu_master_slave_run_status(in_solidNum))
                {   
                    appModbusSyn[in_solidNum].sendWord &= (~SEND_RTC_BIT);
                 //   appModbusSyn[in_solidNum].sendWord |=SEND_MESSAGE_BIT;
                    pbc_reload_timerClock(&appModbusSyn[in_solidNum].send_wait_delay,GetRandomDelayTime());
                    appModbusSyn[in_solidNum].msSynStatsus = STATUS_IDLE;
                }
            }          
            break;
        }
        case STATUS_OCCPYSEND:
        {  
            if(msRunS_idle == mde_msRtu_master_slave_run_status(in_solidNum))
            {
                if(appModbusSyn[in_solidNum].occpyWord & SET_POWER)
                {
                    
                    if((DEVICE_TYPE == 0x8521) || (DEVICE_TYPE == 0x9522) || (DEVICE_TYPE == 0x9521))
                    {//环控机/房间温控器/风盘
                        regeditNum = app_master_slave_pull_device_list_num(in_solidNum);                            
                        for(i = 0;i < regeditNum;i++)
                        {//找到输配中心结束
                            if(pull_device_type(in_solidNum,i) == DEVICE_TYPE_CC)
                            {
                                //app_master_slave_send_one_block(in_solidNum,pull_device_id(in_solidNum,i),MRegaddr_NewairStatusW,1,true);
                                appModbusSyn[in_solidNum].msSynStatsus = STATUS_OCCPYRECEIVE;
                                CCOnlineFlag = true;
                                break;
                            }
                        }
                        if(CCOnlineFlag == false)
                        {
                            appModbusSyn[in_solidNum].msSynStatsus = STATUS_IDLE;
                            appModbusSyn[in_solidNum].occpyWord &= ~SET_POWER;
                        }
                    }
                    else if(DEVICE_TYPE == 0x8522)
                    {//输配中心
                        if(appModbusSyn[in_solidNum].updataWord)
                        {                                
                        }
                        else
                        {
                            regeditNum = app_master_slave_pull_device_list_num(in_solidNum);
                            for(i = 0;i < regeditNum;i++)
                            {//形成同步列表
                                if((pull_device_type(in_solidNum,i) == DEVICE_TYPE_FAN)||
                                (pull_device_type(in_solidNum,i) == DEVICE_TYPE_HC)||
                                (pull_device_type(in_solidNum,i) == DEVICE_TYPE_ROMM))
                                {
                                    appModbusSyn[in_solidNum].updataWord |= (0x01<<i);
                                }
                            }
                        }                            
                        appModbusSyn[in_solidNum].msSynStatsus = STATUS_SEND_UPDATA_LIST;
                    }
                }
            }         
            break;
        } 
        case STATUS_OCCPYRECEIVE:
        {
            if(mRtuS_master_slave_complete == mde_msRtu_master_slave_reveive_status(in_solidNum))
            {//这一帧数据符合要求
                appModbusSyn[in_solidNum].occpyWord &= ~SET_POWER;
                appModbusSyn[in_solidNum].msSynStatsus = STATUS_IDLE;
            }
            else if((mRtuS_master_slave_timeout | mRtuS_master_slave_poterr) & mde_msRtu_master_slave_reveive_status(in_solidNum))
            {
                appModbusSyn[in_solidNum].occpyWord &= ~SET_POWER;
                appModbusSyn[in_solidNum].msSynStatsus = STATUS_IDLE;
            } 
            else
            {
                if(msRunS_conflict_wait == mde_msRtu_master_slave_run_status(in_solidNum))
                {   
                    appModbusSyn[in_solidNum].occpyWord &= ~SET_POWER;
                    pbc_reload_timerClock(&appModbusSyn[in_solidNum].send_wait_delay,GetRandomDelayTime());
                    appModbusSyn[in_solidNum].msSynStatsus = STATUS_IDLE;
                }
            }          
            break;
        }  
        case STATUS_SEND_UPDATA_LIST:
        {  
            regeditNum = app_master_slave_pull_device_list_num(in_solidNum);
            for(i = 0;i < regeditNum;i++)
            {
                if(appModbusSyn[in_solidNum].updataWord & (0x01 << i))
                {
                    appModbusSyn[in_solidNum].updataWord &= (~(0x01<< i));
                 //   app_master_slave_send_one_block(in_solidNum,pull_device_id(in_solidNum,i),MRegaddr_NewairStatusW,1,true);
                    appModbusSyn[in_solidNum].msSynStatsus = STATUS_RECEIVE_UPDATA_LIST;
                    break;
                }
            }
            break;
        } 
        case STATUS_RECEIVE_UPDATA_LIST:
        {
            if(mRtuS_master_slave_complete == mde_msRtu_master_slave_reveive_status(in_solidNum))
            {//这一帧数据符合要求
                appModbusSyn[in_solidNum].msSynStatsus = STATUS_IDLE;
                pbc_reload_timerClock(&appModbusSyn[in_solidNum].send_wait_delay,100);            
                if(appModbusSyn[in_solidNum].updataWord == 0)
                {
                    appModbusSyn[in_solidNum].occpyWord &= ~SET_POWER;
                }
            }
            else if((mRtuS_master_slave_poterr == mde_msRtu_master_slave_reveive_status(in_solidNum)) || (mRtuS_master_slave_timeout == mde_msRtu_master_slave_reveive_status(in_solidNum)))
            {//超时或帧错误
                appModbusSyn[in_solidNum].msSynStatsus  = STATUS_IDLE;
                pbc_reload_timerClock(&appModbusSyn[in_solidNum].send_wait_delay,100);
                if(appModbusSyn[in_solidNum].updataWord == 0)
                {
                    appModbusSyn[in_solidNum].occpyWord &= ~SET_POWER;
                    appModbusSyn[in_solidNum].msSynStatsus = STATUS_IDLE;
                }
            } 
            else
            { //冲突     
                if(msRunS_conflict_wait == mde_msRtu_master_slave_run_status(in_solidNum))
                {   
                    appModbusSyn[in_solidNum].msSynStatsus  = STATUS_IDLE;
                    pbc_reload_timerClock(&appModbusSyn[in_solidNum].send_wait_delay,100);
                    if(appModbusSyn[in_solidNum].updataWord == 0)
                    {
                        appModbusSyn[in_solidNum].occpyWord &= ~SET_POWER;
                        appModbusSyn[in_solidNum].msSynStatsus = STATUS_IDLE;
                    }
                }
            } 
            break;
        }    
        case STATUS_TIMINGSEND:
        {  
            appModbusSyn[in_solidNum].msSynStatsus = STATUS_IDLE;//空闲状态
            break;
        } 
        case STATUS_TIMINGRECEIVE:
        {
            if(mRtuS_master_slave_complete == mde_msRtu_master_slave_reveive_status(in_solidNum))
            {//这一帧数据符合要求
                appModbusSyn[in_solidNum].msSynStatsus = STATUS_IDLE;
            }
            else if((mRtuS_master_slave_poterr == mde_msRtu_master_slave_reveive_status(in_solidNum)) || (mRtuS_master_slave_timeout == mde_msRtu_master_slave_reveive_status(in_solidNum)))
            {
                appModbusSyn[in_solidNum].msSynStatsus = STATUS_IDLE;
            } 
            else
            {
                if(msRunS_conflict_wait == mde_msRtu_master_slave_run_status(in_solidNum))
                {   
                    pbc_reload_timerClock(&appModbusSyn[in_solidNum].send_wait_delay,GetRandomDelayTime());
                    appModbusSyn[in_solidNum].msSynStatsus = STATUS_IDLE;
                }
            }          
            break;
        }  
        case STATUS_DISPOSE_DATA:
        {
            appModbusSyn[in_solidNum].msSynStatsus = STATUS_IDLE;
            break;
        }
        default:
        {
            break;
        }         
    }
}

void system_parameter_change_task(void)
{//数据有变化更新时间戳
    uint8_t i = 0;
    if(app_general_pull_system_parameter_change())
    {
        for(i = 0; i < MAX_MODBUS_NUM;i++)
        {
             app_master_slave_push_occpy(i,SET_POWER);
        }      
        ownStamp = pbc_timeStamp_get_stamp(); //最新时间戳
        needStamp = pbc_timeStamp_get_stamp(); //最新时间戳
    }   
}

void app_modbus_syn_task(void)
{
    uint8_t i = 0;
    static bool cfg = false;
    if(cfg)
    {
        static bool lastOnlineFlag[MAX_MODBUS_NUM] = {false};
        for(i = 0; i < MAX_MODBUS_NUM;i++)
        {     
            if(pull_local_device_online(i))
            {
                system_parameter_change_task();
                app_master_slave_syn_task(i);
                if(lastOnlineFlag[i] == false)
                {//断线变上线
                    lastOnlineFlag[i] = true;
                    pbc_reload_timerClock(&appModbusSyn[i].timing_rtc_dalay,30);
                    pbc_reload_timerClock(&appModbusSyn[i].timing_send_dalay,60);
                }
            }   
            else
            {
                lastOnlineFlag[i] = false;
                appModbusSyn[i].msSynStatsus = STATUS_IDLE;
            }       
        }    
    }
    else
    {
        cfg = true;
        for(i = 0; i < MAX_MODBUS_NUM;i++)
        {
            appModbusSyn[i].timing_send_dalay.timStatusBits = timerType_second;
            appModbusSyn[i].timing_rtc_dalay.timStatusBits = timerType_second;
        }       
    }  
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
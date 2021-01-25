//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include ".\app_cfg.h"
#include "stdlib.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef enum
{
    STATUS_IDLE                = 0x00,
	STATUS_TIMINGSEND,          
    STATUS_CHECK_CONFLICT, 
    STATUS_CONFLICT_RESULT, 
    STATUS_SEND_SYN_DATA,
    STATUS_SEND_UPDATA_CHECK,
    STATUS_SEND_UPDATA_LIST,
    STATUS_RECEIVE_SYN_DATA,
    STATUS_RECEIVE_UPDATA_LIST,
	STATUS_TIMINGRECEIVE,        
	STATUS_OCCPYSEND,           
	STATUS_OCCPYRECEIVE,         
	STATUS_RESEND,             
	STATUS_RERECEIVE ,           
	STATUS_TIMEOUT,             
	STATUS_ERROR,                    
}msStatus_def;


#define PROTOL_CHECK_CONFICT     0xD020  //报文冲突检测
#define PROTOL_SYN               0xD021  //数据同步
#define PROTOL_REGEDIT           0xD022  //报文冲突检测
#define PROTOL_VERSION           0x01  //协议版本
#define CONTROL_REGEDIT          0x01//注册
#define CONTROL_UPDATA           0x02//更新


typedef struct
{
    bool     onlineFlag;       //在线标志
    uint16_t deviceType;       //设备类型
    uint8_t  DeviceID[6];      //ID
}msDeviceList_t;

typedef struct
{
    msStatus_def    msSynStatsus;       //同步状态
    msStatus_def    msRegStatsus;     //注册状态
    uint16_t        occpyWord;       //抢占字
    uint16_t        updataWord;      //更新字
    msDeviceList_t  deviceList[16];  //设备列表
}msComm_t;


msComm_t app485MasterSlave;

sdt_int8u local_addr[6]={0xac,0x20,0x10,0x23,0x02,0x02};               
sdt_int8u dest_addr[6]={0xab,0x20,0x10,0x23,0x00,0x01};
sdt_int8u board_addr[6]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
bool updataFlag = false;
macro_createTimer(send_timing_delay,timerType_second,0);
macro_createTimer(send_delay,timerType_millisecond,0);
macro_createTimer(send_boardcast_delay,timerType_second,0);
macro_createTimer(second_rev_delay,timerType_second,0);
macro_createTimer(send_updata_delay,timerType_millisecond,0);

void app_master_slave_push_occpy(uint16_t in_occpyBit)
{
    app485MasterSlave.occpyWord |= in_occpyBit;
    app485MasterSlave.msSynStatsus = STATUS_CHECK_CONFLICT;
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

uint8_t app_master_slave_pull_device_list_num(void)
{
    uint8_t i = 0;
    uint8_t num =0;
    for(i = 0;i < 16;i++)
    {
        if(app485MasterSlave.deviceList[i].onlineFlag == true)
        {
            num++;
        }
    }
    return num;
}

void app_master_slave_send_regedit_data(uint8_t *in_dest_addr,uint8_t control_cmd,bool responseFlag)
{
    ms_link_comm_data_def tempLinkData;
    uint8_t regeditNum = 0;

    mde_master_slave_link_message_transmit(MASTER_SLAVE_TEST,in_dest_addr,PROTOL_REGEDIT,responseFlag); 
    tempLinkData.Payload[0] = PROTOL_VERSION;
    tempLinkData.Payload[1] = control_cmd;
    tempLinkData.Payload[2] = 0x00;//预留
    if(CONTROL_REGEDIT == control_cmd)
    {
        tempLinkData.Payload[3] = (uint8_t)(DEVICE_TYPE>>8);//设备类型
        tempLinkData.Payload[4] = (uint8_t)DEVICE_TYPE;
        tempLinkData.PayloadLength = 5;
    }
    else if(CONTROL_UPDATA == control_cmd)
    {
        regeditNum = app_master_slave_pull_device_list_num();
        tempLinkData.Payload[3] = regeditNum;
        uint8_t i = 0;
        for(i = 0;i < regeditNum;i++)
        {
            tempLinkData.Payload[4+8*i] =  (uint8_t)(app485MasterSlave.deviceList[i].deviceType>>8); 
            tempLinkData.Payload[5+8*i] =  (uint8_t)(app485MasterSlave.deviceList[i].deviceType);
            tempLinkData.Payload[6+8*i] =  (uint8_t)(app485MasterSlave.deviceList[i].DeviceID[0]);
            tempLinkData.Payload[7+8*i] =  (uint8_t)(app485MasterSlave.deviceList[i].DeviceID[1]);
            tempLinkData.Payload[8+8*i] =  (uint8_t)(app485MasterSlave.deviceList[i].DeviceID[2]);
            tempLinkData.Payload[9+8*i] =  (uint8_t)(app485MasterSlave.deviceList[i].DeviceID[3]);
            tempLinkData.Payload[10+8*i] =  (uint8_t)(app485MasterSlave.deviceList[i].DeviceID[4]);
            tempLinkData.Payload[11+8*i] =  (uint8_t)(app485MasterSlave.deviceList[i].DeviceID[5]);
        }
        tempLinkData.PayloadLength = (4+tempLinkData.Payload[3]*8);
    }
    mde_master_slave_push_link_onemessage(MASTER_SLAVE_TEST,&tempLinkData);
}

void app_master_slave_send_regedit_response_data(uint8_t *in_dest_addr,uint8_t control_cmd)
{
    ms_link_comm_data_def tempLinkData;
    mde_master_slave_link_message_transmit(MASTER_SLAVE_TEST,in_dest_addr,PROTOL_REGEDIT,false); 
    tempLinkData.Payload[0] = PROTOL_VERSION;
    tempLinkData.Payload[1] = control_cmd|0x80;
    tempLinkData.PayloadLength = 2;
    mde_master_slave_push_link_onemessage(MASTER_SLAVE_TEST,&tempLinkData);
}

void app_master_slave_send_one_block(uint8_t *in_dest_addr,uint16_t in_regAddr,uint8_t in_regLength,bool responseFlag)
{
    ms_link_comm_data_def tempLinkData;
    mde_master_slave_link_message_transmit(MASTER_SLAVE_TEST,in_dest_addr,PROTOL_SYN,responseFlag); 
    tempLinkData.Payload[0] = PROTOL_VERSION;
    tempLinkData.Payload[1] = 0x00;
    if(responseFlag)
    {
        tempLinkData.Payload[1] = 0x01; 
    }
    tempLinkData.Payload[2] = 0x00;//预留
    tempLinkData.Payload[3] = 0x01;//块数量
    tempLinkData.Payload[4] = (uint8_t)(StoRunParameter.timeStamp>>24);//时间戳
    tempLinkData.Payload[5] =(uint8_t)(StoRunParameter.timeStamp>>16);
    tempLinkData.Payload[6] =(uint8_t)(StoRunParameter.timeStamp>>8);
    tempLinkData.Payload[7] =(uint8_t)(StoRunParameter.timeStamp);
    tempLinkData.Payload[8] = ((in_regLength<<1) + 3);
    tempLinkData.Payload[9] = (uint8_t)(in_regAddr>>8);
    tempLinkData.Payload[10] = (uint8_t)(in_regAddr);
    app_push_send_reg_485(in_regAddr,in_regLength,&tempLinkData.Payload[11]);
    tempLinkData.PayloadLength = (11+(in_regLength<<1));
    mde_master_slave_push_link_onemessage(MASTER_SLAVE_TEST,&tempLinkData);
}

void app_master_slave_response_one_block(uint8_t *in_dest_addr,uint16_t in_regAddr,uint8_t in_regLength)
{
    ms_link_comm_data_def tempLinkData;
    mde_master_slave_link_message_transmit(MASTER_SLAVE_TEST,in_dest_addr,PROTOL_SYN,false); 
    tempLinkData.Payload[0] = PROTOL_VERSION;  
    tempLinkData.Payload[1] = 0x81;//应答控制
    tempLinkData.Payload[2] = 0x00;//预留
    tempLinkData.Payload[3] = 0x01;//块数量
    tempLinkData.Payload[4] = 0x01;//时间戳
    tempLinkData.Payload[5] = 0x03;
    tempLinkData.Payload[6] = 0x05;
    tempLinkData.Payload[7] = 0x11;
    tempLinkData.Payload[8] = 3;
    tempLinkData.Payload[9] = (uint8_t)(in_regAddr>>8);
    tempLinkData.Payload[10] = (uint8_t)(in_regAddr);   
    tempLinkData.PayloadLength = 11;
    mde_master_slave_push_link_onemessage(MASTER_SLAVE_TEST,&tempLinkData);
}

uint16_t GetRandomDelayTime(void)
{//20-3020ms的随机数
    uint16_t result = 0;
    result = (rand()&0x64)*20+ 500;/* random factor 0-47*/      
    return  result;
}

bool app_master_slave_pull_device_list(uint8_t in_port,uint16_t* out_deviceType)
{
    *out_deviceType = app485MasterSlave.deviceList[in_port].deviceType;
    if(app485MasterSlave.deviceList[in_port].onlineFlag)
    {
        return true;
    }
    return false;
}

bool pull_boardcast_id(uint8_t *in_deviceId)
{
    if(((in_deviceId[0] == 0xff)&&\
    (in_deviceId[1] == 0xff)&&\
    (in_deviceId[2] == 0xff)&&\
    (in_deviceId[3] == 0xff)&&\
    (in_deviceId[4] == 0xff)&&\
    (in_deviceId[5] == 0xff)))
    {
        return true;
    }
    return false;
}

bool pull_local_id(uint8_t *in_deviceId)
{
   if((in_deviceId[0] == local_addr[0])&&\
    (in_deviceId[1] == local_addr[1])&&\
    (in_deviceId[2] == local_addr[2])&&\
    (in_deviceId[3] == local_addr[3])&&\
    (in_deviceId[4] == local_addr[4])&&\
    (in_deviceId[5] == local_addr[5]))
    {
        return true;
    }
    return false;
}

bool pull_first_device_id(uint8_t *in_deviceId)
{
   if((in_deviceId[0] ==app485MasterSlave.deviceList[0].DeviceID[0])&&\
    (in_deviceId[1] == app485MasterSlave.deviceList[0].DeviceID[1])&&\
    (in_deviceId[2] == app485MasterSlave.deviceList[0].DeviceID[2])&&\
    (in_deviceId[3] == app485MasterSlave.deviceList[0].DeviceID[3])&&\
    (in_deviceId[4] == app485MasterSlave.deviceList[0].DeviceID[4])&&\
    (in_deviceId[5] == app485MasterSlave.deviceList[0].DeviceID[5]))
    {
        return true;
    }
    return false;
}

bool pull_second_device_id(uint8_t *in_deviceId)
{
   if((in_deviceId[0] == app485MasterSlave.deviceList[1].DeviceID[0])&&\
    (in_deviceId[1] == app485MasterSlave.deviceList[1].DeviceID[1])&&\
    (in_deviceId[2] == app485MasterSlave.deviceList[1].DeviceID[2])&&\
    (in_deviceId[3] == app485MasterSlave.deviceList[1].DeviceID[3])&&\
    (in_deviceId[4] == app485MasterSlave.deviceList[1].DeviceID[4])&&\
    (in_deviceId[5] == app485MasterSlave.deviceList[1].DeviceID[5]))
    {
        return true;
    }
    return false;
}

void app_master_slave_updata_device_list(void)
{
    uint8_t deviceNum = 0;
    if(app485MasterSlave.deviceList[0].onlineFlag )
    {
        deviceNum = app_master_slave_pull_device_list_num();
        if(deviceNum == 1)
        {
            app485MasterSlave.deviceList[0].onlineFlag = false;//踢掉0号设备
        }
        else if(deviceNum > 1)
        {
            uint8_t i;
            for(i = 1;i < deviceNum;i++)
            {
                app485MasterSlave.deviceList[i-1].onlineFlag = app485MasterSlave.deviceList[i].onlineFlag;
                app485MasterSlave.deviceList[i-1].deviceType =  app485MasterSlave.deviceList[i].deviceType;
                app485MasterSlave.deviceList[i-1].DeviceID[0] = app485MasterSlave.deviceList[i].DeviceID[0];
                app485MasterSlave.deviceList[i-1].DeviceID[1] = app485MasterSlave.deviceList[i].DeviceID[1];
                app485MasterSlave.deviceList[i-1].DeviceID[2] = app485MasterSlave.deviceList[i].DeviceID[2];
                app485MasterSlave.deviceList[i-1].DeviceID[3] = app485MasterSlave.deviceList[i].DeviceID[3];
                app485MasterSlave.deviceList[i-1].DeviceID[4] = app485MasterSlave.deviceList[i].DeviceID[4];
                app485MasterSlave.deviceList[i-1].DeviceID[5] = app485MasterSlave.deviceList[i].DeviceID[5];
                app485MasterSlave.deviceList[i].onlineFlag = false;
            }
        }       
    }  
}


bool deviceChangeFlag = false;
void app_master_slave_dispose_regedit_data(ms_link_comm_data_def *in_rev_data)
{
    uint8_t i = 0;
    bool newIdFlag = true;
    uint8_t deviceNum = 0;
    deviceNum = app_master_slave_pull_device_list_num();
    
    if(pull_boardcast_id(&in_rev_data->LinkDstAddr[0]))
    {//广播地址    
        if(deviceNum == 0)
        {//接收到广播地址同时设备列表里为0
            app485MasterSlave.deviceList[0].onlineFlag = true;
            app485MasterSlave.deviceList[0].DeviceID[0] = in_rev_data->LinkSrcAddr[0];
            app485MasterSlave.deviceList[0].DeviceID[1] = in_rev_data->LinkSrcAddr[1];
            app485MasterSlave.deviceList[0].DeviceID[2] = in_rev_data->LinkSrcAddr[2];
            app485MasterSlave.deviceList[0].DeviceID[3] = in_rev_data->LinkSrcAddr[3];
            app485MasterSlave.deviceList[0].DeviceID[4] = in_rev_data->LinkSrcAddr[4];
            app485MasterSlave.deviceList[0].DeviceID[5] = in_rev_data->LinkSrcAddr[5];
            pbc_reload_timerClock(&send_delay,0);//立即发送
        }
        else if(pull_first_device_id(&in_rev_data->LinkSrcAddr[0]))
        {//收到1号设备的广播帧
            if(deviceNum == 1)
            {
                if(app485MasterSlave.msRegStatsus == STATUS_IDLE)
                {
                    app485MasterSlave.msRegStatsus = STATUS_CHECK_CONFLICT;
                }
            }
            else
            {
                if(pull_second_device_id(&local_addr[0]))
                {//2号设备收到则直接更新列表
                    deviceChangeFlag = true;//发送列表
                    app_master_slave_updata_device_list();
                }  
            }
        }
        else if(pull_first_device_id(&local_addr[0]))
        {//后上电设备加入列表
            deviceChangeFlag = true;
            for(i = 1;i < 16;i++)
            {       
                if(app485MasterSlave.deviceList[i].onlineFlag == true)
                {
                     if((in_rev_data->LinkSrcAddr[0] ==  app485MasterSlave.deviceList[i].DeviceID[0])&&\
                        (in_rev_data->LinkSrcAddr[1] ==  app485MasterSlave.deviceList[i].DeviceID[1])&&\
                        (in_rev_data->LinkSrcAddr[2] ==  app485MasterSlave.deviceList[i].DeviceID[2])&&\
                        (in_rev_data->LinkSrcAddr[3] ==  app485MasterSlave.deviceList[i].DeviceID[3])&&\
                        (in_rev_data->LinkSrcAddr[4] ==  app485MasterSlave.deviceList[i].DeviceID[4])&&\
                        (in_rev_data->LinkSrcAddr[5] ==  app485MasterSlave.deviceList[i].DeviceID[5]))
                    {
                        newIdFlag = false;
                    }
                }
            }
            if(newIdFlag)
            {
                for(i = 1;i < 16;i++)
                {
                    if(app485MasterSlave.deviceList[i].onlineFlag == false)
                    {
                        app485MasterSlave.deviceList[i].onlineFlag = true;
                        app485MasterSlave.deviceList[i].deviceType = pbc_arrayToInt16u_bigEndian(&in_rev_data->Payload[3]);
                        app485MasterSlave.deviceList[i].DeviceID[0] = in_rev_data->LinkSrcAddr[0];
                        app485MasterSlave.deviceList[i].DeviceID[1] = in_rev_data->LinkSrcAddr[1];
                        app485MasterSlave.deviceList[i].DeviceID[2] = in_rev_data->LinkSrcAddr[2];
                        app485MasterSlave.deviceList[i].DeviceID[3] = in_rev_data->LinkSrcAddr[3];
                        app485MasterSlave.deviceList[i].DeviceID[4] = in_rev_data->LinkSrcAddr[4];
                        app485MasterSlave.deviceList[i].DeviceID[5] = in_rev_data->LinkSrcAddr[5];
                        break;
                    }
                }
            }
        }
    }
    else if(pull_local_id(&in_rev_data->LinkDstAddr[0]))       
    {//自己地址
        uint8_t regeditNum = in_rev_data->Payload[3];
        if(pull_first_device_id(&in_rev_data->LinkSrcAddr[0]))
        {//1号设备发过来的
            pbc_reload_timerClock(&second_rev_delay,190);
        }
        if(in_rev_data->Payload[1] == CONTROL_UPDATA)
        {//更新应答
            for(i = 0;i < regeditNum;i++)
            {
                app485MasterSlave.deviceList[i].onlineFlag = true;
                app485MasterSlave.deviceList[i].deviceType = pbc_arrayToInt16u_bigEndian(&in_rev_data->Payload[4+i*8]);
                app485MasterSlave.deviceList[i].DeviceID[0] = in_rev_data->Payload[6+i*8];
                app485MasterSlave.deviceList[i].DeviceID[1] = in_rev_data->Payload[7+i*8];
                app485MasterSlave.deviceList[i].DeviceID[2] = in_rev_data->Payload[8+i*8];
                app485MasterSlave.deviceList[i].DeviceID[3] = in_rev_data->Payload[9+i*8];
                app485MasterSlave.deviceList[i].DeviceID[4] = in_rev_data->Payload[10+i*8];
                app485MasterSlave.deviceList[i].DeviceID[5] = in_rev_data->Payload[11+i*8];
            }
            for(i = regeditNum;i < 16;i++)
            {
                app485MasterSlave.deviceList[i].onlineFlag = false;
            }
            pbc_reload_timerClock(&second_rev_delay,190);
            pbc_reload_timerClock(&send_delay,GetRandomDelayTime());//先响应再更新
            app_master_slave_send_regedit_response_data(&in_rev_data->LinkSrcAddr[0],in_rev_data->Payload[1]);
        }
        else if(in_rev_data->Payload[1] == CONTROL_REGEDIT)
        {//注册应答
            pbc_reload_timerClock(&send_delay,GetRandomDelayTime());//先响应再更新
            app_master_slave_send_regedit_response_data(&in_rev_data->LinkSrcAddr[0],in_rev_data->Payload[1]);
            if(app485MasterSlave.deviceList[0].onlineFlag == false)
            {
                app485MasterSlave.deviceList[0].onlineFlag = true;
                app485MasterSlave.deviceList[0].deviceType = DEVICE_TYPE;
                app485MasterSlave.deviceList[0].DeviceID[0] = in_rev_data->LinkDstAddr[0];
                app485MasterSlave.deviceList[0].DeviceID[1] = in_rev_data->LinkDstAddr[1];
                app485MasterSlave.deviceList[0].DeviceID[2] = in_rev_data->LinkDstAddr[2];
                app485MasterSlave.deviceList[0].DeviceID[3] = in_rev_data->LinkDstAddr[3];
                app485MasterSlave.deviceList[0].DeviceID[4] = in_rev_data->LinkDstAddr[4];
                app485MasterSlave.deviceList[0].DeviceID[5] = in_rev_data->LinkDstAddr[5];
            }
            
            for(i = 1;i < 16;i++)
            {       
                if(app485MasterSlave.deviceList[i].onlineFlag == true)
                {
                     if((in_rev_data->LinkSrcAddr[0] ==  app485MasterSlave.deviceList[i].DeviceID[0])&&\
                        (in_rev_data->LinkSrcAddr[1] ==  app485MasterSlave.deviceList[i].DeviceID[1])&&\
                        (in_rev_data->LinkSrcAddr[2] ==  app485MasterSlave.deviceList[i].DeviceID[2])&&\
                        (in_rev_data->LinkSrcAddr[3] ==  app485MasterSlave.deviceList[i].DeviceID[3])&&\
                        (in_rev_data->LinkSrcAddr[4] ==  app485MasterSlave.deviceList[i].DeviceID[4])&&\
                        (in_rev_data->LinkSrcAddr[5] ==  app485MasterSlave.deviceList[i].DeviceID[5]))
                    {
                        newIdFlag = false;
                    }
                }
            }
            if(newIdFlag)
            {
                for(i = 1;i < 16;i++)
                {
                    if(app485MasterSlave.deviceList[i].onlineFlag == false)
                    {
                        app485MasterSlave.deviceList[i].onlineFlag = true;
                        pbc_reload_timerClock(&second_rev_delay,190);
                        app485MasterSlave.deviceList[i].deviceType = pbc_arrayToInt16u_bigEndian(&in_rev_data->Payload[3]);
                        app485MasterSlave.deviceList[i].DeviceID[0] = in_rev_data->LinkSrcAddr[0];
                        app485MasterSlave.deviceList[i].DeviceID[1] = in_rev_data->LinkSrcAddr[1];
                        app485MasterSlave.deviceList[i].DeviceID[2] = in_rev_data->LinkSrcAddr[2];
                        app485MasterSlave.deviceList[i].DeviceID[3] = in_rev_data->LinkSrcAddr[3];
                        app485MasterSlave.deviceList[i].DeviceID[4] = in_rev_data->LinkSrcAddr[4];
                        app485MasterSlave.deviceList[i].DeviceID[5] = in_rev_data->LinkSrcAddr[5];
                        deviceChangeFlag = true;
                        break;
                    }
                }
            }
        }
    }
    else
    {//不是自己的数据需要避让一下
        pbc_reload_timerClock(&send_delay,GetRandomDelayTime());
    }
}

void app_master_slave_dispose_syn_data(ms_link_comm_data_def *in_rev_data)
{//同步数据
    uint16_t regAdd = 0;
    uint8_t regLength = 0;
    uint8_t lengthIndex = 0;
    uint8_t blockLen = 0;
    uint8_t i = 0;
    uint8_t blockNum = 0;
    uint32_t timeStamp = 0;
    if(pull_local_id(&in_rev_data->LinkDstAddr[0])) 
    {
        blockNum = in_rev_data->Payload[3];
        if((in_rev_data->Payload[1] == 0x00) || (in_rev_data->Payload[1] == 0x01))
        {//主动发起任务
            timeStamp = pbc_arrayToInt32u_bigEndian(&in_rev_data->Payload[4]);
            if(timeStamp > StoRunParameter.timeStamp)
            {
                for(i = 0;i < blockNum;i++)
                {
                    lengthIndex = (4 + (i+1)*4);
                    if(i != 0)
                    {
                        lengthIndex += blockLen;
                    }
                    regAdd =  (((uint16_t)in_rev_data->Payload[lengthIndex+1]<<8) | in_rev_data->Payload[lengthIndex+2]);
                    regLength = ((in_rev_data->Payload[lengthIndex]-3)>>1);            
                    app_pull_receive_reg_485(regAdd,regLength,&in_rev_data->Payload[lengthIndex+3]);            
                    blockLen += in_rev_data->Payload[lengthIndex];            
                }
                StoRunParameter.timeStamp = timeStamp;
            }
            if(in_rev_data->Payload[1] == 0x01)
            {//主动发起任务响应数据
                app_master_slave_response_one_block(&in_rev_data->LinkSrcAddr[0],regAdd,regLength);
            }
        }  
    }
    else
    {//不是自己的数据需要避让一下
        pbc_reload_timerClock(&send_delay,GetRandomDelayTime());
    }
    
}


bool pull_first_device(void)
{
     if(((app485MasterSlave.deviceList[0].DeviceID[0] == local_addr[0])&&\
    (app485MasterSlave.deviceList[0].DeviceID[1] == local_addr[1])&&\
    (app485MasterSlave.deviceList[0].DeviceID[2] == local_addr[2])&&\
    (app485MasterSlave.deviceList[0].DeviceID[3] == local_addr[3])&&\
    (app485MasterSlave.deviceList[0].DeviceID[4] == local_addr[4])&&\
    (app485MasterSlave.deviceList[0].DeviceID[5] == local_addr[5])))
    {
        return true;
    }
    return false;
}

void app_master_slave_regedit_timing_task(void)
{
    if(pull_first_device_id(&local_addr[0]))
    {//1号设备
        if((pbc_pull_timerIsCompleted(&send_timing_delay)) || (deviceChangeFlag))
        {
            if(app485MasterSlave.msRegStatsus == STATUS_IDLE)
            {
                pbc_reload_timerClock(&send_timing_delay,180);
                deviceChangeFlag = false;
                updataFlag = true;
                app485MasterSlave.msRegStatsus = STATUS_SEND_UPDATA_CHECK;
            }       
        }
    }
    else if(pull_second_device_id(&local_addr[0]))
    {//2号设备
        if((pbc_pull_timerIsCompleted(&second_rev_delay)))
        {//3分钟未收到1号设备数据
            pbc_reload_timerClock(&second_rev_delay,190);
            app_master_slave_updata_device_list();//更新列表
            deviceChangeFlag = true;//发送列表
        }
    }
    else
    {//其它设备掉线
        uint8_t i = 0;
        uint8_t regeditNum = 0;
        regeditNum = app_master_slave_pull_device_list_num();
        for(i = 0; i < regeditNum;i++)
        {
             if(pull_local_id(&app485MasterSlave.deviceList[i].DeviceID[0]))
             {
                  if((pbc_pull_timerIsCompleted(&second_rev_delay)))
                  {
                      uint8_t j = 0;
                      for(j = 0; j < regeditNum;j++)
                      {
                          app485MasterSlave.deviceList[j].onlineFlag = false;
                      }
                      app485MasterSlave.msRegStatsus = STATUS_CHECK_CONFLICT;
                  }
             }
        }
    }
}

void app_master_slave_regedit_task(void)
{
    ms_link_comm_data_def  pReceiveData;
    uint8_t deviceNum = 0;
    uint8_t i = 0;
    static uint8_t updataNumber = 1;
    static bool updateResultFlag = false;
    macro_createTimer(send_wait_delay,timerType_millisecond,0);
    pbc_timerClockRun_task(&send_delay);
    pbc_timerClockRun_task(&send_boardcast_delay);
    pbc_timerClockRun_task(&send_timing_delay);
    pbc_timerClockRun_task(&second_rev_delay);   
    app_master_slave_regedit_timing_task();
    switch (app485MasterSlave.msRegStatsus)
    {
        case STATUS_IDLE:
        {
            break;
        }
        case STATUS_CHECK_CONFLICT:
        {//检测冲突
            if((pbc_pull_timerIsCompleted(&send_delay)) && (pbc_pull_timerIsCompleted(&send_boardcast_delay)))
            {
                srand(send_delay.keepTick);
               // if(mde_msRtu_master_slave_phy_idle_status(MASTER_SLAVE_TEST))
                if(msRunS_idle == mde_msRtu_master_slave_run_status(MASTER_SLAVE_TEST))
                {
                    mde_master_slave_link_message_check_conflict(MASTER_SLAVE_TEST,PROTOL_CHECK_CONFICT);
                    app485MasterSlave.msRegStatsus = STATUS_CONFLICT_RESULT; 
                }
            }                            
            break;
        }  
        case STATUS_CONFLICT_RESULT:
        {             
            if(msRunS_transmit_conflict == mde_msRtu_master_slave_run_status(MASTER_SLAVE_TEST))
            {
                pbc_reload_timerClock(&send_delay,GetRandomDelayTime());
                app485MasterSlave.msRegStatsus = STATUS_CHECK_CONFLICT; 
            } 
            else if(msRunS_idle == mde_msRtu_master_slave_run_status(MASTER_SLAVE_TEST))
            {//能正常发送抢占帧则表明总线空闲
                if(updataFlag)
                {
                    updataFlag = false;
                    updateResultFlag = false;
                    app485MasterSlave.msRegStatsus = STATUS_SEND_UPDATA_LIST;
                    pbc_reload_timerClock(&send_wait_delay,40);//延时40ms
                }
                else
                {
                    app485MasterSlave.msRegStatsus = STATUS_SEND_SYN_DATA; 
                    pbc_reload_timerClock(&send_wait_delay,40);//延时40ms
                }
            }                     
            break;
        }
        case STATUS_SEND_SYN_DATA:
        {    
            if(pbc_pull_timerIsCompleted(&send_wait_delay))
            {
                if(msRunS_idle == mde_msRtu_master_slave_run_status(MASTER_SLAVE_TEST))
                {//空闲                           
                    static uint8_t sendCount = 0;  
                    if(app485MasterSlave.deviceList[0].onlineFlag == false)
                    {
                        app_master_slave_send_regedit_data(&board_addr[0],CONTROL_REGEDIT,false);
                        app485MasterSlave.msRegStatsus = STATUS_CHECK_CONFLICT;
                        if(sendCount)
                        {
                            if((30+sendCount) >= 180)
                            {
                                pbc_reload_timerClock(&send_boardcast_delay,20);//静默30s
                            }
                            else
                            {
                                pbc_reload_timerClock(&send_boardcast_delay,(30+sendCount));//静默时间累加
                                sendCount++;
                            }                   
                        }
                        else
                        {
                            sendCount++;
                            pbc_reload_timerClock(&send_delay,(30000+GetRandomDelayTime()));//静默30s
                        }          
                    } 
                    else
                    {
                        deviceNum = app_master_slave_pull_device_list_num();
                        if(deviceNum > 1)
                        {//更新列表
                            sendCount = 0;
                            if(pull_first_device())
                            {//1号设备进行同步
                                updataFlag = true;
                                app485MasterSlave.msRegStatsus = STATUS_SEND_UPDATA_CHECK;
                            }
                            else
                            {
                                app485MasterSlave.msRegStatsus = STATUS_IDLE;//释放总线
                            }
                            
                        }
                        else if(deviceNum == 1)
                        {     //接收同步数据               
                              sendCount = 0;
                              if(app485MasterSlave.deviceList[0].onlineFlag)
                              {
                                  app_master_slave_send_regedit_data(&app485MasterSlave.deviceList[0].DeviceID[0],CONTROL_REGEDIT,true);
                                  app485MasterSlave.msRegStatsus = STATUS_RECEIVE_SYN_DATA;
                              }   
                        }
                    }
                }               
            }   
            break;
        } 
        case STATUS_RECEIVE_SYN_DATA:
        {
            sdt_bool ReceiveOneMessageFlag;
            pReceiveData = mde_pull_msRtu_onemessage(MASTER_SLAVE_TEST,&ReceiveOneMessageFlag);
            if(ReceiveOneMessageFlag)
            {//这一帧数据符合要求
                if(pReceiveData.ProcotolType == PROTOL_REGEDIT) 
                {//注册帧
                    app_master_slave_dispose_regedit_data(&pReceiveData);
                    app485MasterSlave.msRegStatsus = STATUS_IDLE;//释放
                }                     
            }
            else if((mRtuS_master_slave_timeout | mRtuS_master_slave_poterr) & mde_msRtu_master_slave_reveive_status(MASTER_SLAVE_TEST))
            {
                pbc_reload_timerClock(&send_delay,GetRandomDelayTime());
                app485MasterSlave.msRegStatsus = STATUS_CHECK_CONFLICT;//释放
            } 
            else
            {
                if(msRunS_transmit_conflict == mde_msRtu_master_slave_run_status(MASTER_SLAVE_TEST))
                {   
                    pbc_reload_timerClock(&send_delay,GetRandomDelayTime());
                    app485MasterSlave.msRegStatsus = STATUS_CHECK_CONFLICT;//释放
                }
            }          
            break;
        }
        case STATUS_SEND_UPDATA_CHECK:
        {
            if(pbc_pull_timerIsCompleted(&send_wait_delay))
            {
               // if(mde_msRtu_master_slave_phy_idle_status(MASTER_SLAVE_TEST))
                if(msRunS_idle == mde_msRtu_master_slave_run_status(MASTER_SLAVE_TEST))
                {
                    mde_master_slave_link_message_check_conflict(MASTER_SLAVE_TEST,PROTOL_CHECK_CONFICT);
                    app485MasterSlave.msRegStatsus = STATUS_CONFLICT_RESULT; 
                }
            }
            break;
        }    
        case STATUS_SEND_UPDATA_LIST:
        {    
            if(pbc_pull_timerIsCompleted(&send_wait_delay))
            {
                if(msRunS_idle == mde_msRtu_master_slave_run_status(MASTER_SLAVE_TEST))
                {
                    if(app_master_slave_pull_device_list_num() == 1)
                    {
                         app485MasterSlave.deviceList[0].onlineFlag = false;
                         app485MasterSlave.msRegStatsus = STATUS_CHECK_CONFLICT;
                    }
                    else
                    {
                        app_master_slave_send_regedit_data(&app485MasterSlave.deviceList[updataNumber].DeviceID[0],CONTROL_UPDATA,true);
                        app485MasterSlave.msRegStatsus = STATUS_RECEIVE_UPDATA_LIST;
                    }
                }
            }
            
            break;
        }
        case STATUS_RECEIVE_UPDATA_LIST:
        {
            sdt_bool ReceiveOneMessageFlag;
            deviceNum = app_master_slave_pull_device_list_num();
            pReceiveData = mde_pull_msRtu_onemessage(MASTER_SLAVE_TEST,&ReceiveOneMessageFlag);
            if(ReceiveOneMessageFlag)
            {//这一帧数据符合要求
                if(pReceiveData.ProcotolType == PROTOL_REGEDIT) 
                {//注册帧
                    updateResultFlag  = true;
                    app_master_slave_dispose_regedit_data(&pReceiveData);
                    if(updataNumber>=(deviceNum-1))
                    {
                        updataNumber = 1;
                        if(updateResultFlag == false)
                        {//列表有变化更新列表
                            pbc_reload_timerClock(&send_delay,0);//立即发送
                            deviceChangeFlag = true;
                        }  
                        else
                        {
                            app485MasterSlave.msRegStatsus = STATUS_IDLE;//释放
                        }
                    }
                    else
                    {
                        updataFlag = true;
                        app485MasterSlave.msRegStatsus = STATUS_SEND_UPDATA_CHECK;  
                        pbc_reload_timerClock(&send_wait_delay,40);//延时40ms
                        updataNumber++;
                    }                  
                }                     
            }
            else if((mRtuS_master_slave_timeout | mRtuS_master_slave_poterr) & mde_msRtu_master_slave_reveive_status(MASTER_SLAVE_TEST))
            {
                updateResultFlag = false;
                for(i = updataNumber;i < deviceNum;i++ )     
                {
                    app485MasterSlave.deviceList[i].onlineFlag = app485MasterSlave.deviceList[i+1].onlineFlag;
                    app485MasterSlave.deviceList[i].deviceType =  app485MasterSlave.deviceList[i+1].deviceType;
                    app485MasterSlave.deviceList[i].DeviceID[0] = app485MasterSlave.deviceList[i+1].DeviceID[0];
                    app485MasterSlave.deviceList[i].DeviceID[1] = app485MasterSlave.deviceList[i+1].DeviceID[1];
                    app485MasterSlave.deviceList[i].DeviceID[2] = app485MasterSlave.deviceList[i+1].DeviceID[2];
                    app485MasterSlave.deviceList[i].DeviceID[3] = app485MasterSlave.deviceList[i+1].DeviceID[3];
                    app485MasterSlave.deviceList[i].DeviceID[4] = app485MasterSlave.deviceList[i+1].DeviceID[4];
                    app485MasterSlave.deviceList[i].DeviceID[5] = app485MasterSlave.deviceList[i+1].DeviceID[5];
                    app485MasterSlave.deviceList[i+1].onlineFlag = false;
                }
                deviceNum = app_master_slave_pull_device_list_num();
                if(deviceNum == 1)
                {//只剩一个设备时
                     app485MasterSlave.deviceList[0].onlineFlag = false;
                     app485MasterSlave.msRegStatsus = STATUS_CHECK_CONFLICT;
                }
                else
                {
                    if(updataNumber>=(deviceNum-1))
                    {
                        updataNumber = 1;
                        if(updateResultFlag == false)
                        {//列表有变化更新列表
                          //  app485MasterSlave.deviceList[0].onlineFlag = false;
                            pbc_reload_timerClock(&send_delay,0);//立即发送
                            deviceChangeFlag = true;
                           // app485MasterSlave.msRegStatsus = STATUS_CHECK_CONFLICT;
                        }  
                        else
                        {
                            app485MasterSlave.msRegStatsus = STATUS_IDLE;//释放
                        }
                    }
                    else
                    {
                        updataFlag = true;
                        app485MasterSlave.msRegStatsus = STATUS_SEND_UPDATA_CHECK;
                        pbc_reload_timerClock(&send_wait_delay,40);//延时40ms
                     //   updataNumber++;
                    }
                }
            } 
            else
            {
                
                if(msRunS_transmit_conflict == mde_msRtu_master_slave_run_status(MASTER_SLAVE_TEST))
                {   
                    updateResultFlag  = false;
                    for(i = updataNumber;i < deviceNum;i++ )     
                    {
                        app485MasterSlave.deviceList[i].onlineFlag = app485MasterSlave.deviceList[i+1].onlineFlag;
                        app485MasterSlave.deviceList[i].deviceType =  app485MasterSlave.deviceList[i+1].deviceType;
                        app485MasterSlave.deviceList[i].DeviceID[0] = app485MasterSlave.deviceList[i+1].DeviceID[0];
                        app485MasterSlave.deviceList[i].DeviceID[1] = app485MasterSlave.deviceList[i+1].DeviceID[1];
                        app485MasterSlave.deviceList[i].DeviceID[2] = app485MasterSlave.deviceList[i+1].DeviceID[2];
                        app485MasterSlave.deviceList[i].DeviceID[3] = app485MasterSlave.deviceList[i+1].DeviceID[3];
                        app485MasterSlave.deviceList[i].DeviceID[4] = app485MasterSlave.deviceList[i+1].DeviceID[4];
                        app485MasterSlave.deviceList[i].DeviceID[5] = app485MasterSlave.deviceList[i+1].DeviceID[5];
                        app485MasterSlave.deviceList[i+1].onlineFlag = false;
                    }
                    pbc_reload_timerClock(&send_delay,GetRandomDelayTime());
                    deviceNum = app_master_slave_pull_device_list_num();
                    if(deviceNum == 1)
                    {//只剩一个设备时
                         app485MasterSlave.deviceList[0].onlineFlag = false;
                         app485MasterSlave.msRegStatsus = STATUS_CHECK_CONFLICT;
                    }
                    else
                    {
                        if(updataNumber>=(deviceNum-1))
                        {
                            updataNumber = 1;
                            if(updateResultFlag == false)
                            {
                              //  app485MasterSlave.deviceList[0].onlineFlag = false;
                                pbc_reload_timerClock(&send_delay,0);//立即发送
                                deviceChangeFlag = true;
                               // app485MasterSlave.msRegStatsus = STATUS_CHECK_CONFLICT;
                            }  
                            else
                            {
                                app485MasterSlave.msRegStatsus = STATUS_IDLE;//释放
                            }
                        }
                        else
                        {
                            updataFlag = true;
                            app485MasterSlave.msRegStatsus = STATUS_SEND_UPDATA_CHECK;
                            pbc_reload_timerClock(&send_wait_delay,40);//延时40ms
                        //    updataNumber++;
                        }
                    }
                }
            } 
            
            break;
        }    
        default:
        {
            break;
        }          
    }
}
void app_master_slave_syn_task(void)
{
    ms_link_comm_data_def  pReceiveData;
    uint8_t regeditNum = 0;
    uint8_t i; 
    static uint8_t updataNumber = 0;
    pbc_timerClockRun_task(&send_updata_delay);
    switch (app485MasterSlave.msSynStatsus)
    {
        case STATUS_IDLE:
        {
            break;
        }
        case STATUS_CHECK_CONFLICT:
        {
            if(msRunS_idle == mde_msRtu_master_slave_run_status(MASTER_SLAVE_TEST))
            {
                mde_master_slave_link_message_check_conflict(MASTER_SLAVE_TEST,PROTOL_CHECK_CONFICT);
                app485MasterSlave.msSynStatsus = STATUS_CONFLICT_RESULT; 
            }
            break;
        }
        case STATUS_CONFLICT_RESULT:
        {             
            if(msRunS_transmit_conflict == mde_msRtu_master_slave_run_status(MASTER_SLAVE_TEST))
            {
                pbc_reload_timerClock(&send_delay,GetRandomDelayTime());
                app485MasterSlave.msSynStatsus = STATUS_IDLE; 
            } 
            else if(msRunS_idle == mde_msRtu_master_slave_run_status(MASTER_SLAVE_TEST))
            {
                if(app485MasterSlave.updataWord)
                {
                    app485MasterSlave.msSynStatsus = STATUS_SEND_UPDATA_LIST;
                    pbc_reload_timerClock(&send_updata_delay,40);//延时40ms
                }
                else if(app485MasterSlave.occpyWord)
                {
                    app485MasterSlave.msSynStatsus = STATUS_OCCPYSEND;
                    pbc_reload_timerClock(&send_updata_delay,40);//延时40ms
                }
                else
                {
                    app485MasterSlave.msSynStatsus = STATUS_TIMINGSEND;
                    pbc_reload_timerClock(&send_updata_delay,40);//延时40ms
                }                              
            }            
            break;
        }
        case STATUS_OCCPYSEND:
        {  
            if(pbc_pull_timerIsCompleted(&send_updata_delay))
            {
                if(mde_msRtu_master_slave_phy_idle_status(MASTER_SLAVE_TEST))
                {
                    if(app485MasterSlave.occpyWord & SET_POWER)
                    {
                        bool CCOnlineFlag = false; 
                        if((DEVICE_TYPE == 0x8521) || (DEVICE_TYPE == 0x9522) || (DEVICE_TYPE == 0x9521))
                        {//环控机/房间温控器/风盘
                            regeditNum = app_master_slave_pull_device_list_num();                            
                            for(i = 0;i < regeditNum;i++)
                            {//找到输配中心结束
                                if(app485MasterSlave.deviceList[i].deviceType == DEVICE_TYPE_CC)
                                {
                                    app_master_slave_send_one_block(&app485MasterSlave.deviceList[i].DeviceID[0],MRegaddr_NewairStatusW,1,true);
                                    app485MasterSlave.msSynStatsus = STATUS_OCCPYRECEIVE;
                                    CCOnlineFlag = true;
                                    break;
                                }
                            }
                            if(CCOnlineFlag == false)
                            {
                                app485MasterSlave.msSynStatsus = STATUS_IDLE;
                                app485MasterSlave.occpyWord &= ~SET_POWER;
                            }
                        }
                        else if(DEVICE_TYPE == 0x8522)
                        {//输配中心
                            if(app485MasterSlave.updataWord)
                            {                                
                            }
                            else
                            {
                                regeditNum = app_master_slave_pull_device_list_num();
                                for(i = 0;i < regeditNum;i++)
                                {//形成同步列表
                                    if((app485MasterSlave.deviceList[i].deviceType == DEVICE_TYPE_FAN)||
                                    (app485MasterSlave.deviceList[i].deviceType == DEVICE_TYPE_HC)||
                                    (app485MasterSlave.deviceList[i].deviceType == DEVICE_TYPE_ROMM))
                                    {
                                        app485MasterSlave.updataWord |= (0x01<<i);
                                    // app_master_slave_send_one_block(&app485MasterSlave.deviceList[i].DeviceID[0],MRegaddr_NewairStatusW,1,true);
                                    }
                                }
                            }                            
                            app485MasterSlave.msSynStatsus = STATUS_SEND_UPDATA_LIST;
                        }
                    }
                }
            }
            
            break;
        } 
        case STATUS_OCCPYRECEIVE:
        {
            sdt_bool ReceiveOneMessageFlag;
            pReceiveData = mde_pull_msRtu_onemessage(MASTER_SLAVE_TEST,&ReceiveOneMessageFlag);
            if(ReceiveOneMessageFlag)
            {//这一帧数据符合要求
                app485MasterSlave.occpyWord &= ~SET_POWER;
                if(pReceiveData.ProcotolType == PROTOL_CHECK_CONFICT) 
                {
                    pbc_reload_timerClock(&send_delay,GetRandomDelayTime());
                }
                else if(pReceiveData.ProcotolType == PROTOL_SYN) 
                {//处理同步数据
                    app_master_slave_dispose_syn_data(&pReceiveData);
                    app485MasterSlave.msSynStatsus = STATUS_IDLE;
                }       
            }
            else if((mRtuS_master_slave_timeout | mRtuS_master_slave_poterr) & mde_msRtu_master_slave_reveive_status(MASTER_SLAVE_TEST))
            {
                app485MasterSlave.occpyWord &= ~SET_POWER;
                app485MasterSlave.msSynStatsus = STATUS_IDLE;
            } 
            else
            {
                if(msRunS_transmit_conflict == mde_msRtu_master_slave_run_status(MASTER_SLAVE_TEST))
                {   
                    app485MasterSlave.occpyWord &= ~SET_POWER;
                    pbc_reload_timerClock(&send_delay,GetRandomDelayTime());
                    app485MasterSlave.msSynStatsus = STATUS_IDLE;
                }
            }          
            break;
        }  
        case STATUS_SEND_UPDATA_LIST:
        {  
            regeditNum = app_master_slave_pull_device_list_num();
            for(i = 0;i < regeditNum;i++)
            {
                if(app485MasterSlave.updataWord & (0x01 << i))
                {
                    app_master_slave_send_one_block(&app485MasterSlave.deviceList[i].DeviceID[0],MRegaddr_NewairStatusW,1,true);
                    app485MasterSlave.msSynStatsus = STATUS_RECEIVE_UPDATA_LIST;
                    updataNumber = i;
                    break;
                }
            }
            if(app485MasterSlave.msSynStatsus != STATUS_RECEIVE_UPDATA_LIST)
            {//更新列表完成
                app485MasterSlave.occpyWord &= ~SET_POWER;
                app485MasterSlave.msSynStatsus = STATUS_IDLE;
            }
            break;
        } 
        case STATUS_RECEIVE_UPDATA_LIST:
        {
            sdt_bool ReceiveOneMessageFlag;
            pReceiveData = mde_pull_msRtu_onemessage(MASTER_SLAVE_TEST,&ReceiveOneMessageFlag);
            if(ReceiveOneMessageFlag)
            {//这一帧数据符合要求
                app485MasterSlave.msSynStatsus  = STATUS_CHECK_CONFLICT;
                app485MasterSlave.updataWord &= (~(0x01<< updataNumber));
                if(app485MasterSlave.updataWord == 0)
                {
                    app485MasterSlave.occpyWord &= ~SET_POWER;
                    app485MasterSlave.msSynStatsus = STATUS_IDLE;
                }
            }
            else if((mRtuS_master_slave_timeout | mRtuS_master_slave_poterr) & mde_msRtu_master_slave_reveive_status(MASTER_SLAVE_TEST))
            {//超时或帧错误
                app485MasterSlave.msSynStatsus  = STATUS_CHECK_CONFLICT;
                app485MasterSlave.updataWord &= (~(0x01<< updataNumber));
                if(app485MasterSlave.updataWord == 0)
                {
                    app485MasterSlave.occpyWord &= ~SET_POWER;
                    app485MasterSlave.msSynStatsus = STATUS_IDLE;
                }
            } 
            else
            { //冲突     
                if(msRunS_transmit_conflict == mde_msRtu_master_slave_run_status(MASTER_SLAVE_TEST))
                {   
                    app485MasterSlave.msSynStatsus  = STATUS_CHECK_CONFLICT;
                    app485MasterSlave.updataWord &= (~(0x01<< updataNumber));
                    if(app485MasterSlave.updataWord == 0)
                    {
                        app485MasterSlave.occpyWord &= ~SET_POWER;
                        app485MasterSlave.msSynStatsus = STATUS_IDLE;
                    }
                }
            } 
            break;
        }    
        case STATUS_TIMINGSEND:
        {  
            app485MasterSlave.msSynStatsus = STATUS_IDLE;//空闲状态
            break;
        } 
        case STATUS_TIMINGRECEIVE:
        {
            sdt_bool ReceiveOneMessageFlag;
            pReceiveData = mde_pull_msRtu_onemessage(MASTER_SLAVE_TEST,&ReceiveOneMessageFlag);
            if(ReceiveOneMessageFlag)
            {//这一帧数据符合要求
                    if(pReceiveData.ProcotolType == PROTOL_CHECK_CONFICT) 
                    {
                        pbc_reload_timerClock(&send_delay,GetRandomDelayTime());
                    }
                    else if(pReceiveData.ProcotolType == PROTOL_SYN) 
                    {
                        //app_master_slave_dispose_receive_data(&pReceiveData);
                        app485MasterSlave.msSynStatsus = STATUS_IDLE;
                        //响应数据
                    }       
            }
            else if((mRtuS_master_slave_timeout | mRtuS_master_slave_poterr) & mde_msRtu_master_slave_reveive_status(MASTER_SLAVE_TEST))
            {
                app485MasterSlave.msSynStatsus = STATUS_IDLE;
            } 
            else
            {
                if(msRunS_transmit_conflict == mde_msRtu_master_slave_run_status(MASTER_SLAVE_TEST))
                {   
                    pbc_reload_timerClock(&send_delay,GetRandomDelayTime());
                    app485MasterSlave.msSynStatsus = STATUS_IDLE;
                }
            }          
            break;
        }    
        default:
        {
            break;
        }         
    }
}

void app_master_slave_test_task(void)
{
    sdt_bool ReceiveOneMessageFlag;
    ms_link_comm_data_def  pReceiveData;
    static sdt_bool cfged = sdt_false;
    if(cfged)
    {      
        mde_rtu_master_slave_task();
        app_master_slave_syn_task();
        app_master_slave_regedit_task();     
        pReceiveData = mde_pull_msRtu_onemessage(MASTER_SLAVE_TEST,&ReceiveOneMessageFlag);
        if(ReceiveOneMessageFlag)
        {//收到一帧数据
            if(pReceiveData.ProcotolType == PROTOL_CHECK_CONFICT) 
            {//冲突检测帧
                pbc_reload_timerClock(&send_delay,GetRandomDelayTime());
            }
            else if(pReceiveData.ProcotolType == PROTOL_SYN) 
            {//同步帧
                app_master_slave_dispose_syn_data(&pReceiveData);
            }   
            else if(pReceiveData.ProcotolType == PROTOL_REGEDIT) 
            {//注册帧
                app_master_slave_dispose_regedit_data(&pReceiveData);
            }                             
        }
        if(msRunS_transmit_conflict == mde_msRtu_master_slave_run_status(MASTER_SLAVE_TEST))
        {//有冲突延时发送
            pbc_reload_timerClock(&send_delay,GetRandomDelayTime());
        }         
    }
    else
    {
        cfged = sdt_true;
        mde_msRtu_master_slave_push_mRtu_local_id(MASTER_SLAVE_TEST,local_addr);
        pbc_reload_timerClock(&send_delay,GetRandomDelayTime());
        app485MasterSlave.msRegStatsus = STATUS_CHECK_CONFLICT;
    }  
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
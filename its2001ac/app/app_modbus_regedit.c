//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include ".\app_cfg.h"
#include "stdlib.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define MAX_DEVICE_NUM    16
#define ID_NUM            6
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef enum
{
    STATUS_IDLE                    = 0x00,   //空闲状态
   // STATUS_SEND_WAIT,	                 //发送等待
    STATUS_CHECK_CONFLICT,               //冲突侦测
    STATUS_CONFLICT_RESULT, 
    STATUS_SEND_REG_DATA,               //注册数据
    STATUS_RECEIVE_REG_DATA,
    STATUS_SEND_UPDATA_LIST,            //更新列表
    STATUS_RECEIVE_UPDATA_LIST,             
	STATUS_RESEND,                      //重发
	STATUS_RERECEIVE,  
    STATUS_DISPOSE_DATA,
}msStatus_def;

typedef struct
{
    bool     onlineFlag;       //在线标志
    uint16_t deviceType;       //设备类型
    uint8_t  DeviceID[ID_NUM];      //ID
}msDeviceList_t;

typedef struct
{
    msStatus_def    msRegStatsus;       //注册状态
    timerClock_def  send_long_wait_delay;//发送长等待
    timerClock_def  send_wait_delay;    //主动发送等待
    timerClock_def  updata_list_delay;  //更新列表
    timerClock_def  online_delay;       //在綫時長
    bool            deviceChangeFlag;  //设备改变标志
    bool            updateFlag;        //更新标志
    uint16_t        updataWord;        //更新字
    msDeviceList_t  deviceList[MAX_DEVICE_NUM];  //设备列表
}msComm_t;

msComm_t appModbusRegedit[MAX_MODBUS_NUM];

sdt_int8u local_addr[6]={0xaE,0x20,0x11,0x25,0x01,0x01};               
sdt_int8u dest_addr[6]={0xab,0x20,0x10,0x23,0x00,0x01};
sdt_int8u board_addr[6]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
/*bool updataFlag = false;
macro_createTimer(send_timing_delay,timerType_second,0);
macro_createTimer(send_delay,timerType_millisecond,0);
macro_createTimer(send_boardcast_delay,timerType_second,0);
macro_createTimer(second_rev_delay,timerType_second,0);
macro_createTimer(send_updata_delay,timerType_millisecond,0);*/

/*在线数量*/
uint8_t app_master_slave_pull_device_list_num(uint8_t in_solidNum)
{
    uint8_t i = 0;
    uint8_t onLineNum =0;
    for(i = 0;i < MAX_DEVICE_NUM;i++)
    {
        if(appModbusRegedit[in_solidNum].deviceList[i].onlineFlag == true)
        {
            onLineNum++;
        }
    }
    return onLineNum;
}
/*发送注册数据*/
void app_master_slave_send_regedit_data(uint8_t in_solidNum,uint8_t *in_dest_addr,uint8_t control_cmd,bool responseFlag)
{
    ms_link_comm_data_def tempLinkData;
    uint8_t regeditNum = 0;

    mde_master_slave_link_message_transmit(in_solidNum,in_dest_addr,PROTOL_REGEDIT,responseFlag); 
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
        regeditNum = app_master_slave_pull_device_list_num(in_solidNum);
        tempLinkData.Payload[3] = regeditNum;
        uint8_t i = 0;
        for(i = 0;i < regeditNum;i++)
        {
            tempLinkData.Payload[4+8*i] =  (uint8_t)(appModbusRegedit[in_solidNum].deviceList[i].deviceType>>8); 
            tempLinkData.Payload[5+8*i] =  (uint8_t)(appModbusRegedit[in_solidNum].deviceList[i].deviceType);
            tempLinkData.Payload[6+8*i] =  (uint8_t)(appModbusRegedit[in_solidNum].deviceList[i].DeviceID[0]);
            tempLinkData.Payload[7+8*i] =  (uint8_t)(appModbusRegedit[in_solidNum].deviceList[i].DeviceID[1]);
            tempLinkData.Payload[8+8*i] =  (uint8_t)(appModbusRegedit[in_solidNum].deviceList[i].DeviceID[2]);
            tempLinkData.Payload[9+8*i] =  (uint8_t)(appModbusRegedit[in_solidNum].deviceList[i].DeviceID[3]);
            tempLinkData.Payload[10+8*i] =  (uint8_t)(appModbusRegedit[in_solidNum].deviceList[i].DeviceID[4]);
            tempLinkData.Payload[11+8*i] =  (uint8_t)(appModbusRegedit[in_solidNum].deviceList[i].DeviceID[5]);
        }
        tempLinkData.PayloadLength = (4+tempLinkData.Payload[3]*8);
    }
    tempLinkData.responseFlag = false;
    mde_master_slave_push_link_onemessage(in_solidNum,&tempLinkData);
}
/*注册响应*/
void app_master_slave_send_regedit_response_data(uint8_t in_solidNum,uint8_t *in_dest_addr,uint8_t control_cmd)
{
    ms_link_comm_data_def tempLinkData;
    mde_master_slave_link_message_transmit(in_solidNum,in_dest_addr,PROTOL_REGEDIT,false); 
    tempLinkData.Payload[0] = PROTOL_VERSION;
    tempLinkData.Payload[1] = control_cmd|0x80;
    tempLinkData.PayloadLength = 2;
    tempLinkData.responseFlag = true;
    mde_master_slave_push_link_onemessage(in_solidNum,&tempLinkData);
}


uint16_t GetRandomDelayTime(void)
{//20-3020ms的随机数
    uint16_t result = 0;
    result = (rand()&0x0190)*5+ 300;/* random factor 0-47*/      
    return  result;
}

bool app_master_slave_pull_device_list(uint8_t in_solidNum,uint8_t in_port,uint16_t* out_deviceType)
{
    *out_deviceType = appModbusRegedit[in_solidNum].deviceList[in_port].deviceType;
    if(appModbusRegedit[in_solidNum].deviceList[in_port].onlineFlag)
    {
        return true;
    }
    return false;
}
uint16_t pull_device_type(uint8_t in_solidNum,uint8_t in_port)
{
    return (appModbusRegedit[in_solidNum].deviceList[in_port].deviceType);
}
uint8_t *pull_device_id(uint8_t in_solidNum,uint8_t in_port)
{
    return (&appModbusRegedit[in_solidNum].deviceList[in_port].DeviceID[0]);
}
bool pull_local_device_online(uint8_t in_solidNum)
{
    uint8_t i = 0;
    for(i = 0; i < MAX_DEVICE_NUM;i++)
    {
        if(appModbusRegedit[in_solidNum].deviceList[i].deviceType == DEVICE_TYPE_HC)
        {
            if(appModbusRegedit[in_solidNum].deviceList[i].onlineFlag)
            {
                return true;
            }
        }
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

bool pull_first_device_id(uint8_t in_solidNum,uint8_t *in_deviceId)
{
   if((in_deviceId[0] ==appModbusRegedit[in_solidNum].deviceList[0].DeviceID[0])&&\
    (in_deviceId[1] == appModbusRegedit[in_solidNum].deviceList[0].DeviceID[1])&&\
    (in_deviceId[2] == appModbusRegedit[in_solidNum].deviceList[0].DeviceID[2])&&\
    (in_deviceId[3] == appModbusRegedit[in_solidNum].deviceList[0].DeviceID[3])&&\
    (in_deviceId[4] == appModbusRegedit[in_solidNum].deviceList[0].DeviceID[4])&&\
    (in_deviceId[5] == appModbusRegedit[in_solidNum].deviceList[0].DeviceID[5]))
    {
        return true;
    }
    return false;
}

bool pull_second_device_id(uint8_t in_solidNum,uint8_t *in_deviceId)
{
   if((in_deviceId[0] == appModbusRegedit[in_solidNum].deviceList[1].DeviceID[0])&&\
    (in_deviceId[1] == appModbusRegedit[in_solidNum].deviceList[1].DeviceID[1])&&\
    (in_deviceId[2] == appModbusRegedit[in_solidNum].deviceList[1].DeviceID[2])&&\
    (in_deviceId[3] == appModbusRegedit[in_solidNum].deviceList[1].DeviceID[3])&&\
    (in_deviceId[4] == appModbusRegedit[in_solidNum].deviceList[1].DeviceID[4])&&\
    (in_deviceId[5] == appModbusRegedit[in_solidNum].deviceList[1].DeviceID[5]))
    {
        return true;
    }
    return false;
}

void app_master_slave_updata_second_device_list(uint8_t in_solidNum)
{
    uint8_t deviceNum = 0;
    if(appModbusRegedit[in_solidNum].deviceList[0].onlineFlag )
    {
        deviceNum = app_master_slave_pull_device_list_num(in_solidNum);
        if(deviceNum == 1)
        {
            appModbusRegedit[in_solidNum].deviceList[0].onlineFlag = false;//踢掉0号设备
        }
        else if(deviceNum > 1)
        {
            uint8_t i;
            for(i = 1;i < deviceNum;i++)
            {
                appModbusRegedit[in_solidNum].deviceList[i-1].onlineFlag = appModbusRegedit[in_solidNum].deviceList[i].onlineFlag;
                appModbusRegedit[in_solidNum].deviceList[i-1].deviceType =  appModbusRegedit[in_solidNum].deviceList[i].deviceType;
                appModbusRegedit[in_solidNum].deviceList[i-1].DeviceID[0] = appModbusRegedit[in_solidNum].deviceList[i].DeviceID[0];
                appModbusRegedit[in_solidNum].deviceList[i-1].DeviceID[1] = appModbusRegedit[in_solidNum].deviceList[i].DeviceID[1];
                appModbusRegedit[in_solidNum].deviceList[i-1].DeviceID[2] = appModbusRegedit[in_solidNum].deviceList[i].DeviceID[2];
                appModbusRegedit[in_solidNum].deviceList[i-1].DeviceID[3] = appModbusRegedit[in_solidNum].deviceList[i].DeviceID[3];
                appModbusRegedit[in_solidNum].deviceList[i-1].DeviceID[4] = appModbusRegedit[in_solidNum].deviceList[i].DeviceID[4];
                appModbusRegedit[in_solidNum].deviceList[i-1].DeviceID[5] = appModbusRegedit[in_solidNum].deviceList[i].DeviceID[5];
                appModbusRegedit[in_solidNum].deviceList[i].onlineFlag = false;
            }
        }       
    }  
}

void app_master_slave_updata_local_device_list(uint8_t in_solidNum)
{
    uint8_t deviceNum = 0;
    uint16_t updataNum = 0;
    uint8_t i,j;
    deviceNum = app_master_slave_pull_device_list_num(in_solidNum);
    if(deviceNum == 1)
    {
        appModbusRegedit[in_solidNum].deviceList[0].onlineFlag = false;
       // appModbusRegedit[in_solidNum].msRegStatsus = STATUS_CHECK_CONFLICT;
    }
    else
    {
        appModbusRegedit[in_solidNum].deviceChangeFlag = true;//发送列表
        for(i = 0;i < 16;i++)
        {
            if(appModbusRegedit[in_solidNum].deviceList[i].onlineFlag)
            {
                updataNum |= (0x01<<i);
            }       
        }
        for(i = 0; i < deviceNum;i++)
        {
            for(j = 0; j < 16;j++)
            {
                if(updataNum & (0x01<<j))
                {
                    updataNum &= (~(0x01<<j));
                    appModbusRegedit[in_solidNum].deviceList[i].onlineFlag = appModbusRegedit[in_solidNum].deviceList[j].onlineFlag;
                    appModbusRegedit[in_solidNum].deviceList[i].deviceType =  appModbusRegedit[in_solidNum].deviceList[j].deviceType;
                    appModbusRegedit[in_solidNum].deviceList[i].DeviceID[0] = appModbusRegedit[in_solidNum].deviceList[j].DeviceID[0];
                    appModbusRegedit[in_solidNum].deviceList[i].DeviceID[1] = appModbusRegedit[in_solidNum].deviceList[j].DeviceID[1];
                    appModbusRegedit[in_solidNum].deviceList[i].DeviceID[2] = appModbusRegedit[in_solidNum].deviceList[j].DeviceID[2];
                    appModbusRegedit[in_solidNum].deviceList[i].DeviceID[3] = appModbusRegedit[in_solidNum].deviceList[j].DeviceID[3];
                    appModbusRegedit[in_solidNum].deviceList[i].DeviceID[4] = appModbusRegedit[in_solidNum].deviceList[j].DeviceID[4];
                    appModbusRegedit[in_solidNum].deviceList[i].DeviceID[5] = appModbusRegedit[in_solidNum].deviceList[j].DeviceID[5];
                    break;
                }
            }
        }
    }
}

void app_master_slave_dispose_regedit_data(uint8_t in_solidNum,ms_link_comm_data_def *in_rev_data)
{
    uint8_t i = 0;
    bool newIdFlag = true;
    uint8_t deviceNum = 0;
    deviceNum = app_master_slave_pull_device_list_num(in_solidNum);
    appModbusRegedit[in_solidNum].msRegStatsus = STATUS_DISPOSE_DATA;
    if(pull_boardcast_id(&in_rev_data->LinkDstAddr[0]))
    {//广播地址    
        if(deviceNum == 0)
        {//接收到广播地址同时设备列表里为0则记录一下源ID
            appModbusRegedit[in_solidNum].deviceList[0].onlineFlag = true;
            appModbusRegedit[in_solidNum].deviceList[0].DeviceID[0] = in_rev_data->LinkSrcAddr[0];
            appModbusRegedit[in_solidNum].deviceList[0].DeviceID[1] = in_rev_data->LinkSrcAddr[1];
            appModbusRegedit[in_solidNum].deviceList[0].DeviceID[2] = in_rev_data->LinkSrcAddr[2];
            appModbusRegedit[in_solidNum].deviceList[0].DeviceID[3] = in_rev_data->LinkSrcAddr[3];
            appModbusRegedit[in_solidNum].deviceList[0].DeviceID[4] = in_rev_data->LinkSrcAddr[4];
            appModbusRegedit[in_solidNum].deviceList[0].DeviceID[5] = in_rev_data->LinkSrcAddr[5];
            pbc_reload_timerClock(&appModbusRegedit[in_solidNum].send_wait_delay,0);//立即发送
            pbc_reload_timerClock(&appModbusRegedit[in_solidNum].send_long_wait_delay,0);//立即发送
            pbc_reload_timerClock(&appModbusRegedit[in_solidNum].online_delay,190);
        }
        else if(pull_first_device_id(in_solidNum,&in_rev_data->LinkSrcAddr[0]))
        {//收到1号设备的广播帧
            if(deviceNum == 1)
            {
                appModbusRegedit[in_solidNum].msRegStatsus = STATUS_IDLE;
                pbc_reload_timerClock(&appModbusRegedit[in_solidNum].send_wait_delay,0);//立即发送
                pbc_reload_timerClock(&appModbusRegedit[in_solidNum].send_long_wait_delay,0);//立即发送
            }
            else
            {
                if(pull_second_device_id(in_solidNum,&local_addr[0]))
                {//2号设备收到则直接更新列表
                    appModbusRegedit[in_solidNum].deviceChangeFlag = true;//发送列表
                    app_master_slave_updata_second_device_list(in_solidNum);//更新列表
                }  
            }
        }
        else if(pull_first_device_id(in_solidNum,&local_addr[0]))
        {//1号设备 后上电设备加入列表
            appModbusRegedit[in_solidNum].deviceChangeFlag = true;//发送列表
            for(i = 1;i < MAX_DEVICE_NUM;i++)
            {       
                if(appModbusRegedit[in_solidNum].deviceList[i].onlineFlag == true)
                {
                     if((in_rev_data->LinkSrcAddr[0] ==  appModbusRegedit[in_solidNum].deviceList[i].DeviceID[0])&&\
                        (in_rev_data->LinkSrcAddr[1] ==  appModbusRegedit[in_solidNum].deviceList[i].DeviceID[1])&&\
                        (in_rev_data->LinkSrcAddr[2] ==  appModbusRegedit[in_solidNum].deviceList[i].DeviceID[2])&&\
                        (in_rev_data->LinkSrcAddr[3] ==  appModbusRegedit[in_solidNum].deviceList[i].DeviceID[3])&&\
                        (in_rev_data->LinkSrcAddr[4] ==  appModbusRegedit[in_solidNum].deviceList[i].DeviceID[4])&&\
                        (in_rev_data->LinkSrcAddr[5] ==  appModbusRegedit[in_solidNum].deviceList[i].DeviceID[5]))
                    {
                        newIdFlag = false;
                    }
                }
            }
            if(newIdFlag)
            {
                for(i = 1;i < MAX_DEVICE_NUM;i++)
                {
                    if(appModbusRegedit[in_solidNum].deviceList[i].onlineFlag == false)
                    {
                        appModbusRegedit[in_solidNum].deviceList[i].onlineFlag = true;
                        appModbusRegedit[in_solidNum].deviceList[i].deviceType = pbc_arrayToInt16u_bigEndian(&in_rev_data->Payload[3]);
                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[0] = in_rev_data->LinkSrcAddr[0];
                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[1] = in_rev_data->LinkSrcAddr[1];
                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[2] = in_rev_data->LinkSrcAddr[2];
                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[3] = in_rev_data->LinkSrcAddr[3];
                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[4] = in_rev_data->LinkSrcAddr[4];
                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[5] = in_rev_data->LinkSrcAddr[5];
                        break;
                    }
                }
            }
        }
    }
    else if(pull_local_id(&in_rev_data->LinkDstAddr[0]))       
    {//自己地址
        uint8_t regeditNum = in_rev_data->Payload[3];
        if(pull_first_device_id(in_solidNum,&in_rev_data->LinkSrcAddr[0]))
        {//1号设备发过来的
             pbc_reload_timerClock(&appModbusRegedit[in_solidNum].online_delay,190);
        }
        if(in_rev_data->Payload[1] == CONTROL_UPDATA)
        {//更新应答
            for(i = 0;i < regeditNum;i++)
            {
                appModbusRegedit[in_solidNum].deviceList[i].onlineFlag = true;
                appModbusRegedit[in_solidNum].deviceList[i].deviceType = pbc_arrayToInt16u_bigEndian(&in_rev_data->Payload[4+i*8]);
                appModbusRegedit[in_solidNum].deviceList[i].DeviceID[0] = in_rev_data->Payload[6+i*8];
                appModbusRegedit[in_solidNum].deviceList[i].DeviceID[1] = in_rev_data->Payload[7+i*8];
                appModbusRegedit[in_solidNum].deviceList[i].DeviceID[2] = in_rev_data->Payload[8+i*8];
                appModbusRegedit[in_solidNum].deviceList[i].DeviceID[3] = in_rev_data->Payload[9+i*8];
                appModbusRegedit[in_solidNum].deviceList[i].DeviceID[4] = in_rev_data->Payload[10+i*8];
                appModbusRegedit[in_solidNum].deviceList[i].DeviceID[5] = in_rev_data->Payload[11+i*8];
            }
            for(i = regeditNum;i < MAX_DEVICE_NUM;i++)
            {
                appModbusRegedit[in_solidNum].deviceList[i].onlineFlag = false;
            }
            pbc_reload_timerClock(&appModbusRegedit[in_solidNum].online_delay,190);//先响应再更新
            app_master_slave_send_regedit_response_data(in_solidNum,&in_rev_data->LinkSrcAddr[0],in_rev_data->Payload[1]);
            appModbusRegedit[in_solidNum].send_wait_delay.timStatusBits |= timStatusBits_onceTriggered;
            appModbusRegedit[in_solidNum].send_long_wait_delay.timStatusBits |= timStatusBits_onceTriggered;
        }
        else if(in_rev_data->Payload[1] == CONTROL_REGEDIT)
        {//注册应答
            app_master_slave_send_regedit_response_data(in_solidNum,&in_rev_data->LinkSrcAddr[0],in_rev_data->Payload[1]);
            if(appModbusRegedit[in_solidNum].deviceList[0].onlineFlag == false)
            {
                appModbusRegedit[in_solidNum].deviceList[0].onlineFlag = true;
                appModbusRegedit[in_solidNum].deviceList[0].deviceType = DEVICE_TYPE;
                appModbusRegedit[in_solidNum].deviceList[0].DeviceID[0] = in_rev_data->LinkDstAddr[0];
                appModbusRegedit[in_solidNum].deviceList[0].DeviceID[1] = in_rev_data->LinkDstAddr[1];
                appModbusRegedit[in_solidNum].deviceList[0].DeviceID[2] = in_rev_data->LinkDstAddr[2];
                appModbusRegedit[in_solidNum].deviceList[0].DeviceID[3] = in_rev_data->LinkDstAddr[3];
                appModbusRegedit[in_solidNum].deviceList[0].DeviceID[4] = in_rev_data->LinkDstAddr[4];
                appModbusRegedit[in_solidNum].deviceList[0].DeviceID[5] = in_rev_data->LinkDstAddr[5];
            }
            
            for(i = 1;i < MAX_DEVICE_NUM;i++)
            {       
                if(appModbusRegedit[in_solidNum].deviceList[i].onlineFlag == true)
                {
                     if((in_rev_data->LinkSrcAddr[0] ==  appModbusRegedit[in_solidNum].deviceList[i].DeviceID[0])&&\
                        (in_rev_data->LinkSrcAddr[1] ==  appModbusRegedit[in_solidNum].deviceList[i].DeviceID[1])&&\
                        (in_rev_data->LinkSrcAddr[2] ==  appModbusRegedit[in_solidNum].deviceList[i].DeviceID[2])&&\
                        (in_rev_data->LinkSrcAddr[3] ==  appModbusRegedit[in_solidNum].deviceList[i].DeviceID[3])&&\
                        (in_rev_data->LinkSrcAddr[4] ==  appModbusRegedit[in_solidNum].deviceList[i].DeviceID[4])&&\
                        (in_rev_data->LinkSrcAddr[5] ==  appModbusRegedit[in_solidNum].deviceList[i].DeviceID[5]))
                    {
                        newIdFlag = false;
                    }
                }
            }
            if(newIdFlag)
            {
                for(i = 1;i < MAX_DEVICE_NUM;i++)
                {
                    if(appModbusRegedit[in_solidNum].deviceList[i].onlineFlag == false)
                    {
                        appModbusRegedit[in_solidNum].deviceList[i].onlineFlag = true;
                        pbc_reload_timerClock(&appModbusRegedit[in_solidNum].online_delay,190);
                        appModbusRegedit[in_solidNum].deviceList[i].deviceType = pbc_arrayToInt16u_bigEndian(&in_rev_data->Payload[3]);
                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[0] = in_rev_data->LinkSrcAddr[0];
                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[1] = in_rev_data->LinkSrcAddr[1];
                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[2] = in_rev_data->LinkSrcAddr[2];
                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[3] = in_rev_data->LinkSrcAddr[3];
                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[4] = in_rev_data->LinkSrcAddr[4];
                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[5] = in_rev_data->LinkSrcAddr[5];
                        appModbusRegedit[in_solidNum].deviceChangeFlag = true;//发送列表
                     //   pbc_reload_timerClock(&appModbusRegedit[in_solidNum].send_long_wait_delay,0);//立即发送
                        break;
                    }
                }
            }
        }
    }
}




bool pull_first_device(uint8_t in_solidNum)
{
     if(((appModbusRegedit[in_solidNum].deviceList[0].DeviceID[0] == local_addr[0])&&\
    (appModbusRegedit[in_solidNum].deviceList[0].DeviceID[1] == local_addr[1])&&\
    (appModbusRegedit[in_solidNum].deviceList[0].DeviceID[2] == local_addr[2])&&\
    (appModbusRegedit[in_solidNum].deviceList[0].DeviceID[3] == local_addr[3])&&\
    (appModbusRegedit[in_solidNum].deviceList[0].DeviceID[4] == local_addr[4])&&\
    (appModbusRegedit[in_solidNum].deviceList[0].DeviceID[5] == local_addr[5])))
    {
        return true;
    }
    return false;
}

void app_master_slave_regedit_timing_task(uint8_t in_solidNum)
{
    if(pull_first_device_id(in_solidNum,&local_addr[0]))
    {//1号设备
        if((pbc_pull_timerIsCompleted(&appModbusRegedit[in_solidNum].updata_list_delay)) || (appModbusRegedit[in_solidNum].deviceChangeFlag))
        {
            if(appModbusRegedit[in_solidNum].msRegStatsus == STATUS_IDLE)
            {
                pbc_reload_timerClock(&appModbusRegedit[in_solidNum].updata_list_delay,180);
                appModbusRegedit[in_solidNum].deviceChangeFlag = false;
                appModbusRegedit[in_solidNum].updateFlag = true;
                pbc_reload_timerClock(&appModbusRegedit[in_solidNum].send_wait_delay,0);//立即发送
            }       
        }
    }
    else if(pull_second_device_id(in_solidNum,&local_addr[0]))
    {//2号设备
        if((pbc_pull_timerIsCompleted(&appModbusRegedit[in_solidNum].online_delay)))
        {//3分钟未收到1号设备数据
            if(appModbusRegedit[in_solidNum].msRegStatsus == STATUS_IDLE)
            {
                pbc_reload_timerClock(&appModbusRegedit[in_solidNum].online_delay,190);
                app_master_slave_updata_second_device_list(in_solidNum);//更新列表
                appModbusRegedit[in_solidNum].deviceChangeFlag = true;//发送列表
                pbc_reload_timerClock(&appModbusRegedit[in_solidNum].send_wait_delay,0);//立即发送
            }
        }
    }
    else
    {//其它设备掉线
        uint8_t i = 0;
        uint8_t regeditNum = 0;
        if((pbc_pull_timerIsCompleted(&appModbusRegedit[in_solidNum].online_delay)))
        {
            regeditNum = app_master_slave_pull_device_list_num(in_solidNum);
            for(i = 0; i < regeditNum;i++)
            {
                appModbusRegedit[in_solidNum].deviceList[i].onlineFlag = false;
            }
            appModbusRegedit[in_solidNum].msRegStatsus = STATUS_IDLE;
            pbc_reload_timerClock(&appModbusRegedit[in_solidNum].send_wait_delay,0);//立即发送
        }
    }
}


void app_master_slave_regedit_task(uint8_t in_solidNum)
{//注册任务
    uint8_t regeditNum = 0;
    uint8_t i = 0;
    static uint8_t updataNumber = 0;
    pbc_timerMillRun_task(&appModbusRegedit[in_solidNum].send_wait_delay);//发送等待
    pbc_timerClockRun_task(&appModbusRegedit[in_solidNum].updata_list_delay);//更新列表
    pbc_timerClockRun_task(&appModbusRegedit[in_solidNum].online_delay);//在线时长

    app_master_slave_regedit_timing_task(in_solidNum);//定时发送注册信息
    switch (appModbusRegedit[in_solidNum].msRegStatsus)
    {
        case STATUS_IDLE:
        {
           if((pbc_pull_timerIsOnceTriggered(&appModbusRegedit[in_solidNum].send_wait_delay))||
           (pbc_pull_timerIsOnceTriggered(&appModbusRegedit[in_solidNum].send_long_wait_delay)))
           {
                srand(appModbusRegedit[in_solidNum].send_wait_delay.keepTick);
                appModbusRegedit[in_solidNum].msRegStatsus = STATUS_CHECK_CONFLICT;
           }
           break;
        }
//        case STATUS_SEND_WAIT:
//        {          
//            break;
//        } 
        case STATUS_CHECK_CONFLICT:
        {//检测冲突
            if(msRunS_idle == mde_msRtu_master_slave_run_status(in_solidNum))
            {//空闲状态下
                mde_master_slave_link_message_check_conflict(in_solidNum,PROTOL_CHECK_CONFICT);
                appModbusRegedit[in_solidNum].msRegStatsus = STATUS_CONFLICT_RESULT; 
            }                           
            break;
        }  
        case STATUS_CONFLICT_RESULT:
        {             
            if(msRunS_conflict_wait == mde_msRtu_master_slave_run_status(in_solidNum))
            {
                pbc_reload_timerClock(&appModbusRegedit[in_solidNum].send_wait_delay,GetRandomDelayTime());
                appModbusRegedit[in_solidNum].msRegStatsus = STATUS_IDLE; 
            } 
            else if(msRunS_idle == mde_msRtu_master_slave_run_status(in_solidNum))
            {//能正常发送抢占帧则表明总线空闲
                if(appModbusRegedit[in_solidNum].updateFlag)
                {//更新标志
                    appModbusRegedit[in_solidNum].msRegStatsus = STATUS_SEND_UPDATA_LIST;//更新列表
                    if(appModbusRegedit[in_solidNum].updataWord)
                    {

                    }
                    else
                    {
                        regeditNum = app_master_slave_pull_device_list_num(in_solidNum);
                        for(i = 1;i < regeditNum;i++)
                        {//形成同步列表
                            appModbusRegedit[in_solidNum].updataWord |= (0x01<<i);
                        }
                    }                   
                }
                else
                {
                    appModbusRegedit[in_solidNum].msRegStatsus = STATUS_SEND_REG_DATA; //注册数据
                }
            }                     
            break;
        }
        case STATUS_SEND_REG_DATA:
        {    
            if(msRunS_idle == mde_msRtu_master_slave_run_status(in_solidNum))
            {//空闲                           
                static uint8_t silenceCount = 0;  
                if(appModbusRegedit[in_solidNum].deviceList[0].onlineFlag == false)
                {//未形成1号设备发送广播帧
                    app_master_slave_send_regedit_data(in_solidNum,&board_addr[0],CONTROL_REGEDIT,false);
                    appModbusRegedit[in_solidNum].msRegStatsus = STATUS_IDLE;
                    if(silenceCount)
                    {
                        if((30+silenceCount) >= 180)
                        {
                            pbc_reload_timerClock(&appModbusRegedit[in_solidNum].send_long_wait_delay,180);//静默180s
                        }
                        else
                        {
                            pbc_reload_timerClock(&appModbusRegedit[in_solidNum].send_long_wait_delay,(30+silenceCount));//静默时间累加
                            silenceCount++;
                        }                   
                    }
                    else
                    {
                        silenceCount++;
                        pbc_reload_timerClock(&appModbusRegedit[in_solidNum].send_wait_delay,(30000+GetRandomDelayTime()));//静默30s
                    }          
                } 
                else
                {//向1号设备发起注册                                   
                    app_master_slave_send_regedit_data(in_solidNum,&appModbusRegedit[in_solidNum].deviceList[0].DeviceID[0],CONTROL_REGEDIT,true);
                    appModbusRegedit[in_solidNum].msRegStatsus = STATUS_RECEIVE_REG_DATA;
                }
            }                
            break;
        } 
        case STATUS_RECEIVE_REG_DATA:
        {       
            if(mRtuS_master_slave_complete == mde_msRtu_master_slave_reveive_status(in_solidNum))
            {//这一帧数据符合要求
                appModbusRegedit[in_solidNum].msRegStatsus = STATUS_IDLE;//释放    
                appModbusRegedit[in_solidNum].send_long_wait_delay.timStatusBits |= timStatusBits_onceTriggered;
            }
            else if((mRtuS_master_slave_poterr == mde_msRtu_master_slave_reveive_status(in_solidNum)) || (mRtuS_master_slave_timeout == mde_msRtu_master_slave_reveive_status(in_solidNum)))
            {//注册失败则重新发起广播帧
                pbc_reload_timerClock(&appModbusRegedit[in_solidNum].send_wait_delay,GetRandomDelayTime());
                appModbusRegedit[in_solidNum].deviceList[0].onlineFlag = false;
                appModbusRegedit[in_solidNum].msRegStatsus = STATUS_IDLE;//延时再发
            } 
            else
            {
                if(msRunS_conflict_wait == mde_msRtu_master_slave_run_status(in_solidNum))
                {   
                    pbc_reload_timerClock(&appModbusRegedit[in_solidNum].send_wait_delay,GetRandomDelayTime());
                    appModbusRegedit[in_solidNum].deviceList[0].onlineFlag = false;
                    appModbusRegedit[in_solidNum].msRegStatsus = STATUS_IDLE;//延时再发
                }
            }          
            break;
        } 
        case STATUS_SEND_UPDATA_LIST:
        {              
            if(msRunS_idle == mde_msRtu_master_slave_run_status(in_solidNum))
            {
                regeditNum = app_master_slave_pull_device_list_num(in_solidNum);
                if(regeditNum == 1)
                {//更新时在线设备只有1个
                    appModbusRegedit[in_solidNum].deviceList[0].onlineFlag = false;
                    appModbusRegedit[in_solidNum].updateFlag = false;
                    pbc_reload_timerClock(&appModbusRegedit[in_solidNum].send_wait_delay,GetRandomDelayTime());
                    appModbusRegedit[in_solidNum].msRegStatsus = STATUS_IDLE;
                }
                else
                {
                    for(i = 0;i < regeditNum;i++)
                    {
                        if(appModbusRegedit[in_solidNum].updataWord & (0x01 << i))
                        {
                            appModbusRegedit[in_solidNum].updataWord &= (~(0x01<< i));
                            updataNumber = i;
                            app_master_slave_send_regedit_data(in_solidNum,&appModbusRegedit[in_solidNum].deviceList[i].DeviceID[0],CONTROL_UPDATA,true);
                            appModbusRegedit[in_solidNum].msRegStatsus = STATUS_RECEIVE_UPDATA_LIST;
                            break;
                        }
                    }
                }
            }           
            break;
        }
        case STATUS_RECEIVE_UPDATA_LIST:
        {
            if(mRtuS_master_slave_complete == mde_msRtu_master_slave_reveive_status(in_solidNum))
            {//这一帧数据符合要求
                  appModbusRegedit[in_solidNum].msRegStatsus = STATUS_IDLE;
                  pbc_reload_timerClock(&appModbusRegedit[in_solidNum].send_wait_delay,100);
                  if(appModbusRegedit[in_solidNum].updataWord == 0)
                  {
                      appModbusRegedit[in_solidNum].updateFlag = false;
                      appModbusRegedit[in_solidNum].send_wait_delay.timStatusBits |= timStatusBits_onceTriggered;
                      appModbusRegedit[in_solidNum].send_long_wait_delay.timStatusBits |= timStatusBits_onceTriggered;
                  }                          
            }
            else if((mRtuS_master_slave_poterr == mde_msRtu_master_slave_reveive_status(in_solidNum)) || (mRtuS_master_slave_timeout == mde_msRtu_master_slave_reveive_status(in_solidNum)))
            {
                appModbusRegedit[in_solidNum].deviceList[updataNumber].onlineFlag = false;
                appModbusRegedit[in_solidNum].msRegStatsus = STATUS_IDLE;
                pbc_reload_timerClock(&appModbusRegedit[in_solidNum].send_wait_delay,100);
                if(appModbusRegedit[in_solidNum].updataWord == 0)
                {//更新列表
                    appModbusRegedit[in_solidNum].msRegStatsus = STATUS_IDLE;
                    appModbusRegedit[in_solidNum].updateFlag = false;
                    app_master_slave_updata_local_device_list(in_solidNum);//更新列表
                }       
            } 
            else
            {               
                if(msRunS_conflict_wait == mde_msRtu_master_slave_run_status(in_solidNum))
                {   
                    appModbusRegedit[in_solidNum].deviceList[updataNumber].onlineFlag = false;
                    appModbusRegedit[in_solidNum].msRegStatsus = STATUS_IDLE;
                    pbc_reload_timerClock(&appModbusRegedit[in_solidNum].send_wait_delay,100);
                    if(appModbusRegedit[in_solidNum].updataWord == 0)
                    {//更新列表
                        appModbusRegedit[in_solidNum].msRegStatsus = STATUS_IDLE;
                        appModbusRegedit[in_solidNum].updateFlag = false;
                        app_master_slave_updata_local_device_list(in_solidNum);//更新列表
                    }                     
                }
            }             
            break;
        }
        case STATUS_DISPOSE_DATA:
        {       
            appModbusRegedit[in_solidNum].msRegStatsus = STATUS_IDLE;
            break;
        }
        default:
        {
            break;
        }          
    }
}

void app_modbus_regedit_task(void)
{
    uint8_t i = 0;
    static sdt_bool cfged = sdt_false;
    if(cfged)
    {      
        for(i = 0; i < MAX_MODBUS_NUM;i++)
        {
            app_master_slave_regedit_task(i);
        }    
    }
    else
    {
        cfged = sdt_true;
        for(i = 0; i < MAX_MODBUS_NUM;i++)
        {
            mde_msRtu_master_slave_push_mRtu_local_id(i,local_addr);
            appModbusRegedit[i].send_wait_delay.timStatusBits = timerType_millisecond;
            appModbusRegedit[i].send_long_wait_delay.timStatusBits = timerType_second;
            appModbusRegedit[i].updata_list_delay.timStatusBits = timerType_second;
            appModbusRegedit[i].online_delay.timStatusBits = timerType_second;
            pbc_reload_timerClock(&appModbusRegedit[i].send_wait_delay,2000+GetRandomDelayTime());
            pbc_reload_timerClock(&appModbusRegedit[i].online_delay,190);
            appModbusRegedit[i].send_long_wait_delay.timStatusBits |= timStatusBits_onceTriggered;
            appModbusRegedit[i].msRegStatsus = STATUS_IDLE;
           
        }     
    }  
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
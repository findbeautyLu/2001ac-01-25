//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\app_cfg.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef enum
{
    STATUS_IDLE      = 0x00,              //空闲状态
    STATUS_UN_LOG,                        //未登录
    STATUS_LOG_TO_FIRST,                  //登录成1号设备
    STATUS_LOG_TO_SECOND,                 //登录成2号设备
    STATUS_LOG_TO_OTHER,                  //登录成其它设备          
}linkStatus_def;

typedef enum
{
    UN_LOG_IDLE         = 0x00,          //空闲状态
    UN_LOG_WAIT_RECEIVE,                 //等待接收
    UN_LOG_BOARDCAST_ACTIVE,             //申请组网激活
    UN_LOG_BOARDCAST_ACTIVE_RESULT,      //申请组网激活结果
    UN_LOG_SEND_BOARDCAST,               //申请组网
    UN_LOG_SEND_BOARDCAST_RESULT,        //申请组网结果
    UN_LOG_SEND_BOARDCAST_1,               //申请组网
    UN_LOG_SEND_BOARDCAST_RESULT_1,        //申请组网结果
    UN_LOG_SEND_BOARDCAST_ROLL_BACK,     //回退
    UN_LOG_WAIT_LOG_OR_UPDATA,           //等待注册或者更新   主状态跳跃 

    UN_LOG_DETECT_ACTIVE,              //申请加入当前网络激活
    UN_LOG_DETECT_ACTIVE_RESULT,       //申请加入当前网络激活结果
    UN_LOG_SEND_DETECT,                //申请加入当前网络   
    UN_LOG_SEND_DETECT_RESULT,         //申请加入当前网络结果
    UN_LOG_SEND_DETECT_ROLL_BACK,      //申请加入当前网络回退
    UN_LOG_RECEIVE_DETECT,             //接收申请加入当前网络
    
    UN_LOG_DETECT_ACTIVE_1,              //申请加入当前网络激活
    UN_LOG_DETECT_ACTIVE_RESULT_1,       //申请加入当前网络激活结果
    UN_LOG_SEND_DETECT_1,                //申请加入当前网络   
    UN_LOG_SEND_DETECT_RESULT_1,         //申请加入当前网络结果
    UN_LOG_SEND_DETECT_ROLL_BACK_1,      //申请加入当前网络回退
    UN_LOG_RECEIVE_DETECT_1,             //接收申请加入当前网络

    UN_LOG_DETECT_ACTIVE_2,              //申请加入当前网络激活
    UN_LOG_DETECT_ACTIVE_RESULT_2,       //申请加入当前网络激活结果
    UN_LOG_SEND_DETECT_2,                //申请加入当前网络   
    UN_LOG_SEND_DETECT_RESULT_2,         //申请加入当前网络结果
    UN_LOG_SEND_DETECT_ROLL_BACK_2,      //申请加入当前网络回退
    UN_LOG_RECEIVE_DETECT_2,             //接收申请加入当前网络


    UN_LOG_REGISTER_ACTIVE,              //注册激活
    UN_LOG_REGISTER_ACTIVE_RESULT,       //激活结果
    UN_LOG_SEND_REGISTER,                //发送注册帧   
    UN_LOG_SEND_REGISTER_RESULT,         //发送注册帧结果
    UN_LOG_SEND_REGISTER_ROLL_BACK,      //发送注册帧回退
    UN_LOG_RECEIVE_REGISTER,             //接收注册帧数据 
    UN_LOG_RESEND_REGISTER_ACTIVE,         //注册激活
    UN_LOG_RESEND_REGISTER_ACTIVE_RESULT,  //激活结果
    UN_LOG_RESEND_REGISTER,                //发送注册帧   
    UN_LOG_RESEND_REGISTER_RESULT,         //发送注册帧结果
    UN_LOG_RESEND_REGISTER_ROLL_BACK,      //发送注册帧回退
    UN_LOG_RERECEIVE_REGISTER,             //接收注册帧数据
    UN_LOG_ANSWER,                         //响应
    UN_LOG_ANSWER_RESULT,                  //响应结果
}unlogStatus_def;

typedef enum
{
    LOG_FIRST_IDLE         = 0x00,          //空闲状态
    LOG_FIRST_WAIT_RECEIVE,                 //等待接收
    LOG_FIRST_SEND_LIST_ACTIVE,             //发送列表激活
    LOG_FIRST_SEND_LIST_ACTIVE_RESULT,      //激活结果
    LOG_FIRST_SEND_LIST,                    //发送列表
    LOG_FIRST_SEND_LIST_RESULT,             //发送列表结果   
    LOG_FIRST_RECEIVE_LIST,                 //接收结果 
    LOG_FIRST_RESEND_LIST_ACTIVE,            //重发发送列表激活
    LOG_FIRST_RESEND_LIST_ACTIVE_RESULT,      //重发激活结果
    LOG_FIRST_RESEND_LIST,                    //重发发送列表
    LOG_FIRST_RESEND_LIST_RESULT,             //重发发送列表结果   
    LOG_FIRST_RERECEIVE_LIST,                 //重发接收结果 
    LOG_FIRST_ANSWER,                         //响应
    LOG_FIRST_ANSWER_RESULT,                  //响应结果
}firstStatus_def;


typedef enum
{
    LOG_SECOND_IDLE         = 0x00,          //空闲状态
    LOG_SECOND_WAIT_RECEIVE,                //等待接收
    LOG_SECOND_ANSWER,                         //响应
    LOG_SECOND_ANSWER_RESULT,                  //响应结果
}secondStatus_def;

typedef enum
{
    LOG_OTHER_IDLE         = 0x00,          //空闲状态
    LOG_OTHER_WAIT_RECEIVE,                //等待接收
    LOG_OTHER_ANSWER,                         //响应
    LOG_OTHER_ANSWER_RESULT,                  //响应结果
}otherStatus_def;

typedef struct
{
    linkStatus_def    mainLinkStatus;     //链路主状态
    unlogStatus_def   unlogSubStatus;     //未登录子状态
    firstStatus_def   firstSubStatus;
    secondStatus_def  secondSubStatus;
    otherStatus_def   ohterSubStatus; 
    uint8_t silenceCount;
    uint8_t updataNumber;
    bgk_comm_buff_def *in_rev_data;       //收到数据
    bool              receiveOneMessage;//收到一帧数据
    uint8_t           send_cmd;
    timerClock_def    receive_timeout_delay;//接收超时
    timerClock_def    send_boardcast_delay;//广播帧延时
    timerClock_def    send_wait_delay;    //主动发送等待
    timerClock_def    updata_list_delay;  //更新列表
    timerClock_def    online_delay;       //在綫時長
    timerClock_def    roll_back_delay;    //回退延时
    timerClock_def    answer_delay;    //应答延时
    timerClock_def    detect_delay;    //侦测延时
    timerClock_def    requese_net_delay;    //发送申请当前网络延时
    uint16_t          updataWord;        //更新字
    linkDeviceList_t  deviceList[MAX_DEVICE_NUM];  //设备列表
}msComm_t;

msComm_t appModbusRegedit[MAX_MODBUS_NUM];
             
sdt_int8u board_addr[6]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

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
linkDeviceList_t* app_link_log_pull_device_list(uint8_t in_solidNum)
{
    return &appModbusRegedit[in_solidNum].deviceList[0];
}
//发送注册数据
void app_master_slave_send_regedit_data(uint8_t in_solidNum,uint8_t *in_dest_addr,uint8_t control_cmd)
{
    uint8_t regeditNum = 0;
    bgk_comm_buff_def* tempLinkData;
    tempLinkData = pull_bough_message_pBuff(in_solidNum);
    tempLinkData->ProcotolType = PROTOL_REGEDIT;
    tempLinkData->LinkDstAddr[0] = in_dest_addr[0];
    tempLinkData->LinkDstAddr[1] = in_dest_addr[1];
    tempLinkData->LinkDstAddr[2] = in_dest_addr[2];
    tempLinkData->LinkDstAddr[3] = in_dest_addr[3];
    tempLinkData->LinkDstAddr[4] = in_dest_addr[4];
    tempLinkData->LinkDstAddr[5] = in_dest_addr[5];
    uint8_t *local_addr;
    local_addr = app_pull_local_id();
    tempLinkData->LinkSrcAddr[0] = local_addr[0];
    tempLinkData->LinkSrcAddr[1] = local_addr[1];
    tempLinkData->LinkSrcAddr[2] = local_addr[2];
    tempLinkData->LinkSrcAddr[3] = local_addr[3];
    tempLinkData->LinkSrcAddr[4] = local_addr[4];
    tempLinkData->LinkSrcAddr[5] = local_addr[5];

    tempLinkData->Payload[0] = PROTOL_VERSION;
    tempLinkData->Payload[1] = control_cmd;
    tempLinkData->Payload[2] = 0x00;//预留
    if((CONTROL_REGEDIT == control_cmd) || (CONTROL_DETECT == control_cmd))
    {
        tempLinkData->Payload[3] = (uint8_t)(DEVICE_TYPE>>8);//设备类型
        tempLinkData->Payload[4] = (uint8_t)DEVICE_TYPE;
        tempLinkData->PayloadLength = 5;
    }
    else if(CONTROL_UPDATA == control_cmd)
    {
        regeditNum = app_master_slave_pull_device_list_num(in_solidNum);
        tempLinkData->Payload[3] = regeditNum;
        uint8_t i = 0;
        for(i = 0;i < regeditNum;i++)
        {
            tempLinkData->Payload[4+8*i] =  (uint8_t)(appModbusRegedit[in_solidNum].deviceList[i].deviceType>>8); 
            tempLinkData->Payload[5+8*i] =  (uint8_t)(appModbusRegedit[in_solidNum].deviceList[i].deviceType);
            tempLinkData->Payload[6+8*i] =  (uint8_t)(appModbusRegedit[in_solidNum].deviceList[i].DeviceID[0]);
            tempLinkData->Payload[7+8*i] =  (uint8_t)(appModbusRegedit[in_solidNum].deviceList[i].DeviceID[1]);
            tempLinkData->Payload[8+8*i] =  (uint8_t)(appModbusRegedit[in_solidNum].deviceList[i].DeviceID[2]);
            tempLinkData->Payload[9+8*i] =  (uint8_t)(appModbusRegedit[in_solidNum].deviceList[i].DeviceID[3]);
            tempLinkData->Payload[10+8*i] =  (uint8_t)(appModbusRegedit[in_solidNum].deviceList[i].DeviceID[4]);
            tempLinkData->Payload[11+8*i] =  (uint8_t)(appModbusRegedit[in_solidNum].deviceList[i].DeviceID[5]);
        }
        tempLinkData->PayloadLength = (4+tempLinkData->Payload[3]*8);
    }
    else
    {
        tempLinkData->Payload[0] = PROTOL_VERSION;
        tempLinkData->Payload[1] = control_cmd;
        tempLinkData->PayloadLength = 2;
    }  
    push_active_one_message_transmit(in_solidNum,false);
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

bool app_link_log_pull_local_device_online(uint8_t in_solidNum)
{
    if((appModbusRegedit[in_solidNum].mainLinkStatus != STATUS_IDLE) && (appModbusRegedit[in_solidNum].mainLinkStatus != STATUS_UN_LOG))
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
   uint8_t *local_addr;
   local_addr = app_pull_local_id(); 
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
    uint8_t i;
    deviceNum = app_master_slave_pull_device_list_num(in_solidNum);
    appModbusRegedit[in_solidNum].deviceList[0].onlineFlag = false;//踢掉0号设备
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
		appModbusRegedit[in_solidNum].deviceList[i].DeviceID[0] = 0xff;
		appModbusRegedit[in_solidNum].deviceList[i].DeviceID[5] = 0xff;
		
    }
}

void app_master_slave_updata_local_device_list(uint8_t in_solidNum)
{//1号设备进行定时更新时无响应使用
    uint8_t deviceNum = 0;
    uint16_t updataNum = 0;
    uint8_t i,j;
    deviceNum = app_master_slave_pull_device_list_num(in_solidNum);
    for(i = 0;i < MAX_DEVICE_NUM;i++)
    {
        if(appModbusRegedit[in_solidNum].deviceList[i].onlineFlag)
        {
            updataNum |= (0x01<<i);
        }       
    }
    for(i = 0; i < deviceNum;i++)
    {
        for(j = 0; j < MAX_DEVICE_NUM;j++)
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
	appModbusRegedit[in_solidNum].deviceList[i].onlineFlag = false;
	appModbusRegedit[in_solidNum].deviceList[i].DeviceID[0] = 0xff;
	appModbusRegedit[in_solidNum].deviceList[i].DeviceID[5] = 0xff;
}


bool pull_first_device(uint8_t in_solidNum)
{
    uint8_t *local_addr;
    local_addr = app_pull_local_id();
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





void _log_task(uint8_t in_solidNum)
{
    bool newIdFlag = true;
    uint8_t i = 0;
    uint8_t regeditNum;
    pbc_timerClockRun_task(&appModbusRegedit[in_solidNum].send_boardcast_delay);//发送广播帧
    pbc_timerMillRun_task(&appModbusRegedit[in_solidNum].receive_timeout_delay);//接收超时
    pbc_timerClockRun_task(&appModbusRegedit[in_solidNum].updata_list_delay);//更新列表
    pbc_timerClockRun_task(&appModbusRegedit[in_solidNum].online_delay);//在线时长
    pbc_timerMillRun_task(&appModbusRegedit[in_solidNum].send_wait_delay);//发送等待
    pbc_timerMillRun_task(&appModbusRegedit[in_solidNum].roll_back_delay);//发送回退
    pbc_timerMillRun_task(&appModbusRegedit[in_solidNum].answer_delay);//响应延时
    pbc_timerMillRun_task(&appModbusRegedit[in_solidNum].detect_delay);//侦测延时
    pbc_timerMillRun_task(&appModbusRegedit[in_solidNum].requese_net_delay);//申请加入当前网络延时
    linkStatus_def  mainLinkStatus_backup;
    uint8_t oldId = 0;
    do
    {
        mainLinkStatus_backup = appModbusRegedit[in_solidNum].mainLinkStatus;
        switch (appModbusRegedit[in_solidNum].mainLinkStatus)
        {
            case STATUS_IDLE:
            {
                appModbusRegedit[in_solidNum].mainLinkStatus = STATUS_UN_LOG;
                appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_WAIT_RECEIVE;
                break;
            }
            case STATUS_UN_LOG:
            {//未登录
                switch(appModbusRegedit[in_solidNum].unlogSubStatus)
                {
                    case UN_LOG_WAIT_RECEIVE:
                    {//接收到广播帧或者发送广播帧
                        if(appModbusRegedit[in_solidNum].receiveOneMessage)
                        {
                            appModbusRegedit[in_solidNum].receiveOneMessage = false;
                            pbc_reload_timerClock(&appModbusRegedit[in_solidNum].send_wait_delay,DETECT_DELAY);//延时3s发广播帧
                            if(pull_boardcast_id(&appModbusRegedit[in_solidNum].in_rev_data->LinkDstAddr[0]))
                            {//广播地址，忽略其它地址  断电再上电可能收到注册帧跟更新帧
                                if(appModbusRegedit[in_solidNum].in_rev_data->Payload[1] == CONTROL_REGEDIT)
                                {//申请组网帧
                                    appModbusRegedit[in_solidNum].deviceList[0].onlineFlag = true;
                                    appModbusRegedit[in_solidNum].deviceList[0].DeviceID[0] = appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[0];
                                    appModbusRegedit[in_solidNum].deviceList[0].DeviceID[1] = appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[1];
                                    appModbusRegedit[in_solidNum].deviceList[0].DeviceID[2] = appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[2];
                                    appModbusRegedit[in_solidNum].deviceList[0].DeviceID[3] = appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[3];
                                    appModbusRegedit[in_solidNum].deviceList[0].DeviceID[4] = appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[4];
                                    appModbusRegedit[in_solidNum].deviceList[0].DeviceID[5] = appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[5];
                                    appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_REGISTER_ACTIVE;
                                }                                  
                            }
                        }
                        else if((pbc_pull_timerIsCompleted(&appModbusRegedit[in_solidNum].send_boardcast_delay)) && (pbc_pull_timerIsCompleted(&appModbusRegedit[in_solidNum].send_wait_delay)))
                        {//申请加入当前网络
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_DETECT_ACTIVE;              
                        }
                        break;
                    }

                    case UN_LOG_DETECT_ACTIVE:
                    {//申请加入当前网络激活
                        if(app_link_request_transmit_activation_request(in_solidNum))
                        {//激活成功
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_DETECT_ACTIVE_RESULT;
                        }
                        else 
                        {//激活失败
                          //  appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_WAIT_RECEIVE;
                        }           
                        break;
                    }
                    case UN_LOG_DETECT_ACTIVE_RESULT:
                    {//申请加入当前网络激活结果
                        if(TRANSMIT_RESULT_FAIL == app_link_request_get_transmit_result(in_solidNum))
                        {//发送失败
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_DETECT_ACTIVE; 
                        } 
                        else if(TRANSMIT_RESULT_SUCCESS == app_link_request_get_transmit_result(in_solidNum))
                        {//发送成功
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_SEND_DETECT; 
                        }   
                        break;
                    }
                    case UN_LOG_SEND_DETECT:
                    {//申请加入当前网络
                        if(pull_bough_link_is_busy(in_solidNum))
                        {//忙碌先释放
                            //appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_WAIT_RECEIVE;
                        }
                        else
                        {
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_SEND_DETECT_RESULT; 
                            app_master_slave_send_regedit_data(in_solidNum,&board_addr[0],CONTROL_DETECT);                           
                        }                                     
                        break;
                    }
                    case UN_LOG_SEND_DETECT_RESULT:
                    {//申请加入当前网络结果
                        if(pull_bough_transmit_monitor(in_solidNum) == bgk_trans_mon_complete)
                        {//发送完成
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_DETECT_ACTIVE_1;
                            pbc_reload_timerClock(&appModbusRegedit[in_solidNum].requese_net_delay,REQUST_NET_DELAY);
                        }
                        else if(pull_bough_transmit_monitor(in_solidNum) == bgk_trans_mon_conflict)
                        {//冲突报文
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_SEND_DETECT_ROLL_BACK;
                            pbc_reload_timerClock(&appModbusRegedit[in_solidNum].roll_back_delay,ROLL_BACK_DELAY);
                        }
                        else if(pull_bough_transmit_monitor(in_solidNum) == bgk_trans_mon_error)
                        {//未知错误
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_SEND_DETECT_ROLL_BACK;
                            pbc_reload_timerClock(&appModbusRegedit[in_solidNum].roll_back_delay,ROLL_BACK_DELAY);
                        }
                        break;
                    }
                    case UN_LOG_SEND_DETECT_ROLL_BACK:
                    {//申请加入当前网络回退
                        if(pbc_pull_timerIsCompleted(&appModbusRegedit[in_solidNum].roll_back_delay))
                        {
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_SEND_DETECT;
                        }
                        break;
                    }

                    case UN_LOG_DETECT_ACTIVE_1:
                    {//申请加入当前网络激活
                        if(appModbusRegedit[in_solidNum].receiveOneMessage)
                        {
                            appModbusRegedit[in_solidNum].receiveOneMessage = false;
                            pbc_reload_timerClock(&appModbusRegedit[in_solidNum].requese_net_delay,DETECT_DELAY);//收到数据等待3s
                            if(pull_local_id(&appModbusRegedit[in_solidNum].in_rev_data->LinkDstAddr[0]))
                            {
                                appModbusRegedit[in_solidNum].send_cmd = appModbusRegedit[in_solidNum].in_rev_data->Payload[1];
                                appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_ANSWER;
                                pbc_reload_timerClock(&appModbusRegedit[in_solidNum].answer_delay,ANSWER_DELAY);
                                if(appModbusRegedit[in_solidNum].in_rev_data->Payload[1] == CONTROL_UPDATA)
                                { //更新帧                                 
                                    uint8_t regeditNum = appModbusRegedit[in_solidNum].in_rev_data->Payload[3];
                                    for(i = 0;i < regeditNum;i++)
                                    {
                                        appModbusRegedit[in_solidNum].deviceList[i].onlineFlag = true;
                                        appModbusRegedit[in_solidNum].deviceList[i].deviceType = pbc_arrayToInt16u_bigEndian(&appModbusRegedit[in_solidNum].in_rev_data->Payload[4+i*8]);
                                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[0] = appModbusRegedit[in_solidNum].in_rev_data->Payload[6+i*8];
                                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[1] = appModbusRegedit[in_solidNum].in_rev_data->Payload[7+i*8];
                                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[2] = appModbusRegedit[in_solidNum].in_rev_data->Payload[8+i*8];
                                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[3] = appModbusRegedit[in_solidNum].in_rev_data->Payload[9+i*8];
                                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[4] = appModbusRegedit[in_solidNum].in_rev_data->Payload[10+i*8];
                                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[5] = appModbusRegedit[in_solidNum].in_rev_data->Payload[11+i*8];
                                    }
                                    for(i = regeditNum;i < MAX_DEVICE_NUM;i++)
                                    {
                                        appModbusRegedit[in_solidNum].deviceList[i].onlineFlag = false;
                                    }
                                }
                            }
                        }
                        else
                        {
                            if(pbc_pull_timerIsCompleted(&appModbusRegedit[in_solidNum].requese_net_delay))
                            {
                                if(app_link_request_transmit_activation_request(in_solidNum))
                                {//激活成功
                                    appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_DETECT_ACTIVE_RESULT_1;
                                }
                                else 
                                {//激活失败
                                //  appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_WAIT_RECEIVE;
                                }      
                            }
                        }                               
                        break;
                    }
                    case UN_LOG_DETECT_ACTIVE_RESULT_1:
                    {//申请加入当前网络激活结果
                        if(TRANSMIT_RESULT_FAIL == app_link_request_get_transmit_result(in_solidNum))
                        {//发送失败
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_DETECT_ACTIVE_1; 
                        } 
                        else if(TRANSMIT_RESULT_SUCCESS == app_link_request_get_transmit_result(in_solidNum))
                        {//发送成功
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_SEND_DETECT_1; 
                        }   
                        break;
                    }
                    case UN_LOG_SEND_DETECT_1:
                    {//申请加入当前网络
                        if(pull_bough_link_is_busy(in_solidNum))
                        {//忙碌先释放
                            //appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_WAIT_RECEIVE;
                        }
                        else
                        {
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_SEND_DETECT_RESULT_1; 
                            app_master_slave_send_regedit_data(in_solidNum,&board_addr[0],CONTROL_DETECT);                           
                        }                                     
                        break;
                    }
                    case UN_LOG_SEND_DETECT_RESULT_1:
                    {//申请加入当前网络结果
                        if(pull_bough_transmit_monitor(in_solidNum) == bgk_trans_mon_complete)
                        {//发送完成
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_DETECT_ACTIVE_2;
                            pbc_reload_timerClock(&appModbusRegedit[in_solidNum].requese_net_delay,REQUST_NET_DELAY);
                        }
                        else if(pull_bough_transmit_monitor(in_solidNum) == bgk_trans_mon_conflict)
                        {//冲突报文
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_SEND_DETECT_ROLL_BACK_1;
                            pbc_reload_timerClock(&appModbusRegedit[in_solidNum].roll_back_delay,ROLL_BACK_DELAY);
                        }
                        else if(pull_bough_transmit_monitor(in_solidNum) == bgk_trans_mon_error)
                        {//未知错误
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_SEND_DETECT_ROLL_BACK_1;
                            pbc_reload_timerClock(&appModbusRegedit[in_solidNum].roll_back_delay,ROLL_BACK_DELAY);
                        }
                        break;
                    }
                    case UN_LOG_SEND_DETECT_ROLL_BACK_1:
                    {//申请加入当前网络回退
                        if(pbc_pull_timerIsCompleted(&appModbusRegedit[in_solidNum].roll_back_delay))
                        {
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_SEND_DETECT_1;
                        }
                        break;
                    }

                    case UN_LOG_DETECT_ACTIVE_2:
                    {//申请加入当前网络激活
                        if(appModbusRegedit[in_solidNum].receiveOneMessage)
                        {
                            appModbusRegedit[in_solidNum].receiveOneMessage = false;
                            pbc_reload_timerClock(&appModbusRegedit[in_solidNum].requese_net_delay,DETECT_DELAY);//收到数据等待3s
                            if(pull_local_id(&appModbusRegedit[in_solidNum].in_rev_data->LinkDstAddr[0]))
                            {
                                appModbusRegedit[in_solidNum].send_cmd = appModbusRegedit[in_solidNum].in_rev_data->Payload[1];
                                appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_ANSWER;
                                pbc_reload_timerClock(&appModbusRegedit[in_solidNum].answer_delay,ANSWER_DELAY);
                                if(appModbusRegedit[in_solidNum].in_rev_data->Payload[1] == CONTROL_UPDATA)
                                { //更新帧                                 
                                    uint8_t regeditNum = appModbusRegedit[in_solidNum].in_rev_data->Payload[3];
                                    for(i = 0;i < regeditNum;i++)
                                    {
                                        appModbusRegedit[in_solidNum].deviceList[i].onlineFlag = true;
                                        appModbusRegedit[in_solidNum].deviceList[i].deviceType = pbc_arrayToInt16u_bigEndian(&appModbusRegedit[in_solidNum].in_rev_data->Payload[4+i*8]);
                                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[0] = appModbusRegedit[in_solidNum].in_rev_data->Payload[6+i*8];
                                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[1] = appModbusRegedit[in_solidNum].in_rev_data->Payload[7+i*8];
                                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[2] = appModbusRegedit[in_solidNum].in_rev_data->Payload[8+i*8];
                                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[3] = appModbusRegedit[in_solidNum].in_rev_data->Payload[9+i*8];
                                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[4] = appModbusRegedit[in_solidNum].in_rev_data->Payload[10+i*8];
                                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[5] = appModbusRegedit[in_solidNum].in_rev_data->Payload[11+i*8];
                                    }
                                    for(i = regeditNum;i < MAX_DEVICE_NUM;i++)
                                    {
                                        appModbusRegedit[in_solidNum].deviceList[i].onlineFlag = false;
                                    }
                                }
                            }
                        }
                        else
                        {
                            if(pbc_pull_timerIsCompleted(&appModbusRegedit[in_solidNum].requese_net_delay))
                            {
                                if(app_link_request_transmit_activation_request(in_solidNum))
                                {//激活成功
                                    appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_DETECT_ACTIVE_RESULT_2;
                                }
                                else 
                                {//激活失败
                                //  appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_WAIT_RECEIVE;
                                }      
                            }
                        }                               
                        break;
                    }
                    case UN_LOG_DETECT_ACTIVE_RESULT_2:
                    {//申请加入当前网络激活结果
                        if(TRANSMIT_RESULT_FAIL == app_link_request_get_transmit_result(in_solidNum))
                        {//发送失败
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_DETECT_ACTIVE_2; 
                        } 
                        else if(TRANSMIT_RESULT_SUCCESS == app_link_request_get_transmit_result(in_solidNum))
                        {//发送成功
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_SEND_DETECT_2; 
                        }   
                        break;
                    }
                    case UN_LOG_SEND_DETECT_2:
                    {//申请加入当前网络
                        if(pull_bough_link_is_busy(in_solidNum))
                        {//忙碌先释放
                            //appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_WAIT_RECEIVE;
                        }
                        else
                        {
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_SEND_DETECT_RESULT_2; 
                            app_master_slave_send_regedit_data(in_solidNum,&board_addr[0],CONTROL_DETECT);                           
                        }                                     
                        break;
                    }
                    case UN_LOG_SEND_DETECT_RESULT_2:
                    {//申请加入当前网络结果
                        if(pull_bough_transmit_monitor(in_solidNum) == bgk_trans_mon_complete)
                        {//发送完成
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_BOARDCAST_ACTIVE;
                            pbc_reload_timerClock(&appModbusRegedit[in_solidNum].requese_net_delay,REQUST_NET_DELAY);
                        }
                        else if(pull_bough_transmit_monitor(in_solidNum) == bgk_trans_mon_conflict)
                        {//冲突报文
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_SEND_DETECT_ROLL_BACK_2;
                            pbc_reload_timerClock(&appModbusRegedit[in_solidNum].roll_back_delay,ROLL_BACK_DELAY);
                        }
                        else if(pull_bough_transmit_monitor(in_solidNum) == bgk_trans_mon_error)
                        {//未知错误
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_SEND_DETECT_ROLL_BACK_2;
                            pbc_reload_timerClock(&appModbusRegedit[in_solidNum].roll_back_delay,ROLL_BACK_DELAY);
                        }
                        break;
                    }
                    case UN_LOG_SEND_DETECT_ROLL_BACK_2:
                    {//申请加入当前网络回退
                        if(pbc_pull_timerIsCompleted(&appModbusRegedit[in_solidNum].roll_back_delay))
                        {
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_SEND_DETECT_2;
                        }
                        break;
                    }
                    case UN_LOG_REGISTER_ACTIVE:
                    {//注册帧激活
                        if(pbc_pull_timerIsCompleted(&appModbusRegedit[in_solidNum].detect_delay))
                        {
                            if(app_link_request_transmit_activation_request(in_solidNum))
                            {//激活成功
                                appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_REGISTER_ACTIVE_RESULT;
                            }
                            else 
                            {//激活失败
                            //  appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_WAIT_RECEIVE;
                            }       
                        }                          
                        break;
                    }
                    case UN_LOG_REGISTER_ACTIVE_RESULT:
                    {//注册帧激活结果
                        if(TRANSMIT_RESULT_FAIL == app_link_request_get_transmit_result(in_solidNum))
                        {//发送失败
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_REGISTER_ACTIVE; 
                        } 
                        else if(TRANSMIT_RESULT_SUCCESS == app_link_request_get_transmit_result(in_solidNum))
                        {//发送成功
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_SEND_REGISTER; 
                        }   
                        break;
                    }
                    case UN_LOG_SEND_REGISTER:
                    {//发送注册帧
                        if(pull_bough_link_is_busy(in_solidNum))
                        {//忙碌先释放
                            //appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_WAIT_RECEIVE;
                        }
                        else
                        {
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_SEND_REGISTER_RESULT; 
                            app_master_slave_send_regedit_data(in_solidNum,&appModbusRegedit[in_solidNum].deviceList[0].DeviceID[0],CONTROL_REGEDIT);
                        }                                     
                        break;
                    }
                    case UN_LOG_SEND_REGISTER_RESULT:
                    {//发送结果
                        if(pull_bough_transmit_monitor(in_solidNum) == bgk_trans_mon_complete)
                        {//发送完成
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_RECEIVE_REGISTER;
                            pbc_reload_timerClock(&appModbusRegedit[in_solidNum].receive_timeout_delay,RECEIVE_TIMEOUT);
                        }
                        else if(pull_bough_transmit_monitor(in_solidNum) == bgk_trans_mon_conflict)
                        {//冲突报文
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_SEND_REGISTER_ROLL_BACK;
                            pbc_reload_timerClock(&appModbusRegedit[in_solidNum].roll_back_delay,ROLL_BACK_DELAY);
                        }
                        else if(pull_bough_transmit_monitor(in_solidNum) == bgk_trans_mon_error)
                        {//未知错误
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_WAIT_RECEIVE;
                        }
                        break;
                    }
                    case UN_LOG_SEND_REGISTER_ROLL_BACK:
                    {//冲突回退
                        if(pbc_pull_timerIsCompleted(&appModbusRegedit[in_solidNum].roll_back_delay))
                        {
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_SEND_REGISTER;
                        }
                        break;
                    }
                    case UN_LOG_RECEIVE_REGISTER:
                    {//注册帧接收结果
                        if(appModbusRegedit[in_solidNum].receiveOneMessage)
                        {
                            appModbusRegedit[in_solidNum].receiveOneMessage = false;
                            if(pull_local_id(&appModbusRegedit[in_solidNum].in_rev_data->LinkDstAddr[0]))
                            {//自己地址
                                if(CONTROL_REGEDIT_ACK == appModbusRegedit[in_solidNum].in_rev_data->Payload[1])
                                {//注册应答
                                    appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_WAIT_LOG_OR_UPDATA;
                                    pbc_reload_timerClock(&appModbusRegedit[in_solidNum].send_boardcast_delay,30);//静默30s
                                }
                                else
                                {//其它数据则重新发起注册
                                    appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_WAIT_RECEIVE;
                                }                              
                            }
                        }
                        else if(pbc_pull_timerIsCompleted(&appModbusRegedit[in_solidNum].receive_timeout_delay))
                        {//接收超时重发一帧
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_RESEND_REGISTER_ACTIVE;
                        }
                        break;
                    }
                    case UN_LOG_RESEND_REGISTER_ACTIVE:
                    {//重发注册帧激活
                        if(app_link_request_transmit_activation_request(in_solidNum))
                        {//激活成功
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_RESEND_REGISTER_ACTIVE_RESULT;
                        }
                        else 
                        {//激活失败
                       //     appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_WAIT_RECEIVE;
                        }           
                        break;
                    }
                    case UN_LOG_RESEND_REGISTER_ACTIVE_RESULT:
                    {//注册帧激活结果
                        if(TRANSMIT_RESULT_FAIL == app_link_request_get_transmit_result(in_solidNum))
                        {//发送失败
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_RESEND_REGISTER_ACTIVE; 
                        } 
                        else if(TRANSMIT_RESULT_SUCCESS == app_link_request_get_transmit_result(in_solidNum))
                        {//发送成功
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_RESEND_REGISTER; 
                        }   
                        break;
                    }
                    
                    case UN_LOG_RESEND_REGISTER:
                    {//发送注册帧
                        if(pull_bough_link_is_busy(in_solidNum))
                        {//忙碌先释放
                           // appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_WAIT_RECEIVE;
                        }
                        else
                        {
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_RESEND_REGISTER_RESULT; 
                            app_master_slave_send_regedit_data(in_solidNum,&appModbusRegedit[in_solidNum].deviceList[0].DeviceID[0],CONTROL_REGEDIT);
                        }                                     
                        break;
                    }
                    case UN_LOG_RESEND_REGISTER_RESULT:
                    {//发送结果
                        if(pull_bough_transmit_monitor(in_solidNum) == bgk_trans_mon_complete)
                        {//发送完成
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_RERECEIVE_REGISTER;
                            pbc_reload_timerClock(&appModbusRegedit[in_solidNum].receive_timeout_delay,RECEIVE_TIMEOUT);
                        }
                        else if(pull_bough_transmit_monitor(in_solidNum) == bgk_trans_mon_conflict)
                        {//冲突报文
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_RESEND_REGISTER_ROLL_BACK;
                            pbc_reload_timerClock(&appModbusRegedit[in_solidNum].roll_back_delay,ROLL_BACK_DELAY);
                        }
                        else if(pull_bough_transmit_monitor(in_solidNum) == bgk_trans_mon_error)
                        {//未知错误
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_WAIT_RECEIVE;
                        }
                        break;
                    }
                    case UN_LOG_RESEND_REGISTER_ROLL_BACK:
                    {
                        if(pbc_pull_timerIsCompleted(&appModbusRegedit[in_solidNum].roll_back_delay))
                        {
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_RESEND_REGISTER;
                        }
                        break;
                    }
                    case UN_LOG_RERECEIVE_REGISTER:
                    {//注册帧接收结果
                        if(appModbusRegedit[in_solidNum].receiveOneMessage)
                        {
                            appModbusRegedit[in_solidNum].receiveOneMessage = false;
                            if(pull_local_id(&appModbusRegedit[in_solidNum].in_rev_data->LinkDstAddr[0]))
                            {//自己地址
                                if(CONTROL_REGEDIT_ACK == appModbusRegedit[in_solidNum].in_rev_data->Payload[1])
                                {//注册应答
                                    appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_WAIT_LOG_OR_UPDATA;
                                    pbc_reload_timerClock(&appModbusRegedit[in_solidNum].send_boardcast_delay,30);//静默30s
                                }
                                else
                                {//其它数据则重新发起注册
                                    appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_WAIT_RECEIVE;
                                }                              
                            }
                        }
                        else if(pbc_pull_timerIsCompleted(&appModbusRegedit[in_solidNum].receive_timeout_delay))
                        {//接收超时
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_WAIT_RECEIVE;
                        }
                        break;
                    }
                    case UN_LOG_BOARDCAST_ACTIVE:
                    {//广播激活
                        if(appModbusRegedit[in_solidNum].receiveOneMessage)
                        {
                            appModbusRegedit[in_solidNum].receiveOneMessage = false;
                            if(pull_local_id(&appModbusRegedit[in_solidNum].in_rev_data->LinkDstAddr[0]))
                            {
                                appModbusRegedit[in_solidNum].send_cmd = appModbusRegedit[in_solidNum].in_rev_data->Payload[1];
                                appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_ANSWER;
                                pbc_reload_timerClock(&appModbusRegedit[in_solidNum].answer_delay,ANSWER_DELAY);
                                if(appModbusRegedit[in_solidNum].in_rev_data->Payload[1] == CONTROL_UPDATA)
                                { //更新帧                                 
                                    uint8_t regeditNum = appModbusRegedit[in_solidNum].in_rev_data->Payload[3];
                                    for(i = 0;i < regeditNum;i++)
                                    {
                                        appModbusRegedit[in_solidNum].deviceList[i].onlineFlag = true;
                                        appModbusRegedit[in_solidNum].deviceList[i].deviceType = pbc_arrayToInt16u_bigEndian(&appModbusRegedit[in_solidNum].in_rev_data->Payload[4+i*8]);
                                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[0] = appModbusRegedit[in_solidNum].in_rev_data->Payload[6+i*8];
                                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[1] = appModbusRegedit[in_solidNum].in_rev_data->Payload[7+i*8];
                                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[2] = appModbusRegedit[in_solidNum].in_rev_data->Payload[8+i*8];
                                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[3] = appModbusRegedit[in_solidNum].in_rev_data->Payload[9+i*8];
                                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[4] = appModbusRegedit[in_solidNum].in_rev_data->Payload[10+i*8];
                                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[5] = appModbusRegedit[in_solidNum].in_rev_data->Payload[11+i*8];
                                    }
                                    for(i = regeditNum;i < MAX_DEVICE_NUM;i++)
                                    {
                                        appModbusRegedit[in_solidNum].deviceList[i].onlineFlag = false;
                                    }
                                }
                            }
                        }
                        else
                        {
                            if(pbc_pull_timerIsCompleted(&appModbusRegedit[in_solidNum].requese_net_delay))
                            {
                                if(app_link_request_transmit_activation_request(in_solidNum))
                                {
                                    appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_BOARDCAST_ACTIVE_RESULT;
                                }  
                                else
                                {
                                    appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_WAIT_RECEIVE; 
                                }   
                            }                     
                        }                                                  
                        break;
                    }
                    case UN_LOG_BOARDCAST_ACTIVE_RESULT:
                    {
                        if(TRANSMIT_RESULT_FAIL == app_link_request_get_transmit_result(in_solidNum))
                        {//发送失败
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_WAIT_RECEIVE; 
                        } 
                        else if(TRANSMIT_RESULT_SUCCESS == app_link_request_get_transmit_result(in_solidNum))
                        {//发送成功
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_SEND_BOARDCAST; 
                        }   
                        break;
                    }
                    case UN_LOG_SEND_BOARDCAST:
                    {
                        if(pull_bough_link_is_busy(in_solidNum))
                        {
                           // appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_WAIT_RECEIVE; 
                        }
                        else
                        {
                            app_master_slave_send_regedit_data(in_solidNum,&board_addr[0],CONTROL_REGEDIT);
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_SEND_BOARDCAST_RESULT;
                        }
                        
                        break;
                    }
                    case UN_LOG_SEND_BOARDCAST_RESULT:
                    {
                        if(pull_bough_transmit_monitor(in_solidNum) == bgk_trans_mon_complete)
                        {//发送完成
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_WAIT_LOG_OR_UPDATA;
                            pbc_reload_timerClock(&appModbusRegedit[in_solidNum].send_boardcast_delay,(SEND_BOARDCAST_DELAY+appModbusRegedit[in_solidNum].silenceCount));//静默180s
                            if(appModbusRegedit[in_solidNum].silenceCount < SEND_BOARDCAST_DELAY_MAX)
                            {
                                appModbusRegedit[in_solidNum].silenceCount++;
                            }
                        }
                        else if(pull_bough_transmit_monitor(in_solidNum) == bgk_trans_mon_conflict)
                        {
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_SEND_BOARDCAST_ROLL_BACK;
                            pbc_reload_timerClock(&appModbusRegedit[in_solidNum].roll_back_delay,ROLL_BACK_DELAY);
                        }
                        else if(pull_bough_transmit_monitor(in_solidNum) == bgk_trans_mon_error)
                        {//冲突报文或者未知错误
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_SEND_BOARDCAST_ROLL_BACK;
                            pbc_reload_timerClock(&appModbusRegedit[in_solidNum].roll_back_delay,ROLL_BACK_DELAY);
                        }
                        break;
                    }
                    case UN_LOG_SEND_BOARDCAST_ROLL_BACK:
                    {
                        if(pbc_pull_timerIsCompleted(&appModbusRegedit[in_solidNum].roll_back_delay))
                        {
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_SEND_BOARDCAST_1;
                        }
                        break;
                    }
                    case UN_LOG_SEND_BOARDCAST_1:
                    {
                        if(pull_bough_link_is_busy(in_solidNum))
                        {
                           // appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_WAIT_RECEIVE; 
                        }
                        else
                        {
                            app_master_slave_send_regedit_data(in_solidNum,&board_addr[0],CONTROL_REGEDIT);
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_SEND_BOARDCAST_RESULT_1;
                        }
                        break;
                    }
                    case UN_LOG_SEND_BOARDCAST_RESULT_1:
                    {
                        if(pull_bough_transmit_monitor(in_solidNum) == bgk_trans_mon_complete)
                        {//发送完成
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_WAIT_LOG_OR_UPDATA;
                            pbc_reload_timerClock(&appModbusRegedit[in_solidNum].send_boardcast_delay,(SEND_BOARDCAST_DELAY+appModbusRegedit[in_solidNum].silenceCount));//静默180s
                            if(appModbusRegedit[in_solidNum].silenceCount < SEND_BOARDCAST_DELAY_MAX)
                            {
                                appModbusRegedit[in_solidNum].silenceCount++;
                            }
                        }
                        else if(pull_bough_transmit_monitor(in_solidNum) == bgk_trans_mon_conflict)
                        {
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_WAIT_RECEIVE;
                            pbc_reload_timerClock(&appModbusRegedit[in_solidNum].send_boardcast_delay,(SEND_BOARDCAST_DELAY+appModbusRegedit[in_solidNum].silenceCount));//静默180s
                            if(appModbusRegedit[in_solidNum].silenceCount < SEND_BOARDCAST_DELAY_MAX)
                            {
                                appModbusRegedit[in_solidNum].silenceCount++;
                            }
                        }
                        else if(pull_bough_transmit_monitor(in_solidNum) == bgk_trans_mon_error)
                        {//冲突报文或者未知错误
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_WAIT_RECEIVE;
                            pbc_reload_timerClock(&appModbusRegedit[in_solidNum].send_boardcast_delay,(SEND_BOARDCAST_DELAY+appModbusRegedit[in_solidNum].silenceCount));//静默180s
                            if(appModbusRegedit[in_solidNum].silenceCount < SEND_BOARDCAST_DELAY_MAX)
                            {
                                appModbusRegedit[in_solidNum].silenceCount++;
                            }
                        }
                        break;
                    }
                    case UN_LOG_WAIT_LOG_OR_UPDATA:
                    {//收到注册帧  或者更新帧
                        if(appModbusRegedit[in_solidNum].receiveOneMessage)
                        {
                            appModbusRegedit[in_solidNum].receiveOneMessage = false;
                            if(pull_boardcast_id(&appModbusRegedit[in_solidNum].in_rev_data->LinkDstAddr[0]))
                            {//广播地址，忽略其它地址  断电再上电可能收到注册帧跟更新帧
                                if(appModbusRegedit[in_solidNum].in_rev_data->Payload[1] == CONTROL_REGEDIT)
                                {//申请组网帧
                                    appModbusRegedit[in_solidNum].deviceList[0].onlineFlag = true;
                                    appModbusRegedit[in_solidNum].deviceList[0].DeviceID[0] = appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[0];
                                    appModbusRegedit[in_solidNum].deviceList[0].DeviceID[1] = appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[1];
                                    appModbusRegedit[in_solidNum].deviceList[0].DeviceID[2] = appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[2];
                                    appModbusRegedit[in_solidNum].deviceList[0].DeviceID[3] = appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[3];
                                    appModbusRegedit[in_solidNum].deviceList[0].DeviceID[4] = appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[4];
                                    appModbusRegedit[in_solidNum].deviceList[0].DeviceID[5] = appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[5];
                                    appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_REGISTER_ACTIVE;
                                }                                  
                            }
                            else if(pull_local_id(&appModbusRegedit[in_solidNum].in_rev_data->LinkDstAddr[0])) 
                            {//自己地址
                                appModbusRegedit[in_solidNum].send_cmd = appModbusRegedit[in_solidNum].in_rev_data->Payload[1];
                                appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_ANSWER;
                                pbc_reload_timerClock(&appModbusRegedit[in_solidNum].answer_delay,ANSWER_DELAY);
                                if((appModbusRegedit[in_solidNum].in_rev_data->Payload[1] == CONTROL_REGEDIT) || (appModbusRegedit[in_solidNum].in_rev_data->Payload[1] == CONTROL_DETECT))
                                {//收到注册帧  成为1号更新列表                                    
                                    appModbusRegedit[in_solidNum].deviceList[0].onlineFlag = true;
                                    appModbusRegedit[in_solidNum].deviceList[0].deviceType = DEVICE_TYPE;
                                    appModbusRegedit[in_solidNum].deviceList[0].DeviceID[0] = appModbusRegedit[in_solidNum].in_rev_data->LinkDstAddr[0];
                                    appModbusRegedit[in_solidNum].deviceList[0].DeviceID[1] = appModbusRegedit[in_solidNum].in_rev_data->LinkDstAddr[1];
                                    appModbusRegedit[in_solidNum].deviceList[0].DeviceID[2] = appModbusRegedit[in_solidNum].in_rev_data->LinkDstAddr[2];
                                    appModbusRegedit[in_solidNum].deviceList[0].DeviceID[3] = appModbusRegedit[in_solidNum].in_rev_data->LinkDstAddr[3];
                                    appModbusRegedit[in_solidNum].deviceList[0].DeviceID[4] = appModbusRegedit[in_solidNum].in_rev_data->LinkDstAddr[4];
                                    appModbusRegedit[in_solidNum].deviceList[0].DeviceID[5] = appModbusRegedit[in_solidNum].in_rev_data->LinkDstAddr[5];           
                                    for(i = 1;i < MAX_DEVICE_NUM;i++)
                                    {       
                                        if((appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[0] ==  appModbusRegedit[in_solidNum].deviceList[i].DeviceID[0])&&\
                                        (appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[1] ==  appModbusRegedit[in_solidNum].deviceList[i].DeviceID[1])&&\
                                        (appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[2] ==  appModbusRegedit[in_solidNum].deviceList[i].DeviceID[2])&&\
                                        (appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[3] ==  appModbusRegedit[in_solidNum].deviceList[i].DeviceID[3])&&\
                                        (appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[4] ==  appModbusRegedit[in_solidNum].deviceList[i].DeviceID[4])&&\
                                        (appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[5] ==  appModbusRegedit[in_solidNum].deviceList[i].DeviceID[5]))
                                        {
                                            newIdFlag = false;
											appModbusRegedit[in_solidNum].deviceList[i].onlineFlag = true;
                                        }
                                    }
                                    if(newIdFlag)
                                    {
                                        for(i = 1;i < MAX_DEVICE_NUM;i++)
                                        {
                                            if(appModbusRegedit[in_solidNum].deviceList[i].onlineFlag == false)
                                            {
                                                appModbusRegedit[in_solidNum].deviceList[i].onlineFlag = true;
                                                appModbusRegedit[in_solidNum].deviceList[i].deviceType = pbc_arrayToInt16u_bigEndian(&appModbusRegedit[in_solidNum].in_rev_data->Payload[3]);
                                                appModbusRegedit[in_solidNum].deviceList[i].DeviceID[0] = appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[0];
                                                appModbusRegedit[in_solidNum].deviceList[i].DeviceID[1] = appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[1];
                                                appModbusRegedit[in_solidNum].deviceList[i].DeviceID[2] = appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[2];
                                                appModbusRegedit[in_solidNum].deviceList[i].DeviceID[3] = appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[3];
                                                appModbusRegedit[in_solidNum].deviceList[i].DeviceID[4] = appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[4];
                                                appModbusRegedit[in_solidNum].deviceList[i].DeviceID[5] = appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[5];
                                                break;
                                            }
                                        }
                                    }                                  
                                }
                                else if(appModbusRegedit[in_solidNum].in_rev_data->Payload[1] == CONTROL_UPDATA)
                                {                                    
                                    uint8_t regeditNum = appModbusRegedit[in_solidNum].in_rev_data->Payload[3];
                                    for(i = 0;i < regeditNum;i++)
                                    {
                                        appModbusRegedit[in_solidNum].deviceList[i].onlineFlag = true;
                                        appModbusRegedit[in_solidNum].deviceList[i].deviceType = pbc_arrayToInt16u_bigEndian(&appModbusRegedit[in_solidNum].in_rev_data->Payload[4+i*8]);
                                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[0] = appModbusRegedit[in_solidNum].in_rev_data->Payload[6+i*8];
                                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[1] = appModbusRegedit[in_solidNum].in_rev_data->Payload[7+i*8];
                                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[2] = appModbusRegedit[in_solidNum].in_rev_data->Payload[8+i*8];
                                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[3] = appModbusRegedit[in_solidNum].in_rev_data->Payload[9+i*8];
                                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[4] = appModbusRegedit[in_solidNum].in_rev_data->Payload[10+i*8];
                                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[5] = appModbusRegedit[in_solidNum].in_rev_data->Payload[11+i*8];
                                    }
                                    for(i = regeditNum;i < MAX_DEVICE_NUM;i++)
                                    {
                                        appModbusRegedit[in_solidNum].deviceList[i].onlineFlag = false;
                                    }                              
                                }                                
                            }                        
                        }
                        else if(pbc_pull_timerIsCompleted(&appModbusRegedit[in_solidNum].send_boardcast_delay))
                        {
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_WAIT_RECEIVE;
                        }              
                        break;
                    }
                    case UN_LOG_ANSWER:
                    {
                        if(pbc_pull_timerIsCompleted(&appModbusRegedit[in_solidNum].answer_delay))
                        {
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_WAIT_LOG_OR_UPDATA;
                        }
                        else
                        {
                            if(pull_bough_link_is_busy(in_solidNum))
                            {//忙碌先释放
                                
                            }
                            else
                            {
                                appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_ANSWER_RESULT; 
                                app_master_slave_send_regedit_data(in_solidNum,&appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[0],(appModbusRegedit[in_solidNum].in_rev_data->Payload[1]|0x80));
                            }     
                        }                          
                        break;
                    }
                    case UN_LOG_ANSWER_RESULT:
                    {
                        if(pull_bough_transmit_monitor(in_solidNum) == bgk_trans_mon_complete)
                        {//发送完成
                            if((appModbusRegedit[in_solidNum].send_cmd == CONTROL_REGEDIT) || (appModbusRegedit[in_solidNum].send_cmd == CONTROL_DETECT))
                            {
                                appModbusRegedit[in_solidNum].mainLinkStatus = STATUS_LOG_TO_FIRST;
                                appModbusRegedit[in_solidNum].firstSubStatus = LOG_FIRST_WAIT_RECEIVE;
                            }
                            else if(appModbusRegedit[in_solidNum].send_cmd == CONTROL_UPDATA)
                            {
                                uint8_t *local_addr;
                                local_addr = app_pull_local_id();
                                if(pull_second_device_id(in_solidNum,&local_addr[0]))
                                {//2号设备
                                    appModbusRegedit[in_solidNum].mainLinkStatus = STATUS_LOG_TO_SECOND;
                                    appModbusRegedit[in_solidNum].secondSubStatus = LOG_SECOND_WAIT_RECEIVE;
                                    pbc_reload_timerClock(&appModbusRegedit[in_solidNum].online_delay,210);
                                }
                                else
                                {
                                    appModbusRegedit[in_solidNum].mainLinkStatus = STATUS_LOG_TO_OTHER;
                                    appModbusRegedit[in_solidNum].ohterSubStatus = LOG_OTHER_WAIT_RECEIVE;
                                    pbc_reload_timerClock(&appModbusRegedit[in_solidNum].online_delay,240);
                                }                  
                            }
                            else
                            {
                                appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_ANSWER;
                            }                                 
                        }
                        else if(pull_bough_transmit_monitor(in_solidNum) == bgk_trans_mon_conflict)
                        {//冲突报文
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_ANSWER;
                        }
                        else if(pull_bough_transmit_monitor(in_solidNum) == bgk_trans_mon_error)
                        {//未知错误
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_ANSWER;
                        }                        
                        break;
                    }
                }             
                break;
            }
            case STATUS_LOG_TO_FIRST:
            {
                switch (appModbusRegedit[in_solidNum].firstSubStatus)
                {
                    case LOG_FIRST_IDLE:
                    {
                        appModbusRegedit[in_solidNum].firstSubStatus = LOG_FIRST_WAIT_RECEIVE;
                        break;
                    }
                    case LOG_FIRST_WAIT_RECEIVE:
                    {
                        if(appModbusRegedit[in_solidNum].receiveOneMessage)
                        {
                            appModbusRegedit[in_solidNum].receiveOneMessage = false;
                            if(pull_boardcast_id(&appModbusRegedit[in_solidNum].in_rev_data->LinkDstAddr[0]))
                            {//接收到广播地址  后上电设备加入列表
                                
                                for(i = 1;i < MAX_DEVICE_NUM;i++)
                                {       
                                    if((appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[0] ==  appModbusRegedit[in_solidNum].deviceList[i].DeviceID[0])&&\
                                    (appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[1] ==  appModbusRegedit[in_solidNum].deviceList[i].DeviceID[1])&&\
                                    (appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[2] ==  appModbusRegedit[in_solidNum].deviceList[i].DeviceID[2])&&\
                                    (appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[3] ==  appModbusRegedit[in_solidNum].deviceList[i].DeviceID[3])&&\
                                    (appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[4] ==  appModbusRegedit[in_solidNum].deviceList[i].DeviceID[4])&&\
                                    (appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[5] ==  appModbusRegedit[in_solidNum].deviceList[i].DeviceID[5]))
                                    {
                                        oldId = i;
                                        newIdFlag = false;
                                    }
                                }
                                if(newIdFlag)
                                {
                                    for(i = 1;i < MAX_DEVICE_NUM;i++)
                                    {
                                        if(appModbusRegedit[in_solidNum].deviceList[i].onlineFlag == false)
                                        {
                                            appModbusRegedit[in_solidNum].deviceList[i].onlineFlag = true;
                                            appModbusRegedit[in_solidNum].deviceList[i].deviceType = pbc_arrayToInt16u_bigEndian(&appModbusRegedit[in_solidNum].in_rev_data->Payload[3]);
                                            appModbusRegedit[in_solidNum].deviceList[i].DeviceID[0] = appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[0];
                                            appModbusRegedit[in_solidNum].deviceList[i].DeviceID[1] = appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[1];
                                            appModbusRegedit[in_solidNum].deviceList[i].DeviceID[2] = appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[2];
                                            appModbusRegedit[in_solidNum].deviceList[i].DeviceID[3] = appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[3];
                                            appModbusRegedit[in_solidNum].deviceList[i].DeviceID[4] = appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[4];
                                            appModbusRegedit[in_solidNum].deviceList[i].DeviceID[5] = appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[5];
                                            break;
                                        }
                                    }
                                }
                                appModbusRegedit[in_solidNum].updataWord = 0;
                                if(!newIdFlag)
                                {
                                    appModbusRegedit[in_solidNum].updataWord |= (0x01 << oldId);
                                }
                                else
                                {
                                    pbc_reload_timerClock(&appModbusRegedit[in_solidNum].updata_list_delay,0);//发送列表
                                }  
                            }
                            else if(pull_local_id(&appModbusRegedit[in_solidNum].in_rev_data->LinkDstAddr[0])) 
                            {//自己地址
                                if((appModbusRegedit[in_solidNum].in_rev_data->Payload[1] == CONTROL_REGEDIT))
                                {//收到注册帧 
                                    for(i = 1;i < MAX_DEVICE_NUM;i++)
                                    {       
                                        if((appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[0] ==  appModbusRegedit[in_solidNum].deviceList[i].DeviceID[0])&&\
                                        (appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[1] ==  appModbusRegedit[in_solidNum].deviceList[i].DeviceID[1])&&\
                                        (appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[2] ==  appModbusRegedit[in_solidNum].deviceList[i].DeviceID[2])&&\
                                        (appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[3] ==  appModbusRegedit[in_solidNum].deviceList[i].DeviceID[3])&&\
                                        (appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[4] ==  appModbusRegedit[in_solidNum].deviceList[i].DeviceID[4])&&\
                                        (appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[5] ==  appModbusRegedit[in_solidNum].deviceList[i].DeviceID[5]))
                                        {
                                            newIdFlag = false;
                                        }
                                    }
                                    if(newIdFlag)
                                    {
                                        for(i = 1;i < MAX_DEVICE_NUM;i++)
                                        {
                                            if(appModbusRegedit[in_solidNum].deviceList[i].onlineFlag == false)
                                            {
                                                appModbusRegedit[in_solidNum].deviceList[i].onlineFlag = true;
                                                appModbusRegedit[in_solidNum].deviceList[i].deviceType = pbc_arrayToInt16u_bigEndian(&appModbusRegedit[in_solidNum].in_rev_data->Payload[3]);
                                                appModbusRegedit[in_solidNum].deviceList[i].DeviceID[0] = appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[0];
                                                appModbusRegedit[in_solidNum].deviceList[i].DeviceID[1] = appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[1];
                                                appModbusRegedit[in_solidNum].deviceList[i].DeviceID[2] = appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[2];
                                                appModbusRegedit[in_solidNum].deviceList[i].DeviceID[3] = appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[3];
                                                appModbusRegedit[in_solidNum].deviceList[i].DeviceID[4] = appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[4];
                                                appModbusRegedit[in_solidNum].deviceList[i].DeviceID[5] = appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[5];
                                                break;
                                            }
                                        }
                                    }
                                    pbc_reload_timerClock(&appModbusRegedit[in_solidNum].updata_list_delay,0);//发送列表
                                }
                                appModbusRegedit[in_solidNum].firstSubStatus = LOG_FIRST_ANSWER;                             
                            }
                        }

                        else if(pbc_pull_timerIsCompleted(&appModbusRegedit[in_solidNum].updata_list_delay))
                        {
                            regeditNum = app_master_slave_pull_device_list_num(in_solidNum);
                            if(regeditNum == 1)
                            {//降级
                                pbc_reload_timerClock(&appModbusRegedit[in_solidNum].send_boardcast_delay,0);
                                appModbusRegedit[in_solidNum].mainLinkStatus = STATUS_UN_LOG;
                                appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_WAIT_RECEIVE;
                            }
                            else
                            {
                                pbc_reload_timerClock(&appModbusRegedit[in_solidNum].updata_list_delay,180);
                                appModbusRegedit[in_solidNum].firstSubStatus = LOG_FIRST_SEND_LIST_ACTIVE; 
								appModbusRegedit[in_solidNum].updataWord = 0;
                                for(i = 1;i < regeditNum;i++)
                                {//形成同步列表,最后加入设备先发
                                    appModbusRegedit[in_solidNum].updataWord |= (0x01<<i);
                                }            
                            }                                    
                        }
                        else if(appModbusRegedit[in_solidNum].updataWord)
                        {
                            appModbusRegedit[in_solidNum].firstSubStatus = LOG_FIRST_SEND_LIST_ACTIVE;
                        }
                        break;
                    }
                    case LOG_FIRST_ANSWER:
                    {
                        if(pull_bough_link_is_busy(in_solidNum))
                        {
                            
                        }
                        else
                        {
                            app_master_slave_send_regedit_data(in_solidNum,&appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[0],(appModbusRegedit[in_solidNum].in_rev_data->Payload[1]|0x80));
                            appModbusRegedit[in_solidNum].firstSubStatus = LOG_FIRST_WAIT_RECEIVE;
                        }
                        break;
                    }
                    case LOG_FIRST_SEND_LIST_ACTIVE:
                    {
                        if(app_link_request_transmit_activation_request(in_solidNum))
                        {
                            appModbusRegedit[in_solidNum].firstSubStatus = LOG_FIRST_SEND_LIST_ACTIVE_RESULT;
                        }  
                        else
                        {
                            appModbusRegedit[in_solidNum].firstSubStatus = LOG_FIRST_WAIT_RECEIVE; 
                        }       
                        break;
                    }
                    case LOG_FIRST_SEND_LIST_ACTIVE_RESULT:
                    {
                        if(TRANSMIT_RESULT_FAIL == app_link_request_get_transmit_result(in_solidNum))
                        {//发送失败
                            appModbusRegedit[in_solidNum].firstSubStatus = LOG_FIRST_SEND_LIST_ACTIVE; 
                        } 
                        else if(TRANSMIT_RESULT_SUCCESS == app_link_request_get_transmit_result(in_solidNum))
                        {//发送成功
                            appModbusRegedit[in_solidNum].firstSubStatus = LOG_FIRST_SEND_LIST; 
                        }   
                        break;
                    }
                    case LOG_FIRST_SEND_LIST:
                    {
                        if(pull_bough_link_is_busy(in_solidNum))
                        {
                            //appModbusRegedit[in_solidNum].firstSubStatus = LOG_FIRST_WAIT_RECEIVE; 
                        }
                        else
                        {
                            regeditNum = app_master_slave_pull_device_list_num(in_solidNum);
                            for(i = (regeditNum - 1);i > 0;i--)
                            {
                                if(appModbusRegedit[in_solidNum].updataWord & (0x01 << i))
                                {
                                    appModbusRegedit[in_solidNum].updataNumber = i;
                                    app_master_slave_send_regedit_data(in_solidNum,&appModbusRegedit[in_solidNum].deviceList[i].DeviceID[0],CONTROL_UPDATA);
                                    appModbusRegedit[in_solidNum].firstSubStatus = LOG_FIRST_SEND_LIST_RESULT;
                                    pbc_reload_timerClock(&appModbusRegedit[in_solidNum].receive_timeout_delay,1000);
                                    break;
                                }
                            }   
                            
                        }
                        break;
                    }
                    case LOG_FIRST_SEND_LIST_RESULT:
                    {
                        if(pull_bough_transmit_monitor(in_solidNum) == bgk_trans_mon_complete)
                        {//发送完成
                            appModbusRegedit[in_solidNum].firstSubStatus = LOG_FIRST_RECEIVE_LIST;
                            pbc_reload_timerClock(&appModbusRegedit[in_solidNum].receive_timeout_delay,1000);                     
                        }
                        else if((pull_bough_transmit_monitor(in_solidNum) == bgk_trans_mon_conflict)||
                            (pull_bough_transmit_monitor(in_solidNum) == bgk_trans_mon_error) )
                        {//冲突报文或者未知错误
                            appModbusRegedit[in_solidNum].firstSubStatus = LOG_FIRST_WAIT_RECEIVE;
                        }
                        break;
                    }
                    case LOG_FIRST_RECEIVE_LIST:
                    {
                        if(appModbusRegedit[in_solidNum].receiveOneMessage)
                        {
                            appModbusRegedit[in_solidNum].receiveOneMessage = false;
                            if(pull_local_id(&appModbusRegedit[in_solidNum].in_rev_data->LinkDstAddr[0])) 
                            {//自己地址
                                if(appModbusRegedit[in_solidNum].in_rev_data->Payload[1] == CONTROL_UPDATA_ACK)
                                {
                                    appModbusRegedit[in_solidNum].updataWord &= (~(0x01<< appModbusRegedit[in_solidNum].updataNumber));
                                    if(appModbusRegedit[in_solidNum].updataWord)
                                    {
                                        appModbusRegedit[in_solidNum].firstSubStatus = LOG_FIRST_SEND_LIST_ACTIVE;
                                    }
                                    else
                                    {
                                        appModbusRegedit[in_solidNum].firstSubStatus = LOG_FIRST_WAIT_RECEIVE;
                                    }     
                                }
                                else if((appModbusRegedit[in_solidNum].in_rev_data->Payload[1] == CONTROL_REGEDIT) ||(appModbusRegedit[in_solidNum].in_rev_data->Payload[1] == CONTROL_DETECT) )
                                {//收到注册帧 
                                    
                                    for(i = 1;i < MAX_DEVICE_NUM;i++)
                                    {       
                                        if((appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[0] ==  appModbusRegedit[in_solidNum].deviceList[i].DeviceID[0])&&\
                                        (appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[1] ==  appModbusRegedit[in_solidNum].deviceList[i].DeviceID[1])&&\
                                        (appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[2] ==  appModbusRegedit[in_solidNum].deviceList[i].DeviceID[2])&&\
                                        (appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[3] ==  appModbusRegedit[in_solidNum].deviceList[i].DeviceID[3])&&\
                                        (appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[4] ==  appModbusRegedit[in_solidNum].deviceList[i].DeviceID[4])&&\
                                        (appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[5] ==  appModbusRegedit[in_solidNum].deviceList[i].DeviceID[5]))
                                        {
                                          //  oldId = i;
                                            newIdFlag = false;
                                        }
                                    }
                                    if(newIdFlag)
                                    {
                                        for(i = 1;i < MAX_DEVICE_NUM;i++)
                                        {
                                            if(appModbusRegedit[in_solidNum].deviceList[i].onlineFlag == false)
                                            {
                                                appModbusRegedit[in_solidNum].deviceList[i].onlineFlag = true;
                                                appModbusRegedit[in_solidNum].deviceList[i].deviceType = pbc_arrayToInt16u_bigEndian(&appModbusRegedit[in_solidNum].in_rev_data->Payload[3]);
                                                appModbusRegedit[in_solidNum].deviceList[i].DeviceID[0] = appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[0];
                                                appModbusRegedit[in_solidNum].deviceList[i].DeviceID[1] = appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[1];
                                                appModbusRegedit[in_solidNum].deviceList[i].DeviceID[2] = appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[2];
                                                appModbusRegedit[in_solidNum].deviceList[i].DeviceID[3] = appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[3];
                                                appModbusRegedit[in_solidNum].deviceList[i].DeviceID[4] = appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[4];
                                                appModbusRegedit[in_solidNum].deviceList[i].DeviceID[5] = appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[5];
                                                break;
                                            }
                                        }
                                    }
                                    appModbusRegedit[in_solidNum].firstSubStatus = LOG_FIRST_WAIT_RECEIVE;
                                    pbc_reload_timerClock(&appModbusRegedit[in_solidNum].updata_list_delay,2);//发送列表
                                    appModbusRegedit[in_solidNum].updataWord = 0;
                                    app_master_slave_send_regedit_data(in_solidNum,&appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[0],(appModbusRegedit[in_solidNum].in_rev_data->Payload[1]|0x80));      
                                }                                                 
                            }
                        }
                        else if(pbc_pull_timerIsCompleted(&appModbusRegedit[in_solidNum].receive_timeout_delay))
                        {
                            appModbusRegedit[in_solidNum].firstSubStatus = LOG_FIRST_RESEND_LIST_ACTIVE;
                        }
                        break;
                    }
                    case LOG_FIRST_RESEND_LIST_ACTIVE:
                    {
                        if(app_link_request_transmit_activation_request(in_solidNum))
                        {
                            appModbusRegedit[in_solidNum].firstSubStatus = LOG_FIRST_RESEND_LIST_ACTIVE_RESULT;
                        }  
    
                        break;
                    }
                    case LOG_FIRST_RESEND_LIST_ACTIVE_RESULT:
                    {
                        if(TRANSMIT_RESULT_FAIL == app_link_request_get_transmit_result(in_solidNum))
                        {//发送失败
                            appModbusRegedit[in_solidNum].firstSubStatus = LOG_FIRST_RESEND_LIST_ACTIVE; 
                        } 
                        else if(TRANSMIT_RESULT_SUCCESS == app_link_request_get_transmit_result(in_solidNum))
                        {//发送成功
                            appModbusRegedit[in_solidNum].firstSubStatus = LOG_FIRST_RESEND_LIST; 
                        }   
                        break;
                    }
                    case LOG_FIRST_RESEND_LIST:
                    {
                        if(pull_bough_link_is_busy(in_solidNum))
                        {
                            
                        }
                        else
                        {
                            regeditNum = app_master_slave_pull_device_list_num(in_solidNum);
                            for(i = (regeditNum - 1);i > 0;i--)
                            {
                                if(appModbusRegedit[in_solidNum].updataWord & (0x01 << i))
                                {
                                    appModbusRegedit[in_solidNum].updataNumber = i;
                                    app_master_slave_send_regedit_data(in_solidNum,&appModbusRegedit[in_solidNum].deviceList[i].DeviceID[0],CONTROL_UPDATA);
                                    appModbusRegedit[in_solidNum].firstSubStatus = LOG_FIRST_RESEND_LIST_RESULT;
                                    pbc_reload_timerClock(&appModbusRegedit[in_solidNum].receive_timeout_delay,RECEIVE_TIMEOUT);
                                    break;
                                }
                            }   
                            
                        }
                        break;
                    }
                    case LOG_FIRST_RESEND_LIST_RESULT:
                    {
                        if(pull_bough_transmit_monitor(in_solidNum) == bgk_trans_mon_complete)
                        {//发送完成
                            appModbusRegedit[in_solidNum].firstSubStatus = LOG_FIRST_RERECEIVE_LIST;
                            pbc_reload_timerClock(&appModbusRegedit[in_solidNum].receive_timeout_delay,1000);                     
                        }
                        else if((pull_bough_transmit_monitor(in_solidNum) == bgk_trans_mon_conflict)||
                            (pull_bough_transmit_monitor(in_solidNum) == bgk_trans_mon_error) )
                        {//冲突报文或者未知错误
                            appModbusRegedit[in_solidNum].firstSubStatus = LOG_FIRST_RESEND_LIST_ACTIVE;
                        }
                        break;
                    }
                    case LOG_FIRST_RERECEIVE_LIST:
                    {
                        if(appModbusRegedit[in_solidNum].receiveOneMessage)
                        {
                            appModbusRegedit[in_solidNum].receiveOneMessage = false;
                            if(pull_local_id(&appModbusRegedit[in_solidNum].in_rev_data->LinkDstAddr[0])) 
                            {//自己地址
                                if(appModbusRegedit[in_solidNum].in_rev_data->Payload[1] == CONTROL_UPDATA_ACK)
                                {
                                    appModbusRegedit[in_solidNum].updataWord &= (~(0x01<< appModbusRegedit[in_solidNum].updataNumber));
                                    if(appModbusRegedit[in_solidNum].updataWord)
                                    {
                                        appModbusRegedit[in_solidNum].firstSubStatus = LOG_FIRST_SEND_LIST_ACTIVE;
                                    }
                                    else
                                    {
                                        appModbusRegedit[in_solidNum].firstSubStatus = LOG_FIRST_WAIT_RECEIVE;
                                    }     
                                }
                                else if((appModbusRegedit[in_solidNum].in_rev_data->Payload[1] == CONTROL_REGEDIT) ||(appModbusRegedit[in_solidNum].in_rev_data->Payload[1] == CONTROL_DETECT) )
                                {//收到注册帧 
                                    for(i = 1;i < MAX_DEVICE_NUM;i++)
                                    {       
                                        if((appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[0] ==  appModbusRegedit[in_solidNum].deviceList[i].DeviceID[0])&&\
                                        (appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[1] ==  appModbusRegedit[in_solidNum].deviceList[i].DeviceID[1])&&\
                                        (appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[2] ==  appModbusRegedit[in_solidNum].deviceList[i].DeviceID[2])&&\
                                        (appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[3] ==  appModbusRegedit[in_solidNum].deviceList[i].DeviceID[3])&&\
                                        (appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[4] ==  appModbusRegedit[in_solidNum].deviceList[i].DeviceID[4])&&\
                                        (appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[5] ==  appModbusRegedit[in_solidNum].deviceList[i].DeviceID[5]))
                                        {
                                            newIdFlag = false;
                                        }
                                    }
                                    if(newIdFlag)
                                    {
                                        for(i = 1;i < MAX_DEVICE_NUM;i++)
                                        {
                                            if(appModbusRegedit[in_solidNum].deviceList[i].onlineFlag == false)
                                            {
                                                appModbusRegedit[in_solidNum].deviceList[i].onlineFlag = true;
                                                appModbusRegedit[in_solidNum].deviceList[i].deviceType = pbc_arrayToInt16u_bigEndian(&appModbusRegedit[in_solidNum].in_rev_data->Payload[3]);
                                                appModbusRegedit[in_solidNum].deviceList[i].DeviceID[0] = appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[0];
                                                appModbusRegedit[in_solidNum].deviceList[i].DeviceID[1] = appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[1];
                                                appModbusRegedit[in_solidNum].deviceList[i].DeviceID[2] = appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[2];
                                                appModbusRegedit[in_solidNum].deviceList[i].DeviceID[3] = appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[3];
                                                appModbusRegedit[in_solidNum].deviceList[i].DeviceID[4] = appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[4];
                                                appModbusRegedit[in_solidNum].deviceList[i].DeviceID[5] = appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[5];
                                                break;
                                            }
                                        }
                                    }
                                    appModbusRegedit[in_solidNum].firstSubStatus = LOG_FIRST_WAIT_RECEIVE;
                                    pbc_reload_timerClock(&appModbusRegedit[in_solidNum].updata_list_delay,2);//发送列表
                                    appModbusRegedit[in_solidNum].updataWord = 0;

                                    app_master_slave_send_regedit_data(in_solidNum,&appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[0],(appModbusRegedit[in_solidNum].in_rev_data->Payload[1]|0x80));      
                                }                                                 
                            }
                        }
                        else if(pbc_pull_timerIsCompleted(&appModbusRegedit[in_solidNum].receive_timeout_delay))
                        {
                            appModbusRegedit[in_solidNum].firstSubStatus = LOG_FIRST_WAIT_RECEIVE;
							appModbusRegedit[in_solidNum].updataWord &= (~(0x01<< appModbusRegedit[in_solidNum].updataNumber));
                            appModbusRegedit[in_solidNum].deviceList[appModbusRegedit[in_solidNum].updataNumber].onlineFlag = false;
                            app_master_slave_updata_local_device_list(in_solidNum);
                            pbc_reload_timerClock(&appModbusRegedit[in_solidNum].updata_list_delay,0);//发送列表
                        }
                        
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
                break;
            }
            case STATUS_LOG_TO_SECOND:
            {
                switch (appModbusRegedit[in_solidNum].secondSubStatus)
                {
                    case LOG_SECOND_IDLE:
                    {
                        appModbusRegedit[in_solidNum].secondSubStatus = LOG_SECOND_WAIT_RECEIVE;
                        break;
                    }
                    case LOG_SECOND_WAIT_RECEIVE:
                    {
                        if(appModbusRegedit[in_solidNum].receiveOneMessage)
                        {
                            appModbusRegedit[in_solidNum].receiveOneMessage = false;
                            if(pull_local_id(&appModbusRegedit[in_solidNum].in_rev_data->LinkDstAddr[0])) 
                            {//自己地址
                                if(appModbusRegedit[in_solidNum].in_rev_data->Payload[1] == CONTROL_UPDATA)
                                {
                                    pbc_reload_timerClock(&appModbusRegedit[in_solidNum].online_delay,210);                                    
                                    uint8_t regeditNum = appModbusRegedit[in_solidNum].in_rev_data->Payload[3];
                                    for(i = 0;i < regeditNum;i++)
                                    {
                                        appModbusRegedit[in_solidNum].deviceList[i].onlineFlag = true;
                                        appModbusRegedit[in_solidNum].deviceList[i].deviceType = pbc_arrayToInt16u_bigEndian(&appModbusRegedit[in_solidNum].in_rev_data->Payload[4+i*8]);
                                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[0] = appModbusRegedit[in_solidNum].in_rev_data->Payload[6+i*8];
                                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[1] = appModbusRegedit[in_solidNum].in_rev_data->Payload[7+i*8];
                                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[2] = appModbusRegedit[in_solidNum].in_rev_data->Payload[8+i*8];
                                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[3] = appModbusRegedit[in_solidNum].in_rev_data->Payload[9+i*8];
                                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[4] = appModbusRegedit[in_solidNum].in_rev_data->Payload[10+i*8];
                                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[5] = appModbusRegedit[in_solidNum].in_rev_data->Payload[11+i*8];
                                    }
                                    for(i = regeditNum;i < MAX_DEVICE_NUM;i++)
                                    {
                                        appModbusRegedit[in_solidNum].deviceList[i].onlineFlag = false;
                                    }
                                    appModbusRegedit[in_solidNum].secondSubStatus = LOG_SECOND_ANSWER;
                                   // app_master_slave_send_regedit_data(in_solidNum,&appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[0],(appModbusRegedit[in_solidNum].in_rev_data->Payload[1]|0x80)); 
                                }
                            }
                            else if(pull_boardcast_id(&appModbusRegedit[in_solidNum].in_rev_data->LinkDstAddr[0]))
                            {//收到1号设备的广播帧  升级为1号
                                if(pull_first_device_id(in_solidNum,&appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[0]))
                                {
                                    app_master_slave_updata_second_device_list(in_solidNum);//更新列表
                                    appModbusRegedit[in_solidNum].mainLinkStatus = STATUS_LOG_TO_FIRST;
                                    appModbusRegedit[in_solidNum].firstSubStatus = LOG_FIRST_WAIT_RECEIVE;   
                                    pbc_reload_timerClock(&appModbusRegedit[in_solidNum].updata_list_delay,0); 
                                }
                            }
                        }
                        else if((pbc_pull_timerIsCompleted(&appModbusRegedit[in_solidNum].online_delay)))
                        {//升级为1号
                            app_master_slave_updata_second_device_list(in_solidNum);//更新列表
                            appModbusRegedit[in_solidNum].mainLinkStatus = STATUS_LOG_TO_FIRST;
                            appModbusRegedit[in_solidNum].firstSubStatus = LOG_FIRST_WAIT_RECEIVE;   
                            pbc_reload_timerClock(&appModbusRegedit[in_solidNum].updata_list_delay,0); 
                        }
                        break;
                    }
                    case LOG_SECOND_ANSWER:
                    {
                        if(pull_bough_link_is_busy(in_solidNum))
                        {
                            
                        }
                        else
                        {
                            app_master_slave_send_regedit_data(in_solidNum,&appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[0],(appModbusRegedit[in_solidNum].in_rev_data->Payload[1]|0x80)); 
                            appModbusRegedit[in_solidNum].secondSubStatus = LOG_SECOND_WAIT_RECEIVE;
                        }
                        
                    }
                    default:
                    {
                        break;
                    }
                }
                break;
            }
            case STATUS_LOG_TO_OTHER:
            {
                switch (appModbusRegedit[in_solidNum].ohterSubStatus)
                {
                    case LOG_OTHER_IDLE:
                    {
                        appModbusRegedit[in_solidNum].ohterSubStatus = LOG_OTHER_WAIT_RECEIVE;
                        break;
                    }
                    case LOG_OTHER_WAIT_RECEIVE:
                    {
                        if(appModbusRegedit[in_solidNum].receiveOneMessage)
                        {
                            appModbusRegedit[in_solidNum].receiveOneMessage = false;
                            if(pull_local_id(&appModbusRegedit[in_solidNum].in_rev_data->LinkDstAddr[0])) 
                            {//自己地址
                                if(appModbusRegedit[in_solidNum].in_rev_data->Payload[1] == CONTROL_UPDATA)
                                {
                                    pbc_reload_timerClock(&appModbusRegedit[in_solidNum].online_delay,240);                             
                                    uint8_t regeditNum = appModbusRegedit[in_solidNum].in_rev_data->Payload[3];
                                    for(i = 0;i < regeditNum;i++)
                                    {
                                        appModbusRegedit[in_solidNum].deviceList[i].onlineFlag = true;
                                        appModbusRegedit[in_solidNum].deviceList[i].deviceType = pbc_arrayToInt16u_bigEndian(&appModbusRegedit[in_solidNum].in_rev_data->Payload[4+i*8]);
                                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[0] = appModbusRegedit[in_solidNum].in_rev_data->Payload[6+i*8];
                                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[1] = appModbusRegedit[in_solidNum].in_rev_data->Payload[7+i*8];
                                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[2] = appModbusRegedit[in_solidNum].in_rev_data->Payload[8+i*8];
                                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[3] = appModbusRegedit[in_solidNum].in_rev_data->Payload[9+i*8];
                                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[4] = appModbusRegedit[in_solidNum].in_rev_data->Payload[10+i*8];
                                        appModbusRegedit[in_solidNum].deviceList[i].DeviceID[5] = appModbusRegedit[in_solidNum].in_rev_data->Payload[11+i*8];
                                    }
                                    for(i = regeditNum;i < MAX_DEVICE_NUM;i++)
                                    {
                                        appModbusRegedit[in_solidNum].deviceList[i].onlineFlag = false;
                                    }
                                    appModbusRegedit[in_solidNum].ohterSubStatus = LOG_OTHER_ANSWER;
                                    //app_master_slave_send_regedit_data(in_solidNum,&appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[0],(appModbusRegedit[in_solidNum].in_rev_data->Payload[1]|0x80));
                                }
                            }
                        }
                        else if((pbc_pull_timerIsCompleted(&appModbusRegedit[in_solidNum].online_delay)))
                        {//降级为未登录
                            appModbusRegedit[in_solidNum].mainLinkStatus = STATUS_UN_LOG;
                            appModbusRegedit[in_solidNum].unlogSubStatus = UN_LOG_WAIT_RECEIVE;
                            pbc_reload_timerClock(&appModbusRegedit[in_solidNum].send_boardcast_delay,0);
                        }
                        break;
                    } 
                    case LOG_OTHER_ANSWER:
                    {
                        if(pull_bough_link_is_busy(in_solidNum))
                        {
                            
                        }
                        else
                        {
                            app_master_slave_send_regedit_data(in_solidNum,&appModbusRegedit[in_solidNum].in_rev_data->LinkSrcAddr[0],(appModbusRegedit[in_solidNum].in_rev_data->Payload[1]|0x80)); 
                            appModbusRegedit[in_solidNum].ohterSubStatus = LOG_OTHER_WAIT_RECEIVE;
                        }
                        
                    }      
                    default:
                    {
                        break;
                    }                
                }          
            }
            default:break;
        }
    } while(mainLinkStatus_backup != appModbusRegedit[in_solidNum].mainLinkStatus);   
}
void app_link_log_push_receive_data(uint8_t in_solidNum,bgk_comm_buff_def *in_rev_data)
{
    appModbusRegedit[in_solidNum].in_rev_data = in_rev_data;
    appModbusRegedit[in_solidNum].receiveOneMessage = true;
    _log_task(in_solidNum);
}

void app_link_log_task(void)
{
    uint8_t i = 0;
    static sdt_bool cfged = sdt_false;
    if(cfged)
    {      
        for(i = 0; i < MAX_MODBUS_NUM;i++)
        {
            _log_task(i);
        }    
    }
    else
    {
        cfged = sdt_true;
        for(i = 0; i < MAX_MODBUS_NUM;i++)
        {
            appModbusRegedit[i].send_wait_delay.timStatusBits = timerType_millisecond;
            appModbusRegedit[i].updata_list_delay.timStatusBits = timerType_second;
            appModbusRegedit[i].send_boardcast_delay.timStatusBits = timerType_second;
            appModbusRegedit[i].online_delay.timStatusBits = timerType_second;
            appModbusRegedit[i].mainLinkStatus = STATUS_IDLE;  
            pbc_reload_timerClock(&appModbusRegedit[i].send_wait_delay, (2000 + GetRandomDelayTime()));       
        }  
//		for(i = 0; i < 6;i++)
//		{
//			appModbusRegedit[0].deviceList[i].onlineFlag = true;
//		}
//		appModbusRegedit[0].deviceList[0].DeviceID[0] = 0xf1;
//		appModbusRegedit[0].deviceList[1].DeviceID[0] = 0xac;
//		appModbusRegedit[0].deviceList[2].DeviceID[0] = 0xf2;
//		appModbusRegedit[0].deviceList[3].DeviceID[0] = 0xf1;
//		appModbusRegedit[0].deviceList[4].DeviceID[0] = 0xac;
//		appModbusRegedit[0].deviceList[5].DeviceID[0] = 0xf2;
//		appModbusRegedit[0].deviceList[2].onlineFlag = false;
//		app_master_slave_updata_local_device_list(0);
    }  
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef app_link_log_H
#define app_link_log_H
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//----------------------------------------------------------------------------
#define ID_NUM            6
//-----------------------------------------------------------------------------
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef struct
{
    bool     onlineFlag;           //在线标志
    uint16_t deviceType;           //设备类型
    uint8_t  DeviceID[ID_NUM];     //ID
}linkDeviceList_t;
//-----------------------------------------------------------------------------
#define PROTOL_CHECK_CONFICT     0xD020  //报文冲突检测
#define PROTOL_SYN               0xD021  //数据同步
#define PROTOL_REGEDIT           0xD022  //报文冲突检测
#define PROTOL_VERSION           0x01  //协议版本
#define CONTROL_REGEDIT          0x01//注册申请组网
#define CONTROL_REGEDIT_ACK      0x81//注册应答
#define CONTROL_UPDATA           0x02//更新
#define CONTROL_UPDATA_ACK       0x82//更新应答

#define CONTROL_DETECT           0x03//申请加入当前网络
#define CONTROL_DETECT_ACK       0x83//申请加入当前网络应答

#define CMD_SYN_00               0x00//主动同步          无需应答
#define CMD_SYN_01               0x01//主动同步          需要应答
#define CMD_SYN_81               0x81//应答同步带数据    无需应答
#define CMD_SYN_41               0x41//应答同步不带数据  无需应答


#define RECEIVE_TIMEOUT          1000
#define ROLL_BACK_DELAY          3
#define ANSWER_DELAY             8
#define SEND_BOARDCAST_DELAY     10
#define SEND_BOARDCAST_DELAY_MAX  50
#define DETECT_DELAY             3000
#define REQUST_NET_DELAY         300
//-----------------------------------------------------------------------------
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//interface list  4
1:注册任务:调度器使用         
void app_link_log_task(void);  

2:网络在线设备列表
linkDeviceList_t* app_link_log_pull_device_list(uint8_t solidNum);

3:本地在线接口
true 在线
bool app_link_log_pull_local_device_online(uint8_t solidNum);

4:数据压入接口:调度器使用
void app_link_log_push_receive_data(uint8_t in_solidNum,bgk_comm_buff_def *in_rev_data);
--------------------------------E N D-----------------------------------------*/
//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++*/
//序号: 1  
//名称: app_link_log_task
//功能: 注册列表维护:调度器使用
//入口: 无                           
//出口: 无
void app_link_log_task(void);
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++*/
//序号: 2 
//名称: app_link_log_pull_device_list
//功能: 获取网络在线设备列表
//入口: in_solidNum 对应实例                           
//出口: linkDeviceList_t* 列表指针地址
linkDeviceList_t* app_link_log_pull_device_list(uint8_t in_solidNum);
//------------------------------E N D-------------------------------------------


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++*/
//序号: 3
//名称: app_link_log_pull_local_device_online
//功能: 获取本地在线状态
//入口: in_solidNum 对应实例                               
//出口: true在线
bool app_link_log_pull_local_device_online(uint8_t in_solidNum);
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++*/
//序号: 4 
//名称: app_link_log_push_receive_data
//功能: 数据压入接口:调度器使用
//入口: in_solidNum 对应实例   in_rev_data  接收到的数据                       
//出口: 无
void app_link_log_push_receive_data(uint8_t in_solidNum,bgk_comm_buff_def *in_rev_data);
//------------------------------E N D-------------------------------------------
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#endif
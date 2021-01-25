#ifndef med_MASTER_SLAVE_H
#define med_MASTER_SLAVE_H
//-----------------------------------------------------------------------------
#ifndef snail_data_types
    #include ".\depend\snail_data_types.h"
#endif
//-----------------------------------------------------------------------------
#define SYSTEM_MASTER     0
#define MAX_BUFF_SIZE    256
#define PROTOL_CHECK_CONFICT     0xD020  //报文冲突检测
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
typedef enum
{
    msRunS_idle             = 0x00,
    msRunS_phyBusy,
    msRunS_transmit_str,
    msRunS_transmit_respose_str,
    msRunS_transmit_wait,
    msRunS_transmit_data,
    msRunS_transmit_stop, 
    msRunS_transmit_end,
    msRunS_conflict_wait,
    msRunS_receive_wait,
    msRunS_receive_data,
    msRunS_receive_end,   
}modbus_master_slave_runState_def;

typedef enum 
{
    mRtuS_master_slave_none       = 0x00,
    mRtuS_master_slave_complete   = 0x01,
    mRtuS_master_slave_timeout    = 0x02,
    mRtuS_master_slave_poterr     = 0x03,
}mRtu_master_slave_status_def;

typedef enum 
{
    msRtu_cmd_none    = 0x00,
    msRtu_cmd_03      = 0x01,
    msRtu_cmd_06      = 0x02,
    msRtu_cmd_10      = 0x03,
    msRtu_cmd_17      = 0x04,
}mRtu_cmd_status_def;

//-----------------------------------------------------------------------------
typedef enum
{
    mRtu_master_slave_parity_none  = 0x00,
    mRtu_master_slave_parity_even  = 0x01,
    mRtu_master_slave_parity_odd   = 0x02,
}mRtu_master_slave_parity_def;
//-----------------------------------------------------------------------------
typedef enum
{
    mRtu_master_slave_stopBits_one = 0x00,
    mRtu_master_slave_stopBits_two = 0x01,
}mRtu_master_slave_stopBits_def;
//-----------------------------------------------------------------------------
//typedef struct mde_mRtu_master
//{
//    sdt_int32u                 mRtu_baudrate;
//    sdt_int32u                 mRtu_sysFrequency;
//    mRtu_master_slave_parity_def     mRtu_parity;
//    mRtu_master_slave_stopBits_def   mRtu_stopBits;
//}mRtu_master_slave_parameter_def;

typedef struct
{
    sdt_int16u ProcotolType;    //协议类型
    sdt_int16u PayloadLength;   //数据长度  
    sdt_bool   responseFlag;
    union
    {
        struct
        {
            sdt_int8u  LinkDstAddr[6];  //目标地址
            sdt_int8u  LinkSrcAddr[6];  //源地址
            sdt_int8u  Payload[MAX_BUFF_SIZE-12];    //数据
        };
        sdt_int8u raw_payload[MAX_BUFF_SIZE];
    };
}ms_link_comm_data_def;
//++++++++++++++++++++++++++++++++++task+++++++++++++++++++++++++++++++++++++++
//name:mRtu 主站任务
//-----------------------------------------------------------------------------
void mde_rtu_master_slave_task(void);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------
//name:获取modbus register 读取的地址和长度
//写读状态时，获取读取的首地址和长度
//in: in_solidNum实例号,从0开始,*out_reg_addr 地址指针，*out_length 长度指针
//out: 
//name:获取主站接收状态
//发送数据后等待响应结果
//in: in_solidNum实例号,从0开始,
//out: mRtu_master_status_def 正常响应/超时/帧错误
mRtu_master_slave_status_def mde_msRtu_master_slave_reveive_status(sdt_int8u in_solidNum);    
//name:获取主站运行状态
//确定主站当前运行状态
//in: in_solidNum实例号,从0开始,
//out: modbus_master_runState_def 空闲或者其它忙碌状态
modbus_master_slave_runState_def mde_msRtu_master_slave_run_status(sdt_int8u in_solidNum);    
ms_link_comm_data_def mde_pull_msRtu_onemessage(sdt_int8u in_solidNum,sdt_bool* out_HaveMessage);
void mde_master_slave_push_link_onemessage(sdt_int8u in_solidNum,ms_link_comm_data_def* in_pTransmitData);
sdt_bool mde_msRtu_master_slave_phy_idle_status(sdt_int8u in_solidNum);
//void mde_msRtu_master_slave_push_idle_status(sdt_int8u in_solidNum);  
void mde_master_slave_link_message_transmit(sdt_int8u in_solidNum,sdt_int8u *in_dest_addr,sdt_int16u in_protolType,sdt_bool resposeFlag);//固定18个字节
void mde_msRtu_master_slave_push_mRtu_local_id(sdt_int8u in_solidNum,sdt_int8u* in_id);
void mde_master_slave_link_message_check_conflict(sdt_int8u in_solidNum,sdt_int16u inType);//发送冲突侦测帧
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#endif
//空闲状态下发送03/06/10/17报文
//等待相应结果
//根据结果处理数据或者重发数据
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
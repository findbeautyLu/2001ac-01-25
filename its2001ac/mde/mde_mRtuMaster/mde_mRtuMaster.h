#ifndef med_mRtuMaster_H
#define med_mRtuMaster_H
//-----------------------------------------------------------------------------
#ifndef snail_data_types
    #include ".\depend\snail_data_types.h"
#endif
//-----------------------------------------------------------------------------
#define MASTER_SENSOR    1
#define MASTER_COMP      0
#define MAX_BUFF_SIZE    256
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
typedef enum
{
    mmRunS_idle             = 0x00,
    mmRunS_transmit_str,
    mmRunS_transmit_35T,
    mmRunS_transmit_data,
    mmRunS_transmit_stop,    
    mmRunS_transmit_end,
    mmRunS_receive_wait,
    mmRunS_receive_data,
    mmRunS_receive_end,
    
}modbus_master_runState_def;

typedef enum 
{
    mRtuS_master_none       = 0x00,
    mRtuS_master_complete   = 0x01,
    mRtuS_master_timeout    = 0x02,
    mRtuS_master_poterr     = 0x03,
}mRtu_master_status_def;
//-----------------------------------------------------------------------------
typedef enum
{
    mRtu_master_parity_none  = 0x00,
    mRtu_master_parity_even  = 0x01,
    mRtu_master_parity_odd   = 0x02,
}mRtu_master_parity_def;
//-----------------------------------------------------------------------------
typedef enum
{
    mRtu_master_stopBits_one = 0x00,
    mRtu_master_stopBits_two = 0x01,
}mRtu_master_stopBits_def;
//-----------------------------------------------------------------------------
typedef struct mde_mRtu_master
{
    sdt_int32u                 mRtu_baudrate;
    sdt_int32u                 mRtu_sysFrequency;
    mRtu_master_parity_def     mRtu_parity;
    mRtu_master_stopBits_def   mRtu_stopBits;
}mRtu_master_parameter_def;

//++++++++++++++++++++++++++++++++++task+++++++++++++++++++++++++++++++++++++++
//name:mRtu 主站任务
//-----------------------------------------------------------------------------
void mde_mRtu_master_task(void);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------
//name:获取modbus register 读取的地址和长度
//写读状态时，获取读取的首地址和长度
//in: in_solidNum实例号,从0开始,*out_reg_addr 地址指针，*out_length 长度指针
//out: 
void mde_pull_mRtu_register_add_len(sdt_int8u in_solidNum,sdt_int16u* out_reg_addr,sdt_int16u* out_length); 
//name:获取主站接收状态
//发送数据后等待响应结果
//in: in_solidNum实例号,从0开始,
//out: mRtu_master_status_def 正常响应/超时/帧错误
mRtu_master_status_def mde_mRtu_master_reveive_status(sdt_int8u in_solidNum);    
//name:获取主站运行状态
//确定主站当前运行状态
//in: in_solidNum实例号,从0开始,
//out: modbus_master_runState_def 空闲或者其它忙碌状态
modbus_master_runState_def mde_mRtu_master_run_status(sdt_int8u in_solidNum);    

//name:modbus读取命令
//fun:把modbus需要读取的数据，推入modbus模块
//in: in_solidNum实例号,从0开始,in_reg_addr 寄存器地址，in_length 长度，*in_pRegDetails 寄存器内容
//out: sdt_true 推入成功,sdt_false 推入失败
//-----------------------------------------------------------------------------
sdt_bool mde_push_mRtu_master_sendReg(sdt_int8u in_solidNum,sdt_int16u in_reg_addr,sdt_int16u in_RegDetails);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//name:modbus写入命令
//fun:从模块中获取需要写入的寄存器内容
//in: in_solidNum实例号,从0开始，in_reg_addr 寄存器地址，*out_pRregDetails 寄存器内容指针
//out: sdt_true 获取成功,sdt_false 获取失败
//-----------------------------------------------------------------------------
sdt_bool mde_pull_mRtu_receiveReg(sdt_int8u in_solidNum,sdt_int16u in_reg_addr,sdt_int16u* out_pRregDetails);
//++++++++++++++++++++++++++++++++interface++++++++++++++++++++++++++++++++++++
//name:发送0x03命令
//主机发送一次0x03命令的报文
//in:  in_solidNum    --实例号，
//     in_slave_addr  --从机地址，
//     in_reg_addr    --读取的寄存器首地址，
//     in_reg_length  --读取的寄存器长度
//-----------------------------------------------------------------------------
void mde_mRtu_master_cmd0x03_transmit(sdt_int8u in_solidNum,sdt_int8u in_slave_addr,\
                                      sdt_int16u in_reg_addr,sdt_int16u in_reg_length);
void mde_mRtu_master_cmd0x04_transmit(sdt_int8u in_solidNum,sdt_int8u in_slave_addr,\
                                      sdt_int16u in_reg_addr,sdt_int16u in_reg_length);
//++++++++++++++++++++++++++++++++interface++++++++++++++++++++++++++++++++++++
//name:发送0x06命令
//主机发送一次0x06命令的报文
//in:  in_solidNum    --实例号，
//     in_slave_addr  --从机地址，
//     in_reg_addr    --写入的寄存器首地址，
//     in_reg_data    --写入的寄存器数据
void mde_mRtu_master_cmd0x06_transmit(sdt_int8u in_solidNum,sdt_int8u in_slave_addr,\
                                      sdt_int16u in_reg_addr,sdt_int16u in_reg_data);
//++++++++++++++++++++++++++++++++interface++++++++++++++++++++++++++++++++++++
//name:发送0x10命令
//主机发送一次0x10命令的报文需配合mde_push_mRtu_master_sendReg写入相应的数据
//in:  in_solidNum    --实例号，
//     in_slave_addr  --从机地址，
//     in_reg_addr    --写入的寄存器首地址，
//     in_reg_length  --写入的寄存器长度

void mde_mRtu_master_cmd0x10_transmit(sdt_int8u in_solidNum,sdt_int8u in_slave_addr,\
                                      sdt_int16u in_reg_addr,sdt_int16u in_reg_length);  
//++++++++++++++++++++++++++++++++interface++++++++++++++++++++++++++++++++++++
//name:发送0x17命令
//主机发送一次0x17命令的报文需配合mde_push_mRtu_master_sendReg写入相应的数据
//in:  in_solidNum    --实例号，
//     in_slave_addr  --从机地址，
//     in_reg_read_addr    --读取的寄存器首地址，
//     in_reg_read_length  --读取的寄存器长度
//     in_reg_write_addr    --写入的寄存器首地址，
//     in_reg_write_length  --写入的寄存器长度
void mde_mRtu_master_cmd0x17_transmit(sdt_int8u in_solidNum,sdt_int8u in_slave_addr,\
                                      sdt_int16u in_reg_read_addr,sdt_int16u in_reg_read_length,\
                                      sdt_int16u in_reg_write_addr,sdt_int16u in_reg_write_length);                                                                                                  
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#endif
//空闲状态下发送03/06/10/17报文
//等待相应结果
//根据结果处理数据或者重发数据
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
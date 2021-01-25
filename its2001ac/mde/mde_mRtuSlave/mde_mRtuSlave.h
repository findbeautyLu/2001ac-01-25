#ifndef med_mRtuSlave_H
#define mde_mRtuSlave_H
//-----------------------------------------------------------------------------
#ifndef snail_data_types
    #include ".\depend\snail_data_types.h"
#endif
//-----------------------------------------------------------------------------
#define  MODBUS_ONEWIRE       0
#define  MODBUS_REMOTE        1

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
typedef enum 
{
    mRtuS_none    = 0x00,
    mRtuS_read    = 0x01,
    mRtuS_write   = 0x02,
    mRtuS_rwBoth  = 0x03,
}mRtu_status_def;
//-----------------------------------------------------------------------------
typedef enum
{
    mRtu_parity_none  = 0x00,
    mRtu_parity_even  = 0x01,
    mRtu_parity_odd   = 0x02,
}mRtu_parity_def;
//-----------------------------------------------------------------------------
typedef enum
{
    mRtu_stopBits_one = 0x00,
    mRtu_stopBits_two = 0x01,
}mRtu_stopBits_def;
//-----------------------------------------------------------------------------
typedef struct mde_mRtuSlave
{
    sdt_int8u           mRtu_address;
    sdt_int32u          mRtu_baudrate;
    sdt_int32u          mRtu_sysFrequency;
    mRtu_parity_def     mRtu_parity;
    mRtu_stopBits_def   mRtu_stopBits;
}mRtu_parameter_def;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//name:mRtu任务
//-----------------------------------------------------------------------------
void mde_mRtu_task(void);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//name:设置modbus参数
//in: in_solidNum实例号,从0开始
//-----------------------------------------------------------------------------
void set_mRtu_parameter(sdt_int8u in_solidNum,mRtu_parameter_def* in_parameter);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//name:获取modbus register 当前状态
//获取modbus模块的状态，
//读状态，指针获取读寄存器首地址和寄存器长度
//写状态，指针获取写入的寄存器首地址和长度
//写读状态，指针获取读寄存器首地址和寄存器长度
//in: in_solidNum实例号,从0开始,*out_reg_addr 地址指针，*out_length 长度指针
//out: mRtuS_none 无,mRtuS_read 读数据,mRtuS_write 写数据
//     mRtuS_rwBoth 写读数据，指针获取读的内容
//-----------------------------------------------------------------------------
mRtu_status_def pull_mRtu_register(sdt_int8u in_solidNum,sdt_int16u* out_reg_addr,sdt_int16u* out_length);
//-----------------------------------------------------------------------------
//name:获取modbus register 写入的内容
//写读状态时，获取写入的首地址和长度
//in: in_solidNum实例号,从0开始,*out_reg_addr 地址指针，*out_length 长度指针
//out: 
//-----------------------------------------------------------------------------
void pull_mRtu_register_wb(sdt_int8u in_solidNum,sdt_int16u* out_reg_addr,sdt_int16u* out_length);
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//name:modbus读取命令
//fun:把modbus需要读取的数据，推入modbus模块
//in: in_solidNum实例号,从0开始,in_reg_addr 寄存器地址，in_length 长度，*in_pRegDetails 寄存器内容
//out: sdt_true 推入成功,sdt_false 推入失败
//-----------------------------------------------------------------------------
sdt_bool push_mRtu_readReg(sdt_int8u in_solidNum,sdt_int16u in_reg_addr,sdt_int16u in_RegDetails);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//name:modbus写入命令
//fun:从模块中获取需要写入的寄存器内容
//in: in_solidNum实例号,从0开始，in_reg_addr 寄存器地址，*out_pRregDetails 寄存器内容指针
//out: sdt_true 获取成功,sdt_false 获取失败
//-----------------------------------------------------------------------------
sdt_bool pull_mRtu_writeReg(sdt_int8u in_solidNum,sdt_int16u in_reg_addr,sdt_int16u* out_pRregDetails);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//name:应答事件
//fun:modbus总线开始应答
//in: in_solidNum实例号,从0开始
//-----------------------------------------------------------------------------
void mRtu_answer_event(sdt_int8u in_solidNum);
void mRtu_remote_answer_event(sdt_int8u in_solidNum,sdt_int8u *in_buff,sdt_int16u length);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#endif
//报文接收完毕
//读取命令
//推送读取地址和长度到接口
//app代码推入数据到接口
//应答报文

//写入命令
//推送读取地址和长度到接口
//app代码获取写入数据
//应答报文
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
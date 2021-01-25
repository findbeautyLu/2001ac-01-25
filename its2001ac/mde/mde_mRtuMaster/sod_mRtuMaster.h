#ifndef sod_mRtuMaster_H
#define sod_mRtuMaster_H
//-----------------------------------------------------------------------------
#ifndef snail_data_types
    #include ".\depend\snail_data_types.h"
#endif
#include ".\depend\bsp_mRtuMaster.h"
//-----------------------------------------------------------------------------
//+++++++++++++++++++++++++++++++solid++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define max_solid    2
//--------------------------------------------------------------------------------------------------------------------------
static modbus_master_oper_def modbus_master_solid[max_solid];
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
static void modbus_master_solid_cfg(void)
{
//--------------------------------------------------------------------------------------------------------------------------
    bsp_uart5_cfg();
    modbus_master_solid[0].pull_receive_byte = bsp_pull_oneByte_uart5_rxd;
    modbus_master_solid[0].push_transmit_byte = bsp_push_oneByte_uart5_txd;
    modbus_master_solid[0].pull_busFree = bsp_uart5_busFree;
    modbus_master_solid[0].restart_busFree_timer = bsp_restart_tim6;
    modbus_master_solid[0].phy_into_receive = bps_uart5_into_receive;
    modbus_master_solid[0].phy_into_transmit_status = bps_uart5_into_transmit;
    modbus_master_solid[0].pull_transmit_complete =bsp_pull_uart5_txd_cmp;
    
    bsp_uart2_cfg();
    modbus_master_solid[1].pull_receive_byte = bsp_pull_oneByte_uart2_rxd;
    modbus_master_solid[1].push_transmit_byte = bsp_push_oneByte_uart2_txd;
    modbus_master_solid[1].pull_busFree = bsp_uart2_busFree;
    modbus_master_solid[1].restart_busFree_timer = bsp_restart_tim2;
    modbus_master_solid[1].phy_into_receive = bps_uart2_into_receive;
    modbus_master_solid[1].phy_into_transmit_status = bps_uart2_into_transmit;
    modbus_master_solid[1].pull_transmit_complete =bsp_pull_uart2_txd_cmp;
//---------------------------------------------------------------------------------------------------------------------------
}                                                                                            
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#endif
//空闲状态下发送03/06/10/17报文
//等待相应结果
//根据结果处理数据或者重发数据
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
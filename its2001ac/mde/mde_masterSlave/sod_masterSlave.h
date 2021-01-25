#ifndef sod_MASTER_SLAVE_H
#define sod_MASTER_SLAVE_H
//-----------------------------------------------------------------------------
#ifndef snail_data_types
    #include ".\depend\snail_data_types.h"
#endif
#include ".\depend\bsp_masterSlave.h"
//-----------------------------------------------------------------------------
//+++++++++++++++++++++++++++++++solid++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define max_solid    1
//--------------------------------------------------------------------------------------------------------------------------
static modbus_master_slave_oper_def modbus_master_slave_solid[max_solid];
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
static void modbus_master_slave_solid_cfg(void)
{
//--------------------------------------------------------------------------------------------------------------------------
//    bsp_ms_uart5_cfg();
//    modbus_master_slave_solid[0].pull_receive_byte = bsp_ms_pull_oneByte_uart5_rxd;
//    modbus_master_slave_solid[0].push_transmit_byte = bsp_ms_push_oneByte_uart5_txd;
//    modbus_master_slave_solid[0].phy_into_receive = bps_ms_uart5_into_receive;
//    modbus_master_slave_solid[0].phy_into_transmit_status = bps_ms_uart5_into_transmit;
//    modbus_master_slave_solid[0].pull_transmit_complete =bsp_ms_pull_uart5_txd_cmp;
//    modbus_master_slave_solid[0].bsp_ms_check_phy_bus = bsp_ms_check_uart5_phy_bus;
//    modbus_master_slave_solid[0].bsp_ms_pull_phyBus_status = bsp_ms_pull_uart5_phyBus_status;
//    modbus_master_slave_solid[0].bsp_ms_pull_rxd_num = bsp_ms_pull_uart5_rxd_num;
    
    bsp_uart4_cfg();
    modbus_master_slave_solid[0].pull_receive_byte = bsp_pull_oneByte_uart4_rxd;
    modbus_master_slave_solid[0].push_transmit_byte = bsp_push_oneByte_uart4_txd;
    modbus_master_slave_solid[0].phy_into_receive = bps_uart4_into_receive;
    modbus_master_slave_solid[0].phy_into_transmit_status = bps_uart4_into_transmit;
    modbus_master_slave_solid[0].pull_transmit_complete =bsp_pull_uart4_txd_cmp;
    modbus_master_slave_solid[0].bsp_ms_check_phy_bus = bsp_ms_check_uart4_phy_bus;
    modbus_master_slave_solid[0].bsp_ms_pull_phyBus_status = bsp_ms_pull_uart4_phyBus_status;
    modbus_master_slave_solid[0].bsp_ms_pull_rxd_num = bsp_ms_pull_uart4_rxd_num;
//    bsp_ms_uart2_cfg();
//    modbus_master_slave_solid[1].pull_receive_byte = bsp_ms_pull_oneByte_uart2_rxd;
//    modbus_master_slave_solid[1].push_transmit_byte = bsp_ms_push_oneByte_uart2_txd;
//    modbus_master_slave_solid[1].phy_into_receive = bps_ms_uart2_into_receive;
//    modbus_master_slave_solid[1].pull_transmit_complete =bsp_ms_pull_uart2_txd_cmp;
//---------------------------------------------------------------------------------------------------------------------------
}                                                                                            
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#endif
//空闲状态下发送03/06/10/17报文
//等待相应结果
//根据结果处理数据或者重发数据
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
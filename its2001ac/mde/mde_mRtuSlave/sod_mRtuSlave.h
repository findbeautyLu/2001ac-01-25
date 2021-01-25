#ifndef med_mRtuSlave_H
#define mde_mRtuSlave_H
//-----------------------------------------------------------------------------
#ifndef snail_data_types
    #include ".\depend\snail_data_types.h"
#endif
#include ".\depend\bsp_mRtuSlave.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
//+++++++++++++++++++++++++++++++solid+++++++++++++++++++++++++++++++++++++++++
#define max_solid    2
//-----------------------------------------------------------------------------
static modbus_oper_def modbus_oper_solid[max_solid];
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
static void modbus_solid_cfg(void)
{
//-----------------------------------------------------------------------------
    bsp_uart1_cfg();
    modbus_oper_solid[0].pull_receive_byte = bsp_pull_oneByte_uart1_rxd;
    modbus_oper_solid[0].push_transmit_byte = bsp_push_oneByte_uart1_txd;
    modbus_oper_solid[0].pull_busFree = bsp_uart1_busFree;
    modbus_oper_solid[0].restart_busFree_timer = bsp_restart_tim1;
    modbus_oper_solid[0].phy_into_receive = bps_uart1_into_receive;
    modbus_oper_solid[0].phy_into_transmit_status = bps_uart1_into_transmit;
    modbus_oper_solid[0].pull_transmit_complete =bsp_pull_uart1_txd_cmp;
    
    bsp_uart3_cfg();
    modbus_oper_solid[1].pull_receive_byte = bsp_pull_oneByte_uart3_rxd;
    modbus_oper_solid[1].push_transmit_byte = bsp_push_oneByte_uart3_txd;
    modbus_oper_solid[1].pull_busFree = bsp_uart3_busFree;
    modbus_oper_solid[1].restart_busFree_timer = bsp_restart_tim4;
    modbus_oper_solid[1].phy_into_receive = bps_uart3_into_receive;
    modbus_oper_solid[1].phy_into_transmit_status = bps_uart3_into_transmit;
    modbus_oper_solid[1].pull_transmit_complete =bsp_pull_uart3_txd_cmp;
    
//    bsp_uart4_cfg();
//    modbus_oper_solid[1].pull_receive_byte = bsp_pull_oneByte_uart4_rxd;
//    modbus_oper_solid[1].push_transmit_byte = bsp_push_oneByte_uart4_txd;
//    modbus_oper_solid[1].pull_busFree = bsp_uart4_busFree;
//    modbus_oper_solid[1].restart_busFree_timer = bsp_restart_tim3;
//    modbus_oper_solid[1].phy_into_receive = bps_uart4_into_receive;
//    modbus_oper_solid[1].phy_into_transmit_status = bps_uart4_into_transmit;
//    modbus_oper_solid[1].pull_transmit_complete =bsp_pull_uart4_txd_cmp;
    
//
//-----------------------------------------------------------------------------
}
#endif
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
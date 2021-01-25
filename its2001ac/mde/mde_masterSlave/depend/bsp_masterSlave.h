//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef bsp_MASTER_SLAVE_H
#define bsp_MASTER_SLAVE_H
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
#ifndef snail_data_types
    #include ".\snail_data_types.h"
#endif
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------
void bsp_uart4_cfg(void);
sdt_bool bsp_pull_oneByte_uart4_rxd(sdt_int8u* out_byte_details);
sdt_bool bsp_push_oneByte_uart4_txd(sdt_int8u in_byte_details);
//sdt_bool bsp_uart4_busFree(sdt_int8u t_char_dis);
//void bsp_restart_tim3(void);
sdt_bool bsp_pull_uart4_txd_cmp(void);
void bps_uart4_into_receive(void);
void bps_uart4_into_transmit(sdt_bool in_weekPullUp);
void bsp_ms_check_uart4_phy_bus(void);//总线是否空闲
sdt_bool bsp_ms_pull_uart4_phyBus_status(void);//获取总线状态
sdt_int8u bsp_ms_pull_uart4_rxd_num(void);//接收数量


void bsp_ms_uart5_cfg(void);
sdt_bool bsp_ms_pull_oneByte_uart5_rxd(sdt_int8u* out_byte_details);
sdt_bool bsp_ms_push_oneByte_uart5_txd(sdt_int8u in_byte_details);
//sdt_bool bsp_ms_uart5_busFree(sdt_int8u t_char_dis);
//void bsp_ms_restart_tim6(void);
sdt_bool bsp_ms_pull_uart5_txd_cmp(void);
void bps_ms_uart5_into_receive(void);
void bps_ms_uart5_into_transmit(sdt_bool in_weekPullUp);
void bsp_ms_check_uart5_phy_bus(void);//总线是否空闲
sdt_bool bsp_ms_pull_uart5_phyBus_status(void);//获取总线状态
sdt_int8u bsp_ms_pull_uart5_rxd_num(void);//接收数量
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#endif 
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
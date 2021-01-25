//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#include "stm32f10x.h"
#include ".\BSP_relay.h"
//------------------------------E N D-------------------------------------------
//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++ 
//#define RELAY1_DATA    T_BIT3  
//#define RELAY2_DATA    T_BIT2      
//#define RELAY3_DATA    T_BIT1   
//#define RELAY4_DATA    T_BIT0
//#define RELAY5_DATA    T_BIT15   
//#define RELAY6_DATA    T_BIT14      
//#define RELAY7_DATA    T_BIT13   
//#define RELAY8_DATA    T_BIT12 
//#define RELAY9_DATA    T_BIT11   
//#define RELAY10_DATA   T_BIT10      
//#define RELAY11_DATA   T_BIT9   
//#define RELAY12_DATA   T_BIT8 

#define RELAY1_DATA    3  
#define RELAY2_DATA    2      
#define RELAY3_DATA    1   
#define RELAY4_DATA    0
#define RELAY5_DATA    15   
#define RELAY6_DATA    14      
#define RELAY7_DATA    13   
#define RELAY8_DATA    12 
#define RELAY9_DATA    11   
#define RELAY10_DATA   10      
#define RELAY11_DATA   9   
#define RELAY12_DATA   8 
void BSP_Relay1_Enable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData |= RELAY1_DATA;
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(RELAY1_DATA,true);
}

void BSP_Relay2_Enable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData |= RELAY2_DATA;
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(RELAY2_DATA,true);
}
void BSP_Relay3_Enable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData |= RELAY3_DATA;
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(RELAY3_DATA,true);
}
void BSP_Relay4_Enable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData |= RELAY4_DATA;
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(RELAY4_DATA,true);
}
void BSP_Relay5_Enable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData |= RELAY5_DATA;
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(RELAY5_DATA,true);
}
void BSP_Relay6_Enable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData |= RELAY6_DATA;
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(RELAY6_DATA,true);
}
void BSP_Relay7_Enable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData |= RELAY7_DATA;
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(RELAY7_DATA,true);
}
void BSP_Relay8_Enable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData |= RELAY8_DATA;
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(RELAY8_DATA,true);
}
void BSP_Relay9_Enable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData |= RELAY9_DATA;
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(RELAY9_DATA,true);
}
void BSP_Relay10_Enable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData |= RELAY10_DATA;
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(RELAY10_DATA,true);
}
void BSP_Relay11_Enable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData |= RELAY11_DATA;
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(RELAY11_DATA,true);
}
void BSP_Relay12_Enable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData |= RELAY12_DATA;
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(RELAY12_DATA,true);
}

void BSP_Relay1_Disable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData &= (~RELAY1_DATA);
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(RELAY1_DATA,false);
}
void BSP_Relay2_Disable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData &= (~RELAY2_DATA);
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(RELAY2_DATA,false);
}
void BSP_Relay3_Disable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData &= (~RELAY3_DATA);
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(RELAY3_DATA,false);
}
void BSP_Relay4_Disable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData &= (~RELAY4_DATA);
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(RELAY4_DATA,false);
}
void BSP_Relay5_Disable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData &= (~RELAY5_DATA);
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(RELAY5_DATA,false);
}
void BSP_Relay6_Disable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData &= (~RELAY6_DATA);
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(RELAY6_DATA,false);
}
void BSP_Relay7_Disable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData &= (~RELAY7_DATA);
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(RELAY7_DATA,false);
}
void BSP_Relay8_Disable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData &= (~RELAY8_DATA);
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(RELAY8_DATA,false);
}
void BSP_Relay9_Disable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData &= (~RELAY9_DATA);
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(RELAY9_DATA,false);
}
void BSP_Relay10_Disable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData &= (~RELAY10_DATA);
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(RELAY10_DATA,false);
}
void BSP_Relay11_Disable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData &= (~RELAY11_DATA);
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(RELAY11_DATA,false);
}
void BSP_Relay12_Disable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData &= (~RELAY12_DATA);
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(RELAY12_DATA,false);
}
//------------------------------E N D-------------------------------------------
//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
//**************************IO口配置推挽****************************************

//-----------------------BSP_RelayPWM.c--END------------------------------------

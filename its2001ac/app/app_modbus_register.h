#ifndef app_modbus_register_H
#define app_modbus_register_H
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include ".\app_cfg.h"
//----------------------------------------------------------------------------
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define MRegaddr_FactoryParameter                        0x1500   //恢复出厂设置,0x2591安全字
//-----------------------------------------------------------------------------
#define MRegaddr_SNWriteCtlHi                            0x1600   //控制字
#define MRegaddr_SNWriteCtlLo                            0x1601
#define MRegaddr_CiphersVerCtl                           0x1602   //密文版本控制   
#define MRegaddr_AllowWrSTxtD                            0x1603   //允许写入标志 0xFFFF允许写入
#define MRegaddr_STxtEncodeType                          0x1604   //编码类型
#define MRegaddr_STxtBytesLength                         0x1605   //标识字节长度
#define MRegaddr_STxtDetailsStart                        0x1606   //标识内容开始地址

#define STXTDETAILSREGAMOUNT                             64       //标识最大支持128byte
//-----------------------------------------------------------------------------URL
#define MRegaddr_URLBytesLength                          0x1700
#define MRegaddr_URLDetailsStart                         0x1701   //URL开始地址

#define URLDETAILSAOUNT                                  254
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define MRegaddr_StrUpdateWord                           0x1800
#define MRegaddr_UpdateVersion                           0x1801
#define MRegaddr_MessageTnumber                          0x1802
#define MRegaddr_CheckSumHI                              0x1805
#define MRegaddr_CheckSumLo                              0x1806                                    
#define MRegaddr_SlaveStatusW                            0x1810
#define MRegaddr_RequestNumber                           0x1811
#define MRegaddr_HardwareTypeHi                          0x1812
#define MRegaddr_HardwareTypeLo                          0x1813                                     
#define MRegaddr_MessageNumber                           0x187F  //129 register
#define MRegaddr_Message0                                0x1880
#define MRegaddr_Message63                               0x18BF
#define MRegaddr_STM32F103R8T6WRPRHi                     0x1900   //加密字
#define MRegaddr_STM32F103R8T6WRPRLo                     0x1901
#define MRegaddr_STM32F103R8T6OBRHi                      0x1902   //状态字
#define MRegaddr_STM32F103R8T6OBRLo                      0x1903
#define MRegaddr_EraseWRPR                               0x1904   //擦除加密字
//-----------------------------------------------------------------------------
#define MRegaddr_ProductType                             0x2000   //产品类型
#define MRegaddr_SoftVersion                             0x2001   //软件版本
#define MRegaddr_SecondAndWeek                           0x2002   //时钟 秒-星期
#define MRegaddr_HourAndMinute                           0x2003   //时钟 小时-分钟
#define MRegaddr_MonthAndDay                             0x2004   //时钟 月-天
#define MRegaddr_Year                                    0x2005   //时钟 年
#define MRegaddr_SalveAddr                               0x2006   //从机地址
#define MRegaddr_SalveBaudrate                           0x2007   //从机波特率 2400 4800 9600
#define MRegaddr_SalveParityBit                          0x2008   //从机奇偶校验位
#define MRegaddr_Restoragefac                            0x200F   //
//-----------------------------------------------------------------------------
#define REG_ADDR_MASTER_ERROR_WORD                       0X2C00
#define REG_ADDR_MASTER_RESET_WORD                       0X2C01
#define REG_ADDR_SYSTEM_POWER                            0X2C02
#define REG_ADDR_SYSTEM_RUN_MODE                         0X2C03
#define REG_ADDR_FAN_SET_HUM                             0X2C04
#define REG_ADDR_FAN_SET_SPEED                           0X2C05
#define REG_ADDR_SET_COLD_TEMP                           0X2C06
#define REG_ADDR_SET_HEAT_TEMP                           0X2C07
#define REG_ADDR_NEW_AIR_LOW_PWM                         0X2C08
#define REG_ADDR_NEW_AIR_MID_PWM                         0X2C09
#define REG_ADDR_NEW_AIR_HIGH_PWM                        0X2C0A
#define REG_ADDR_BACK_AIR_LOW_PWM                        0X2C0B
#define REG_ADDR_BACK_AIR_MID_PWM                        0X2C0C
#define REG_ADDR_BACK_AIR_HIGH_PWM                       0X2C0D
#define REG_ADDR_MASTER_SET_COLD_WATER_TEMP              0X2C0E
#define REG_ADDR_MASTER_SET_HEAT_WATER_TEMP              0X2C0F
#define REG_ADDR_MASTER_SET_LIVING_WATER_TEMP            0X2C10
#define REG_ADDR_MASTER_OUTDOOR_IN_FAN_TEMP              0X2C11
#define REG_ADDR_MASTER_OUTDOOR_IN_FAN_HUM               0X2C12
#define REG_ADDR_BEFORE_FU_TEMP                          0X2C13
#define REG_ADDR_BEFORE_FU_HUM                           0X2C14
#define REG_ADDR_AFTER_FU_TEMP                           0X2C15
#define REG_ADDR_AFTER_FU_HUM                            0X2C16
#define REG_ADDR_BACK_AIR_TEMP                           0X2C17
#define REG_ADDR_BACK_AIR_HUM                            0X2C18
#define REG_ADDR_EXHAST_AIR_TEMP                         0X2C19
#define REG_ADDR_EXHAST_AIR_HUM                          0X2C1A
#define REG_ADDR_BACK_AIR_CO2                            0X2C1B
#define REG_ADDR_BACK_AIR_PM25                           0X2C1C
#define REG_ADDR_DEADZONE_FAN_HUM                        0X2C1D
#define REG_ADDR_DEADZONE_FAN_TEMP                       0X2C1E
#define REG_ADDR_NEW_AIR_SPEED                           0X2C1F
#define REG_ADDR_BACK_AIR_SPEED                          0X2C20
#define REG_ADDR_WATER_MACHINE_FRE                       0X2C21
#define REG_ADDR_FU_MACHINE_FRE                          0X2C22
#define REG_ADDR_MASTER_SUPPLY_TEMP                      0X2C23
#define REG_ADDR_MASTER_LIVING_TEMP                      0X2C24
#define REG_ADDR_MASTER_OUTDOOR_TEMP                     0X2C25
#define REG_ADDR_ADJUST_OUTDOORAIR_HUM                   0X2C2A
#define REG_ADDR_ADJUST_OUTDOORAIR_TEMP                  0X2C2B
#define REG_ADDR_ADJUST_OUTDOORAIR_CO2                   0X2C2C
#define REG_ADDR_ADJUST_OUTDOORAIR_PM25                  0X2C2D
#define REG_ADDR_ADJUST_BEFORFU_HUM                      0X2C2E
#define REG_ADDR_ADJUST_BEFORFU_TEMP                     0X2C2F
#define REG_ADDR_ADJUST_BEFORFU_CO2                      0X2C30
#define REG_ADDR_ADJUST_BEFORFU_PM25                     0X2C31
#define REG_ADDR_ADJUST_AFTERFU_HUM                      0X2C32
#define REG_ADDR_ADJUST_AFTERFU_TEMP                     0X2C33
#define REG_ADDR_ADJUST_AFTERFU_CO2                      0X2C34
#define REG_ADDR_ADJUST_AFTERFU_PM25                     0X2C35
#define REG_ADDR_ADJUST_BACKAIR_HUM                      0X2C36
#define REG_ADDR_ADJUST_BACKAIR_TEMP                     0X2C37
#define REG_ADDR_ADJUST_BACKAIR_CO2                      0X2C38
#define REG_ADDR_ADJUST_BACKAIR_PM25                     0X2C39
#define REG_ADDR_ADJUST_EXHASTAIR_HUM                    0X2C3A
#define REG_ADDR_ADJUST_EXHASTAIR_TEMP                   0X2C3B
#define REG_ADDR_ADJUST_EXHASTAIR_CO2                    0X2C3C
#define REG_ADDR_ADJUST_EXHASTAIR_PM25                   0X2C3D
#define REG_ADDR_SET_MIXWATER_COLD_TEMP                  0X2C3E
#define REG_ADDR_SET_MIXWATER_HEAT_TEMP                  0X2C3F
#define REG_ADDR_MIXWATER_INWATER_TEMP                   0X2C40
#define REG_ADDR_MIXWATER_BACKWATER_TEMP                 0X2C41
#define REG_ADDR_MIXWATER_TEMP                           0X2C42
#define REG_ADDR_OUTPUT_PUMP                             0X2C43
#define REG_ADDR_OUTPUT_THREE_VAVLE                      0X2C44
#define REG_ADDR_MIXWATER_AI_DEST_TEMP                   0X2C45
#define REG_ADDR_MIXWATER_AI_ENABLE                      0X2C46
#define REG_ADDR_MIXWATER_LOW_TEMP_PROTECT               0X2C47


#define REG_ADRESS_COOL_BACKWIND_SETTEMP				 0x2c4c//制冷回风设置温度
#define REG_ADRESS_HOT_BACKWIND_SETTEMP					 0x2c4d//制热回风设置温度
#define REG_ADRESS_COOL_OUTWATER_SETTEMP				 0x2c4e//制冷出水设置温度
#define REG_ADRESS_COOL_BACKWATER_SETTEMP				 0x2c4f//制冷回水设置温度
#define REG_ADRESS_HOT_OUTWATER_SETTEMP					 0x2c50//制热出水设置温度
#define REG_ADRESS_HOT_BACKWATER_SETTEMP				 0x2c51//制热回水设置温度

#define DP_ADDR_START                                    0X0100
#define DP_ADDR_SYSTEM_POWER                             0X0100
#define DP_ADDR_SYSTEM_RUN_MODE                          0X0101
#define DP_ADDR_SYSTEM_WIND_SET_CLOD_TEMP                0X0102
#define DP_ADDR_SYSTEM_WIND_SET_HEAT_TEMP                0X0103
#define DP_ADDR_SYSTEM_WIND_SET_HUM                      0X0104
#define DP_ADDR_SYSTEM_WIND_SET_SPEED                    0X0105
#define DP_ADDR_SYSTEM_WATER_SET_CLOD_TEMP               0X0106
#define DP_ADDR_SYSTEM_WATER_SET_HEAT_TEMP               0X0107
#define DP_ADDR_SYSTEM_WATER_SET_LIVING_TEMP             0X0108
#define DP_ADDR_SYSTEM_MIXWATER_SET_CLOD_TEMP            0X0109
#define DP_ADDR_SYSTEM_MIXWATER_SET_HEAT_TEMP            0X010A
#define DP_ADDR_SYSTEM_MIXWATER_AI_ENABLE                0X010B
#define DP_ADDR_SYSTEM_MIXWATER_LOW_TEMP_PROTECT         0X010C
#define DP_ADDR_SYSTEM_MIXWATER_AI_DEST_TEMP             0X010D
#define DP_ADDR_SYSTEM_LIS_NEW_AIR_PWM                   0X010E
#define DP_ADDR_SYSTEM_LIS_BACK_AIR_PWM                  0X010F
#define DP_ADDR_SYSTEM_LIS_OUTDOOR_WEATHER               0X0110
#define DP_ADDR_SYSTEM_LIS_BEFORE_FU_WEATHER             0X0111
#define DP_ADDR_SYSTEM_LIS_AFTER_FU_WEATHER              0X0112
#define DP_ADDR_SYSTEM_LIS_BACK_AIR_WEATHER              0X0113
#define DP_ADDR_SYSTEM_LIS_EXHAST_AIR_WEATHER            0X0114
#define DP_ADDR_SYSTEM_LIS_DEAD_ZONE                     0X0115
#define DP_ADDR_SYSTEM_LIS_OUT_VALVE                     0X0116
#define DP_ADDR_SYSTEM_LIS_OUT_STATUS                    0X0117
#define DP_ADDR_SYSTEM_LIS_MIXWATER_OUT                  0X0118
#define DP_ADDR_SYSTEM_LIS_MAIN_MACHINE_TEMP             0X0119
#define DP_ADDR_SYSTEM_LIS_MIXWATER_TEMP                 0X011A
#define DP_ADDR_SYSTEM_LIS_RTC                           0X011B
#define DP_ADDR_SYSTEM_LIS_485_PAREMETER                 0X011C

#define DEVICE_TYPE                                      0x8521
#define DEVICE_VERSION                                   0x2001
#define DEVICE_TYPE_HC                                   0x8521
#define DEVICE_TYPE_CC                                   0x8522
#define DEVICE_TYPE_FAN                                  0x9521
#define DEVICE_TYPE_ROMM                                 0x9522
//-----------------------------------------------------------------------------
uint16_t app_modbus_read_reg_data(uint16_t reg_addr);
bool app_modebus_write_reg_data(uint16_t regAddress,uint16_t reg_detailes);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#endif

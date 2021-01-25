#ifndef app_master_sensor_H
#define app_master_sensor_H
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include ".\app_cfg.h"
//----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define  SENSOR_OUTDOOR    0
#define  SENSOR_BEFOREFU   1
#define  SENSOR_AFTERFU    2
#define  SENSOR_BACKAIR    3
#define  SENSOR_EXHASTAIR  4
//-----------------------------------------------------------------------------
void app_master_sensor_task(void);
int16_t Pull_RSS1001H_Temperature(uint8_t in_soildNum);
uint16_t Pull_RSS1001H_Humidity(uint8_t in_soildNum);
uint16_t Pull_RSS1001H_CO2(uint8_t in_soildNum);
uint16_t Pull_RSS1001H_PM25(uint8_t in_soildNum);
bool Pull_RSS1001H_OnlineState(uint8_t in_soildNum);
void Enable_OncePM25Measure(uint8_t in_soildNum);

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#endif
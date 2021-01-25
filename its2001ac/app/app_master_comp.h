#ifndef app_master_comp_H
#define app_master_comp_H
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define DRIVE_BOARD_SET_MODE           		5001
#define DRIVE_BOARD_FORCE_SET_MODE          5002
#define DRIVE_BOARD_POWER_ON_OFF          	5003

#define DRIVE_BOARD_COOL_SET_TEMP      		5007
#define DRIVE_BOARD_HOT_SET_TEMP          	5008
#define DRIVE_BOARD_HUMIDITY_SET_TEMP       5009
#define DRIVE_BOARD_LIFE_HOTWATER_SET_TEMP  5010

#define DRIVE_BOARD_COOL_OUTWATER_SETTEMP   5024
#define DRIVE_BOARD_COOL_BACKWATER_SETTEMP  5025
#define DRIVE_BOARD_HOT_OUTWATER_SETTEMP    5026
#define DRIVE_BOARD_HOT_BACKWATER_SETTEMP   5027

#define DRIVE_BOARD_TWO_WAY_VALUE_STATE		5042
#define DRIVE_BOARD_INDOOR_MESSAGE			5047

#define UPDATA_DELAY                           1000
//----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void app_master_comp_task(void);
bool APP_pull_aricod_message(uint16_t in_reg_add,uint16_t *out_reg_value);
bool APP_push_aricod_message(uint16_t in_reg_add,uint16_t in_reg_value);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#endif

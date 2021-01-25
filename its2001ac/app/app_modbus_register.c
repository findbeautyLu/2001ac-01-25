//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include ".\app_cfg.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//name: 应用task
//-----------------------------------------------------------------------------
uint16_t app_modbus_read_reg_data(uint16_t reg_addr)
{
   uint16_t reg_detailes = 0;
    //uint16_t Temp16u = 0;
    if((reg_addr >= 0x3A01) && (reg_addr <= (0x3A01+67)))
    {
        APP_pull_aricod_message((reg_addr-0x3a01+1),&reg_detailes);
    }
    else if((reg_addr >= 0x3b01) && (reg_addr <= (0x3b01+52)))
    {
        APP_pull_aricod_message((reg_addr-0x3b01+5001),&reg_detailes);
    }
    else
    {
        switch(reg_addr)
        {
            case MRegaddr_ProductType:
            {
                reg_detailes = DEVICE_TYPE;
                break;
            }
            case MRegaddr_SoftVersion:
            {
                reg_detailes = DEVICE_VERSION;
                break;
            }
            case  MRegaddr_SecondAndWeek:
            {
                realTime_t* currentRtc;
                uint8_t buff[2];
                currentRtc = app_real_time_pull_rtc();
                buff[0] = currentRtc->second;
                buff[1] = currentRtc->week;
                reg_detailes = pbc_arrayToInt16u_bigEndian(&buff[0]);
                break;
            }
            case  MRegaddr_HourAndMinute:
            {
                realTime_t* currentRtc;
                uint8_t buff[2];
                currentRtc = app_real_time_pull_rtc();
                buff[0] = currentRtc->hour;
                buff[1] = currentRtc->minute;
                reg_detailes = pbc_arrayToInt16u_bigEndian(&buff[0]);
                break;
            }
            case  MRegaddr_MonthAndDay:
            {
                realTime_t* currentRtc;
                uint8_t buff[2];
                currentRtc = app_real_time_pull_rtc();
                buff[0] = currentRtc->month;
                buff[1] = currentRtc->day;
                reg_detailes = pbc_arrayToInt16u_bigEndian(&buff[0]);
                break;
            }
            case  MRegaddr_Year:
            {
                realTime_t* currentRtc;
                currentRtc = app_real_time_pull_rtc();          
                reg_detailes = (currentRtc->year + 2000);
                break;
            }
        
            case REG_ADDR_MASTER_ERROR_WORD:
            {
                reg_detailes = app_general_pull_master_error_word();
                break;
            }
            case REG_ADDR_MASTER_RESET_WORD:
            {
                reg_detailes = 0;
                break;
            }
            case REG_ADDR_SYSTEM_POWER:
            {
                reg_detailes = app_general_pull_power_status();
                break;
            }
            case REG_ADDR_SYSTEM_RUN_MODE:
            {
                reg_detailes = app_general_pull_aircod_mode();
                break;
            }
            case REG_ADDR_FAN_SET_HUM:
            {
                reg_detailes = app_general_pull_aircod_humidity();
                break;
            }
            case REG_ADDR_FAN_SET_SPEED:
            {
                reg_detailes = app_general_pull_aircod_fanSpeed();
                break;
            }
            case REG_ADDR_SET_COLD_TEMP:
            {
                reg_detailes = app_general_pull_aircod_cold_temp();
                break;
            }
            case REG_ADDR_SET_HEAT_TEMP:
            {
                reg_detailes = app_general_pull_aircod_heat_temp();
                break;
            }
            case REG_ADDR_NEW_AIR_LOW_PWM:
            {
                reg_detailes = app_general_pull_new_air_pwm_low();
                break;
            }
            case REG_ADDR_NEW_AIR_MID_PWM:
            {
                reg_detailes = app_general_pull_new_air_pwm_mid();
                break;
            }
            case REG_ADDR_NEW_AIR_HIGH_PWM:
            {
                reg_detailes = app_general_pull_new_air_pwm_high();
                break;
            }
            case REG_ADDR_BACK_AIR_LOW_PWM:
            {
                reg_detailes = app_general_pull_back_air_pwm_low();
                break;
            }
            case REG_ADDR_BACK_AIR_MID_PWM:
            {
                reg_detailes = app_general_pull_back_air_pwm_mid();
                break;
            }
            case REG_ADDR_BACK_AIR_HIGH_PWM:
            {
                reg_detailes = app_general_pull_back_air_pwm_high();
                break;
            }
            case REG_ADDR_MASTER_SET_COLD_WATER_TEMP:
            {
                reg_detailes = app_general_pull_set_cold_water_temp();
                break;
            }
            case REG_ADDR_MASTER_SET_HEAT_WATER_TEMP:
            {
                reg_detailes = app_general_pull_set_heat_water_temp();
                break;
            }
            case REG_ADDR_MASTER_SET_LIVING_WATER_TEMP:
            {
                reg_detailes = app_general_pull_set_living_water_temp();
                break;
            }
            case REG_ADDR_MASTER_OUTDOOR_IN_FAN_TEMP:
            {
                reg_detailes = Pull_RSS1001H_Temperature(SENSOR_OUTDOOR);
                break;
            }
            case REG_ADDR_MASTER_OUTDOOR_IN_FAN_HUM:
            {
                reg_detailes = Pull_RSS1001H_Humidity(SENSOR_OUTDOOR);
                break;
            }
            case REG_ADDR_BEFORE_FU_TEMP:
            {
                reg_detailes = Pull_RSS1001H_Temperature(SENSOR_BEFOREFU);
                break;
            }
            case REG_ADDR_BEFORE_FU_HUM:
            {
                reg_detailes = Pull_RSS1001H_Humidity(SENSOR_BEFOREFU);
                break;
            }
            case REG_ADDR_AFTER_FU_TEMP:
            {
                reg_detailes = Pull_RSS1001H_Temperature(SENSOR_AFTERFU);
                break;
            }
            case REG_ADDR_AFTER_FU_HUM:
            {
                reg_detailes = Pull_RSS1001H_Humidity(SENSOR_AFTERFU);
                break;
            }
            case REG_ADDR_BACK_AIR_TEMP:
            {
                reg_detailes = Pull_RSS1001H_Temperature(SENSOR_BACKAIR);
                break;
            }
            case REG_ADDR_BACK_AIR_HUM:
            {
                reg_detailes = Pull_RSS1001H_Humidity(SENSOR_BACKAIR);
                break;
            }
            case REG_ADDR_EXHAST_AIR_TEMP:
            {
                reg_detailes = Pull_RSS1001H_Temperature(SENSOR_EXHASTAIR);
                break;
            }
            case REG_ADDR_EXHAST_AIR_HUM:
            {
                reg_detailes = Pull_RSS1001H_Humidity(SENSOR_EXHASTAIR);
                break;
            }
            case REG_ADDR_BACK_AIR_CO2:
            {
                reg_detailes = Pull_RSS1001H_CO2(SENSOR_BACKAIR);
                break;
            }
            case REG_ADDR_BACK_AIR_PM25:
            {
                reg_detailes = Pull_RSS1001H_PM25(SENSOR_BACKAIR);
                break;
            }
            case REG_ADDR_DEADZONE_FAN_HUM:
            {
                reg_detailes = app_general_pull_aircod_setHum_deadZone();
                break;
            }
            case REG_ADDR_DEADZONE_FAN_TEMP:
            {
                reg_detailes = app_general_pull_aircod_setTemp_deadZone();
                break;
            }
            case REG_ADDR_NEW_AIR_SPEED:
            {
                reg_detailes = app_general_pull_aircod_newair_speed();
                break;
            }
            case REG_ADDR_BACK_AIR_SPEED:
            {
                reg_detailes = app_general_pull_aircod_newair_speed();
                break;
            }
            case REG_ADDR_WATER_MACHINE_FRE:
            {
                reg_detailes = app_general_pull_water_machine_fre();
                break;
            }
            case REG_ADDR_FU_MACHINE_FRE:
            {
                reg_detailes = app_general_pull_fu_machine_fre();
                break;
            }
            case REG_ADDR_MASTER_SUPPLY_TEMP:
            {
                reg_detailes = app_general_pull_master_supply_temp();
                break;
            }
            case REG_ADDR_MASTER_LIVING_TEMP:
            {
                reg_detailes = app_general_pull_master_living_temp();
                break;
            }
            case REG_ADDR_MASTER_OUTDOOR_TEMP:
            {
                reg_detailes = app_general_pull_master_outdoor_temp();
                break;
            }
            case REG_ADDR_ADJUST_OUTDOORAIR_HUM:
            {
                reg_detailes = app_general_pull_adjust_outdoor_hum();
                break;
            }
            case REG_ADDR_ADJUST_OUTDOORAIR_TEMP:
            {
                reg_detailes = app_general_pull_adjust_outdoor_temp();
                break;
            }
            case REG_ADDR_ADJUST_OUTDOORAIR_CO2:
            {
                reg_detailes = app_general_pull_adjust_outdoor_co2();
                break;
            }
            case REG_ADDR_ADJUST_OUTDOORAIR_PM25:
            {
                reg_detailes = app_general_pull_adjust_outdoor_pm25();
                break;
            }
            case REG_ADDR_ADJUST_BEFORFU_HUM:
            {
                reg_detailes = app_general_pull_adjust_beforfu_hum();
                break;
            }
            case REG_ADDR_ADJUST_BEFORFU_TEMP:
            {
                reg_detailes = app_general_pull_adjust_beforfu_temp();
                break;
            }
            case REG_ADDR_ADJUST_BEFORFU_CO2:
            {
                reg_detailes = app_general_pull_adjust_beforfu_co2();
                break;
            }
            case REG_ADDR_ADJUST_BEFORFU_PM25:
            {
                reg_detailes = app_general_pull_adjust_beforfu_pm25();
                break;
            }
            case REG_ADDR_ADJUST_AFTERFU_HUM:
            {
                reg_detailes = app_general_pull_adjust_afterfu_hum();
                break;
            }
            case REG_ADDR_ADJUST_AFTERFU_TEMP:
            {
                reg_detailes = app_general_pull_adjust_afterfu_temp();
                break;
            }
            case REG_ADDR_ADJUST_AFTERFU_CO2:
            {
                reg_detailes = app_general_pull_adjust_afterfu_co2();
                break;
            }
            case REG_ADDR_ADJUST_AFTERFU_PM25:
            {
                reg_detailes = app_general_pull_adjust_afterfu_pm25();
                break;
            }
            case REG_ADDR_ADJUST_BACKAIR_HUM:
            {
                reg_detailes = app_general_pull_adjust_backair_hum();
                break;
            }
            case REG_ADDR_ADJUST_BACKAIR_TEMP:
            {
                reg_detailes = app_general_pull_adjust_backair_temp();
                break;
            }
            case REG_ADDR_ADJUST_BACKAIR_CO2:
            {
                reg_detailes = app_general_pull_adjust_backair_co2();
                break;
            }
            case REG_ADDR_ADJUST_BACKAIR_PM25:
            {
                reg_detailes = app_general_pull_adjust_backair_pm25();
                break;
            }
            case REG_ADDR_ADJUST_EXHASTAIR_HUM:
            {
                reg_detailes = app_general_pull_adjust_exhastair_hum();
                break;
            }
            case REG_ADDR_ADJUST_EXHASTAIR_TEMP:
            {
                reg_detailes = app_general_pull_adjust_exhastair_temp();
                break;
            }
            case REG_ADDR_ADJUST_EXHASTAIR_CO2:
            {
                reg_detailes = app_general_pull_adjust_exhastair_co2();
                break;
            }
            case REG_ADDR_ADJUST_EXHASTAIR_PM25:
            {
                reg_detailes = app_general_pull_adjust_exhastair_pm25();
                break;
            }
            case REG_ADDR_SET_MIXWATER_COLD_TEMP:
            {
                reg_detailes = app_general_pull_set_mixwater_cold_temp();
                break;
            }
            case REG_ADDR_SET_MIXWATER_HEAT_TEMP:
            {
                reg_detailes = app_general_pull_set_mixwater_heat_temp();
                break;
            }
            case REG_ADDR_MIXWATER_INWATER_TEMP:
            {
                reg_detailes = app_general_pull_in_water_measure_temp();
                break;
            }
            case REG_ADDR_MIXWATER_BACKWATER_TEMP:
            {
                reg_detailes = app_general_pull_back_water_measure_temp();
                break;
            }
            case REG_ADDR_MIXWATER_TEMP:
            {
                reg_detailes = app_general_pull_mix_water_measure_temp();
                break;
            }
            case REG_ADDR_OUTPUT_PUMP:
            {
                reg_detailes = app_general_pull_pump_output();
                break;
            }
            case REG_ADDR_OUTPUT_THREE_VAVLE:
            {
                reg_detailes = app_general_pull_three_vavle_output();
                break;
            }
            case REG_ADDR_MIXWATER_AI_DEST_TEMP:
            {
                reg_detailes = app_general_pull_ai_dest_temp();
                break;
            }
            case REG_ADDR_MIXWATER_AI_ENABLE:
            {
                reg_detailes = app_general_pull_ai_enable_status();
                break;
            }
            case REG_ADDR_MIXWATER_LOW_TEMP_PROTECT:
            {
                reg_detailes = app_general_pull_mix_freeze_protect();
                break;
            }
			case REG_ADRESS_COOL_BACKWIND_SETTEMP  : 
				reg_detailes = app_general_pull_coolbackwind_set_temp();
				break;
			case REG_ADRESS_HOT_BACKWIND_SETTEMP   : 
				reg_detailes = app_general_pull_hotbackwind_set_temp();
				break;
			case REG_ADRESS_COOL_OUTWATER_SETTEMP  : 
				reg_detailes = app_general_pull_cooloutwater_set_temp();
				break;
			case REG_ADRESS_HOT_OUTWATER_SETTEMP   : 
				reg_detailes = app_general_pull_hotoutwater_set_temp();
				break;
			case REG_ADRESS_COOL_BACKWATER_SETTEMP : 
				reg_detailes = app_general_pull_coolbackwater_set_temp();
				break;
			case REG_ADRESS_HOT_BACKWATER_SETTEMP  : 
				reg_detailes = app_general_pull_hotbackwater_set_temp();
				break;
            default:
            {
                reg_detailes = 0;
                break;
            }
        }
    }
    return reg_detailes;
}

bool app_modebus_write_reg_data(uint16_t regAddress,uint16_t reg_detailes)
{
    switch(regAddress)
    {
        case  MRegaddr_SecondAndWeek:
        {
            realTime_t* currentRtc;
			currentRtc = app_real_time_pull_rtc();
            uint8_t buff[2];
            pbc_int16uToArray_bigEndian(reg_detailes,&buff[0]);
            currentRtc->second = buff[0];
            currentRtc->week = buff[1];
            app_push_once_save_write_rtc();
            break;
        }
        case  MRegaddr_HourAndMinute:
        {
            realTime_t* currentRtc;
			currentRtc = app_real_time_pull_rtc();
            uint8_t buff[2];
            pbc_int16uToArray_bigEndian(reg_detailes,&buff[0]);
            currentRtc->hour = buff[0];
            currentRtc->minute = buff[1];
           // app_real_time_push_rtc(currentRtc);
            app_push_once_save_write_rtc();
            break;
        }
        case  MRegaddr_MonthAndDay:
        {
            realTime_t* currentRtc;
			currentRtc = app_real_time_pull_rtc();
            uint8_t buff[2];
            pbc_int16uToArray_bigEndian(reg_detailes,&buff[0]);
            currentRtc->month = buff[0];
            currentRtc->day = buff[1];
          //  app_real_time_push_rtc(currentRtc);
            app_push_once_save_write_rtc();
            break;
        }
        case  MRegaddr_Year:
        {
            realTime_t* currentRtc;
			currentRtc = app_real_time_pull_rtc();
            currentRtc->year = (reg_detailes-2000);
           // app_real_time_push_rtc(currentRtc);
            app_push_once_save_write_rtc();
            break;
        }
       
        case REG_ADDR_MASTER_ERROR_WORD:
        {
            app_general_push_master_error_word(reg_detailes);
            break;
        }
        case REG_ADDR_MASTER_RESET_WORD:
        {
            //reg_detailes = 0;
            break;
        }
        case REG_ADDR_SYSTEM_POWER:
        {
            app_general_push_power_status(reg_detailes);
            break;
        }
        case REG_ADDR_SYSTEM_RUN_MODE:
        {
            app_general_push_aircod_mode((AirRunMode_Def)reg_detailes);
            break;
        }
        case REG_ADDR_FAN_SET_HUM:
        {
            app_general_push_aircod_humidity(reg_detailes);
            break;
        }
        case REG_ADDR_FAN_SET_SPEED:
        {
            app_general_push_aircod_fanSpeed((NewAirLevelSet_Def)reg_detailes);
            break;
        }
        case REG_ADDR_SET_COLD_TEMP:
        {
            app_general_push_aircod_cold_temp(reg_detailes);
            break;
        }
        case REG_ADDR_SET_HEAT_TEMP:
        {
            app_general_push_aircod_heat_temp(reg_detailes);
            break;
        }
        case REG_ADDR_NEW_AIR_LOW_PWM:
        {
            app_general_push_new_air_pwm_low(reg_detailes);
            break;
        }
        case REG_ADDR_NEW_AIR_MID_PWM:
        {
            app_general_push_new_air_pwm_mid(reg_detailes);
            break;
        }
        case REG_ADDR_NEW_AIR_HIGH_PWM:
        {
            app_general_push_new_air_pwm_high(reg_detailes);
            break;
        }
        case REG_ADDR_BACK_AIR_LOW_PWM:
        {
            app_general_push_back_air_pwm_low(reg_detailes);
            break;
        }
        case REG_ADDR_BACK_AIR_MID_PWM:
        {
            app_general_push_back_air_pwm_mid(reg_detailes);
            break;
        }
        case REG_ADDR_BACK_AIR_HIGH_PWM:
        {
            app_general_push_back_air_pwm_high(reg_detailes);
            break;
        }
        case REG_ADDR_MASTER_SET_COLD_WATER_TEMP:
        {
            app_general_push_set_cold_water_temp(reg_detailes);
            break;
        }
        case REG_ADDR_MASTER_SET_HEAT_WATER_TEMP:
        {
            app_general_push_set_heat_water_temp(reg_detailes);
            break;
        }
        case REG_ADDR_MASTER_SET_LIVING_WATER_TEMP:
        {
            app_general_push_set_living_water_temp(reg_detailes);
            break;
        }
        case REG_ADDR_MASTER_OUTDOOR_IN_FAN_TEMP:
        {
            app_general_push_outdoor_temp(reg_detailes);
            break;
        }
        case REG_ADDR_MASTER_OUTDOOR_IN_FAN_HUM:
        {
            app_general_push_outdoor_hum(reg_detailes);
            break;
        }
        case REG_ADDR_BEFORE_FU_TEMP:
        {
            app_general_push_before_fu_temp(reg_detailes);
            break;
        }
        case REG_ADDR_BEFORE_FU_HUM:
        {
            app_general_push_before_fu_hum(reg_detailes);
            break;
        }
        case REG_ADDR_AFTER_FU_TEMP:
        {
            app_general_push_after_fu_temp(reg_detailes);
            break;
        }
        case REG_ADDR_AFTER_FU_HUM:
        {
            app_general_push_after_fu_hum(reg_detailes);
            break;
        }
        case REG_ADDR_BACK_AIR_TEMP:
        {
            app_general_push_backair_temp(reg_detailes);
            break;
        }
        case REG_ADDR_BACK_AIR_HUM:
        {
            app_general_push_backair_hum(reg_detailes);
            break;
        }
        case REG_ADDR_EXHAST_AIR_TEMP:
        {
            app_general_push_exhastair_temp(reg_detailes);
            break;
        }
        case REG_ADDR_EXHAST_AIR_HUM:
        {
            app_general_push_exhastair_hum(reg_detailes);
            break;
        }
        case REG_ADDR_BACK_AIR_CO2:
        {
            app_general_push_backair_co2(reg_detailes);
            break;
        }
        case REG_ADDR_BACK_AIR_PM25:
        {
            app_general_push_backair_pm25(reg_detailes);
            break;
        }
        case REG_ADDR_DEADZONE_FAN_HUM:
        {
            app_general_push_aircod_setHum_deadZone(reg_detailes);
            break;
        }
        case REG_ADDR_DEADZONE_FAN_TEMP:
        {
            app_general_push_aircod_setTemp_deadZone(reg_detailes);
            break;
        }
        case REG_ADDR_NEW_AIR_SPEED:
        {
            app_general_push_aircod_newair_speed(reg_detailes);
            break;
        }
        case REG_ADDR_BACK_AIR_SPEED:
        {
            app_general_push_aircod_backair_speed(reg_detailes);
            break;
        }
        case REG_ADDR_WATER_MACHINE_FRE:
        {
            app_general_push_water_machine_fre(reg_detailes);
            break;
        }
        case REG_ADDR_FU_MACHINE_FRE:
        {
            app_general_push_fu_machine_fre(reg_detailes);
            break;
        }
        case REG_ADDR_MASTER_SUPPLY_TEMP:
        {
            app_general_push_master_supply_temp(reg_detailes);
            break;
        }
        case REG_ADDR_MASTER_LIVING_TEMP:
        {
            app_general_push_master_living_temp(reg_detailes);
            break;
        }
        case REG_ADDR_MASTER_OUTDOOR_TEMP:
        {
            app_general_push_master_outdoor_temp(reg_detailes);
            break;
        }
        case REG_ADDR_ADJUST_OUTDOORAIR_HUM:
        {
            app_general_push_adjust_outdoor_hum(reg_detailes);
            break;
        }
        case REG_ADDR_ADJUST_OUTDOORAIR_TEMP:
        {
            app_general_push_adjust_outdoor_temp(reg_detailes);
            break;
        }
        case REG_ADDR_ADJUST_OUTDOORAIR_CO2:
        {
            app_general_push_adjust_outdoor_co2(reg_detailes);
            break;
        }
        case REG_ADDR_ADJUST_OUTDOORAIR_PM25:
        {
            app_general_push_adjust_outdoor_pm25(reg_detailes);
            break;
        }
        case REG_ADDR_ADJUST_BEFORFU_HUM:
        {
            app_general_push_adjust_beforfu_hum(reg_detailes);
            break;
        }
        case REG_ADDR_ADJUST_BEFORFU_TEMP:
        {
            app_general_push_adjust_beforfu_temp(reg_detailes);
            break;
        }
        case REG_ADDR_ADJUST_BEFORFU_CO2:
        {
            app_general_push_adjust_beforfu_co2(reg_detailes);
            break;
        }
        case REG_ADDR_ADJUST_BEFORFU_PM25:
        {
            app_general_push_adjust_beforfu_pm25(reg_detailes);
            break;
        }
        case REG_ADDR_ADJUST_AFTERFU_HUM:
        {
            app_general_push_adjust_afterfu_hum(reg_detailes);
            break;
        }
        case REG_ADDR_ADJUST_AFTERFU_TEMP:
        {
            app_general_push_adjust_afterfu_temp(reg_detailes);
            break;
        }
        case REG_ADDR_ADJUST_AFTERFU_CO2:
        {
            app_general_push_adjust_afterfu_co2(reg_detailes);
            break;
        }
        case REG_ADDR_ADJUST_AFTERFU_PM25:
        {
            app_general_push_adjust_afterfu_pm25(reg_detailes);
            break;
        }
        case REG_ADDR_ADJUST_BACKAIR_HUM:
        {
            app_general_push_adjust_backair_hum(reg_detailes);
            break;
        }
        case REG_ADDR_ADJUST_BACKAIR_TEMP:
        {
            app_general_push_adjust_backair_temp(reg_detailes);
            break;
        }
        case REG_ADDR_ADJUST_BACKAIR_CO2:
        {
            app_general_push_adjust_backair_co2(reg_detailes);
            break;
        }
        case REG_ADDR_ADJUST_BACKAIR_PM25:
        {
            app_general_push_adjust_backair_pm25(reg_detailes);
            break;
        }
        case REG_ADDR_ADJUST_EXHASTAIR_HUM:
        {
            app_general_push_adjust_exhastair_hum(reg_detailes);
            break;
        }
        case REG_ADDR_ADJUST_EXHASTAIR_TEMP:
        {
            app_general_push_adjust_exhastair_temp(reg_detailes);
            break;
        }
        case REG_ADDR_ADJUST_EXHASTAIR_CO2:
        {
            app_general_push_adjust_exhastair_co2(reg_detailes);
            break;
        }
        case REG_ADDR_ADJUST_EXHASTAIR_PM25:
        {
            app_general_push_adjust_exhastair_pm25(reg_detailes);
            break;
        }
        case REG_ADDR_SET_MIXWATER_COLD_TEMP:
        {
            app_general_push_set_mixwater_cold_temp(reg_detailes);
            break;
        }
        case REG_ADDR_SET_MIXWATER_HEAT_TEMP:
        {
            app_general_push_set_mixwater_heat_temp(reg_detailes);
            break;
        }
        case REG_ADDR_MIXWATER_INWATER_TEMP:
        {
            break;
        }
        case REG_ADDR_MIXWATER_BACKWATER_TEMP:
        {
            break;
        }
        case REG_ADDR_MIXWATER_TEMP:
        {
            break;
        }
        case REG_ADDR_OUTPUT_PUMP:
        {
            break;
        }
        case REG_ADDR_OUTPUT_THREE_VAVLE:
        {
            break;
        }
        case REG_ADDR_MIXWATER_AI_DEST_TEMP:
        {
            break;
        }
        case REG_ADDR_MIXWATER_AI_ENABLE:
        {
            app_general_push_ai_enable_status(reg_detailes);
            break;
        }
        case REG_ADDR_MIXWATER_LOW_TEMP_PROTECT:
        {
            app_general_push_mix_freeze_protect(reg_detailes);
            break;
        }
		case REG_ADRESS_COOL_BACKWIND_SETTEMP  : 
			app_general_push_coolbackwind_set_temp(reg_detailes);
			break;
		case REG_ADRESS_HOT_BACKWIND_SETTEMP   : 
			app_general_push_hotbackwind_set_temp(reg_detailes);
			break;
		case REG_ADRESS_COOL_OUTWATER_SETTEMP  : 
			app_general_push_cooloutwater_set_temp(reg_detailes);
			break;
		case REG_ADRESS_HOT_OUTWATER_SETTEMP   : 
			app_general_push_hotoutwater_set_temp(reg_detailes);
			break;
		case REG_ADRESS_COOL_BACKWATER_SETTEMP : 
			app_general_push_coolbackwater_set_temp(reg_detailes);
			break;
		case REG_ADRESS_HOT_BACKWATER_SETTEMP  : 
			app_general_push_hotbackwater_set_temp(reg_detailes);
			break;		
        default:
        {
            break;
        }
    }
    return true;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

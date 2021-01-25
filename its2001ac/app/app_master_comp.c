//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include ".\app_cfg.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define   MODBUS_ADDR    0x01
typedef enum
{
    mm_aircod_idle                  = 0x0000,//空闲状态
    mm_aircod_transmit_read_only_inf,//读取只读信息
    mm_aircod_receive_read_only_inf,
    mm_aircod_tramsimt_read_write_inf,//读取写读信息
    mm_aircod_receive_read_write_inf,
    mm_aircod_tramsimt_set_mode           ,//设置模式
    mm_aircod_receive_set_mode            ,
    mm_aircod_tramsimt_power          ,//开关机
    mm_aircod_receive_power           ,
    mm_aircod_tramsimt_hot_water_enable          ,//热水模式使能
    mm_aircod_receive_hot_water_enable           ,
    mm_aircod_tramsimt_hot_water_disinfect          ,//热水消毒功能
    mm_aircod_receive_hot_water_disinfect           ,
    mm_aircod_tramsimt_cold_mode_first          ,//制冷模式优先
    mm_aircod_receive_cold_mode_first           ,
    mm_aircod_tramsimt_defroster_interval          ,//除霜间隔时间
    mm_aircod_receive_defroster_interval           ,
    mm_aircod_tramsimt_max_deforster_run_2          ,//2#除霜最大运行时间
    mm_aircod_receive_max_deforster_run_2           ,
    mm_aircod_tramsimt_comp_fre_cal_2          ,//2#压缩机频率计算周期
    mm_aircod_receive_comp_fre_cal_2           ,
    mm_aircod_tramsimt_fre_max_run          ,//压缩机最高运行频率
    mm_aircod_receive_fre_max_run           ,
    mm_aircod_tramsimt_max_speed_2          ,//2#风机最高转速
    mm_aircod_receive_max_speed_2           ,
    mm_aircod_tramsimt_function_feature          ,//功能特征
    mm_aircod_receive_function_feature           ,
    mm_aircod_tramsimt_cold_set_water_temp          ,//制冷出水设置温度
    mm_aircod_receive_cold_set_water_temp           ,
    mm_aircod_tramsimt_heat_set_water_temp  ,//制热出水设置温度
    mm_aircod_receive_heat_set_water_temp  ,
    mm_aircod_tramsimt_cold_set_freeze_temp          ,//制冷防冻设置温度
    mm_aircod_receive_cold_set_freeze_temp           ,
    mm_aircod_tramsimt_dest_over_hot_degree          ,//目标过热度
    mm_aircod_receive_dest_over_hot_degree           ,
    mm_aircod_tramsimt_deforster_line_a          ,//除霜检测线A点
    mm_aircod_receive_deforster_line_a          ,
    mm_aircod_tramsimt_deforster_line_b          ,//除霜检测线B点
    mm_aircod_receive_deforster_line_b           ,
    mm_aircod_tramsimt_deforster_end_temp          ,//除霜出盘结束温度
    mm_aircod_receive_deforster_end_temp           ,
    mm_aircod_tramsimt_max_deforster_run_1          ,//1#除霜最大运行时间
    mm_aircod_receive_max_deforster_run_1           ,
    mm_aircod_tramsimt_comp_fre_cal_1          ,//1#压缩机频率计算周期
    mm_aircod_receive_comp_fre_cal_1           ,
    mm_aircod_tramsimt_max_speed_1          ,//1#风机最高转速
    mm_aircod_receive_max_speed_1           ,
    mm_aircod_tramsimt_indoor_fan_control_enable          ,//室内风机控制使能
    mm_aircod_receive_indoor_fan_control_enable           ,
    mm_aircod_tramsimt_indoor_fan_speed          ,//室内风机转速
    mm_aircod_receive_indoor_fan_speed           ,
    mm_aircod_tramsimt_indoor_two_vavle_status          ,//室内电动二通阀状态
    mm_aircod_receive_indoor_two_vavle_status           ,
    mm_aircod_tramsimt_indoor_drain_pum_status          ,//室内排水泵状态
    mm_aircod_receive_indoor_drain_pum_status           ,
    mm_aircod_tramsimt_indoor_water_level          ,//室内水位开关状态
    mm_aircod_receive_indoor_water_level           ,
    mm_aircod_tramsimt_indoor_elec_hot_status          ,//室内电加热状态
    mm_aircod_receive_indoor_elec_hot_status           ,
    mm_aircod_tramsimt_indoor_exv1_degree          ,//室内exv1开度
    mm_aircod_receive_indoor_exv1_degree           ,
    mm_aircod_tramsimt_indoor_backair_hum          ,//室内回风湿度值
    mm_aircod_receive_indoor_backair_hum           ,
    mm_aircod_tramsimt_indoor_inair_hum          ,//室内进风湿度值
    mm_aircod_receive_indoor_inair_hum           ,
    mm_aircod_tramsimt_indoor_inair_temp          ,//室内进风温度值
    mm_aircod_receive_indoor_inair_temp           ,
    mm_aircod_tramsimt_indoor_backair_temp          ,//室内回风温度值
    mm_aircod_receive_indoor_backair_temp           ,

	mm_aircod_tramsimt_cool_backair_settemp       ,//5007制冷回风设置温度
	mm_aircod_receive_cool_backair_settemp        ,
	mm_aircod_tramsimt_hot_backair_settemp        ,//5008制热回风设置温度
	mm_aircod_receive_hot_backair_settemp         ,
	mm_aircod_tramsimt_sethumidity		   		  ,//5009相对湿度
	mm_aircod_receive_sethumidity 				  ,
	mm_aircod_tramsimt_lifewater				  ,//5010生活热水
	mm_aircod_receive_lifewater				  	  ,
	mm_aircod_tramsimt_cool_outwater_settemp	  ,//5024制冷出水设置温度
	mm_aircod_receive_cool_outwater_settemp 	  ,
	mm_aircod_tramsimt_hot_outwater_settemp	      ,//5026制热出水设置温度
	mm_aircod_receive_hot_outwater_settemp 	  	  ,
	mm_aircod_tramsimt_two_way_value_state 	      ,//5042二通阀状态
	mm_aircod_receive_two_way_value_state		  ,
	mm_aircod_tramsimt_indoor_message  		      ,//5047室内信息
 	mm_aircod_receive_indoor_message 		      ,
}modbus_master_state_def;

uint32_t updataWord = 0;

#define SET_MODE_BIT                   0X0001//5001
#define SET_POWER_BIT                  0X0002//5003
#define SET_SUPPLY_COLD_BIT            0X0004//5025 
#define SET_SUPPLY_WARM_BIT            0X0008//5027 

#define SET_COLD_BACKWIND_SETTEMP_BIT   0X0010//5007
#define SET_HOT_BACKWIND_SETTEMP_BIT    0X0020//5008
#define SET_HUMIDITY_BIT  				0X0040//5009
#define SET_LIFE_HOTWATER_BIT   		0X0080//5010

#define SET_SUPPLY_COLD_OUTWATER_BIT  0X0100//5024
#define SET_SUPPLY_HOT_OUTWATER_BIT   0X0200//5026
#define TWO_WAY_STATE_BIT   		   0X0400//5042

#define INSIDE_BACKWIND_HUMIDITY_BIT   0X0800//5047
#define INSIDE_INWIND_HUMIDITY_BIT     0X1000//5048
#define INSIDE_INWIND_TEMP_BIT   	   0X2000//5049
#define INSIDE_BACKWIND_TEMP_BIT   	   0X4000//5050
modbus_master_state_def   aircod_status = mm_aircod_idle;

uint16_t aircodMesBuff_1[67]={
            0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, //0-15
            0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,//16-31
            0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, //32-47
            0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, //48-63
            0x0000,0x0000,0x0000,                                                                                    //64-67
                            };

uint16_t aircodMesBuff_5001[52]={
            0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, //0-15
            0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,//16-31
            0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, //32-47
            0x0000,0x0000,0x0000,0x0000,                                                                             //48-52
                                };

bool APP_pull_aricod_message(uint16_t in_reg_add,uint16_t *out_reg_value)
{
    if((in_reg_add >= 1) && (in_reg_add <= 67))
    {
        *out_reg_value = aircodMesBuff_1[(in_reg_add-1)];
        return true;
    }  
    else if((in_reg_add >= 5001) && (in_reg_add <= 5052))
    {
        *out_reg_value = aircodMesBuff_5001[(in_reg_add-5001)];
        return true;
    }  
    return false;
}

bool APP_push_aricod_message(uint16_t in_reg_add,uint16_t in_reg_value)
{
    if((in_reg_add >= 1) && (in_reg_add <= 67))
    {
       aircodMesBuff_1[(in_reg_add-1)] = in_reg_value;
       return true;
    } 
    else if((in_reg_add >= 5001) && (in_reg_add <= 5052))
    {
       aircodMesBuff_5001[(in_reg_add-5001)] = in_reg_value;
       switch(in_reg_add)
       {
           case DRIVE_BOARD_SET_MODE:
           {
               updataWord |= SET_MODE_BIT;

			   //这边要删除
			   //若模式为制冷或者除湿
			   //设置制冷回风温度，若无数据则推一个默认数据上去。4x5007 数组下标[7-1]赋值
			   //设置相对湿度，若无数据则推一个默认数据上去。4x5009 数组下标[9-1]赋值
			   //设置制冷出水设置温度，若无数据则推一个默认数据上去。4x5024 数组下标[24-1]赋值
			   //设置制冷回水设置温度，若无数据则推一个默认数据上去。4x5025 数组下标[25-1]赋值
			   //设置电动二通阀状态，4x5042 数组下标[42-1]赋值

			   //若模式为制热模式
			   //设置制热回风温度，若无数据则推一个默认数据上去。4x5008 数组下标[8-1]赋值
			   //设置制热出水温度，若无数据则推一个默认数据上去。4x5026 数组下标[26-1]赋值
			   //设置制热回水温度，若无数据则推一个默认数据上去。4x5027 数组下标[27-1]赋值

			   //室内电动二通阀(预冷/预热)				4x5024 这边没实现
			   //室内风机实际转速					4x5062
			   
			   if(in_reg_value == 1 || in_reg_value == 3)//制冷 | 除湿
			   {
			   		updataWord |= SET_COLD_BACKWIND_SETTEMP_BIT;//制冷回风温度5007
					if(app_general_pull_coolbackwind_set_temp() == 0)
					{
						app_general_push_coolbackwind_set_temp(150);
					}
					aircodMesBuff_5001[6] = app_general_pull_coolbackwind_set_temp();//5007-5001制冷回风设置温度
					
					updataWord |= SET_HUMIDITY_BIT;
					aircodMesBuff_5001[8] = app_general_pull_aircod_humidity()*10;//5009-5001相对湿度设置
					//
					updataWord |= SET_SUPPLY_COLD_OUTWATER_BIT;
					if(app_general_pull_cooloutwater_set_temp() == 0)
					{
						app_general_push_cooloutwater_set_temp(200);
					}
					aircodMesBuff_5001[23] = app_general_pull_cooloutwater_set_temp();//5024-5001制冷出水设置温度
					
					//					
					updataWord |= SET_SUPPLY_COLD_BIT;
					if(app_general_pull_coolbackwater_set_temp() == 0)
					{
						app_general_push_coolbackwater_set_temp(200);
					}
					aircodMesBuff_5001[24] = app_general_pull_coolbackwater_set_temp();//5025-5001制冷回水设置温度
					updataWord |= TWO_WAY_STATE_BIT;
					aircodMesBuff_5001[41] = 1;
			   }
			   else if(in_reg_value == 2)//制热
			   {
				   updataWord |= SET_HOT_BACKWIND_SETTEMP_BIT;//制热回风设置温度5008
				   if(app_general_pull_hotbackwind_set_temp() == 0)
				   {
					   app_general_push_hotbackwind_set_temp(350);
				   }
				   aircodMesBuff_5001[7] = app_general_pull_hotbackwind_set_temp();
				   
				   updataWord |= SET_SUPPLY_HOT_OUTWATER_BIT;//制热出水设置温度5026
				   if(app_general_pull_hotoutwater_set_temp() == 0)
				   {
					   app_general_push_hotoutwater_set_temp(350);
				   }
				   aircodMesBuff_5001[25] = app_general_pull_hotoutwater_set_temp();
				   
				   updataWord |= SET_SUPPLY_WARM_BIT;//制热回水
				   if(app_general_pull_hotbackwater_set_temp() == 0)
				   {
					   app_general_push_hotbackwater_set_temp(350);
				   }
				   aircodMesBuff_5001[26] = app_general_pull_hotbackwater_set_temp();
			   }

               break;
           }
           case DRIVE_BOARD_POWER_ON_OFF:
           {
               updataWord |= SET_POWER_BIT;
               break;
           }
		   case DRIVE_BOARD_COOL_SET_TEMP://制冷回风设置温度5007
			    updataWord |= SET_COLD_BACKWIND_SETTEMP_BIT;
		   		break;
		   case DRIVE_BOARD_HOT_SET_TEMP://制热回风设置温度5008
				updataWord |= SET_HOT_BACKWIND_SETTEMP_BIT;
		   		break;
		   case DRIVE_BOARD_HUMIDITY_SET_TEMP://制热回风设置温度5009
			    updataWord |= SET_HUMIDITY_BIT;
		   		break;
		   case DRIVE_BOARD_LIFE_HOTWATER_SET_TEMP://生活热水5010
				updataWord |= SET_LIFE_HOTWATER_BIT;
				break;
		   case DRIVE_BOARD_COOL_OUTWATER_SETTEMP://制冷出水设置温度5024
				updataWord |= SET_SUPPLY_COLD_OUTWATER_BIT;
				break;
           case DRIVE_BOARD_COOL_BACKWATER_SETTEMP://制冷回水设置温度
           {
                updataWord |= SET_SUPPLY_COLD_BIT;
                break;
           }
		   case DRIVE_BOARD_HOT_OUTWATER_SETTEMP://制热出水设置温度
				updataWord |= SET_SUPPLY_HOT_OUTWATER_BIT;
		   	    break;
           case DRIVE_BOARD_HOT_BACKWATER_SETTEMP:
           {
                updataWord |= SET_SUPPLY_WARM_BIT;
                break;
           }
		   case DRIVE_BOARD_TWO_WAY_VALUE_STATE://室内电动二通阀状态5042
				updataWord |= TWO_WAY_STATE_BIT;
		        break;
		   case DRIVE_BOARD_INDOOR_MESSAGE://室内回风湿度值5047 间隔一段时间10s/20s 写入一次 用10指令写入
		   		updataWord |= INSIDE_BACKWIND_HUMIDITY_BIT;
			   break;
           default:break;
       }
       return true;
    } 
    return false;
}
bool _push_aricod_message(uint16_t in_reg_add,uint16_t in_reg_value)
{
    if((in_reg_add >= 1) && (in_reg_add <= 67))
    {
       aircodMesBuff_1[(in_reg_add-1)] = in_reg_value;
       return true;
    } 
    else if((in_reg_add >= 5001) && (in_reg_add <= 5052))
    {
	   if(updataWord == 0)
	   {
		   aircodMesBuff_5001[(in_reg_add-5001)] = in_reg_value;
	   }  
	   switch(in_reg_add)
       {
           case DRIVE_BOARD_SET_MODE:
           {   
               AirRunMode_Def mode = app_general_pull_aircod_mode();
               if(aircodMesBuff_5001[0] != mode)
               {
                   app_general_push_aircod_mode(mode);
               }           
               break;
           }
           case DRIVE_BOARD_POWER_ON_OFF:
           {
               uint8_t power = app_general_pull_power_status();
               if(aircodMesBuff_5001[2] != power)
               {
                   app_general_push_power_status(power);
               }    
               break;
           }
           case DRIVE_BOARD_COOL_BACKWATER_SETTEMP:
           {
               break;
           }
           case DRIVE_BOARD_HOT_BACKWATER_SETTEMP:
           {
               break;
           }
           default:break;
       }
       return true;
    } 
    return false;
}
void App_push_aircod_buff(uint16_t RegAddr,uint8_t RegLength)
{
    uint16_t regDetails;
    while(RegLength)
    {
        if(mde_pull_mRtu_receiveReg(MASTER_COMP,RegAddr,&regDetails))
        {
            _push_aricod_message((RegAddr+1),regDetails);
        }	
		switch(RegAddr)
		{
			default:
			{              				
				break;
			}
		}
        RegAddr++;
        RegLength--;
    }
}

static void app_push_send_reg_aircod(uint16_t in_regAddr,uint8_t in_regLength)
{
    uint16_t regDetails;
    while(in_regLength)
    {
        APP_pull_aricod_message(in_regAddr,&regDetails);
        mde_push_mRtu_master_sendReg(MASTER_COMP,in_regAddr,regDetails);
        in_regAddr++;
        in_regLength--;
    }
}

void app_master_comp_task(void)
{
    mde_mRtu_master_task();
    macro_createTimer(ReadInterval,timerType_millisecond,1000);
	macro_createTimer(blinkDelay,timerType_millisecond,0);
    macro_createTimer(updata_delay,timerType_millisecond,0);
    pbc_timerClockRun_task(&ReadInterval);
	pbc_timerClockRun_task(&blinkDelay);
    pbc_timerClockRun_task(&updata_delay);
    uint16_t regDetails;
    uint16_t reg_addr;
    uint16_t reg_length;
	if(pbc_pull_timerIsCompleted(&blinkDelay))
	{
		mde_led_off(LED_2);
	}
    switch(aircod_status)
    {
        case mm_aircod_idle:
        {         
			if(updataWord)
			{
				pbc_reload_timerClock(&blinkDelay,5000);//闪动5s
				mde_led_blink(LED_2,5,5);
                if(pbc_pull_timerIsCompleted(&updata_delay))
                {
                    if(updataWord & SET_MODE_BIT)
                    {
                        aircod_status = mm_aircod_tramsimt_set_mode;
                    }
                    else if(updataWord & SET_POWER_BIT)
                    {
                        aircod_status = mm_aircod_tramsimt_power;
                    }
                    else if(updataWord & SET_SUPPLY_COLD_BIT)
                    {
                        aircod_status = mm_aircod_tramsimt_cold_set_water_temp;
                    }
                    else if(updataWord & SET_SUPPLY_WARM_BIT)
                    {
                        aircod_status = mm_aircod_tramsimt_heat_set_water_temp;
                    }
                    else if(updataWord & SET_COLD_BACKWIND_SETTEMP_BIT)//5007
                    {
                        aircod_status = mm_aircod_tramsimt_cool_backair_settemp;
                    }
                    else if(updataWord & SET_HOT_BACKWIND_SETTEMP_BIT)//5008
                    {
                        aircod_status = mm_aircod_tramsimt_hot_backair_settemp;
                    }
                    else if(updataWord & SET_HUMIDITY_BIT)//5009
                    {
                        aircod_status = mm_aircod_tramsimt_sethumidity;
                    }
                    else if(updataWord & SET_LIFE_HOTWATER_BIT)//5010
                    {
                        aircod_status = mm_aircod_tramsimt_lifewater;
                    }
                    else if(updataWord & SET_SUPPLY_COLD_OUTWATER_BIT)//5024
                    {
                        aircod_status = mm_aircod_tramsimt_cool_outwater_settemp;
                    }
                    else if(updataWord & SET_SUPPLY_HOT_OUTWATER_BIT)//5026
                    {
                        aircod_status = mm_aircod_tramsimt_hot_outwater_settemp;
                    }
                    else if(updataWord & TWO_WAY_STATE_BIT)//5042
                    {
                        aircod_status = mm_aircod_tramsimt_two_way_value_state;
                    }
                    else if(updataWord & INSIDE_BACKWIND_HUMIDITY_BIT)//5047~5050 10指令写4个数据
                    {
                        aircod_status = mm_aircod_tramsimt_indoor_message;
                    }
                }				
			}
			else
			{
				if(pbc_pull_timerIsCompleted(&ReadInterval))
				{
					pbc_reload_timerClock(&ReadInterval,1000);

					static uint16_t timecount = 0;
					if(++timecount > 9)
					{
						timecount = 0;
						
						aircodMesBuff_5001[46] = Pull_RSS1001H_Temperature(SENSOR_AFTERFU);
						aircodMesBuff_5001[47] = Pull_RSS1001H_Humidity(SENSOR_AFTERFU);
						aircodMesBuff_5001[48] = Pull_RSS1001H_Temperature(SENSOR_BEFOREFU);
						aircodMesBuff_5001[49] = Pull_RSS1001H_Humidity(SENSOR_BEFOREFU);
						updataWord |= INSIDE_BACKWIND_HUMIDITY_BIT;
					}
					else
					{
						aircod_status = mm_aircod_transmit_read_only_inf;
					}	
				}
			}
            break;
        }
        case mm_aircod_transmit_read_only_inf:
        {//读取只读信息
            if(mmRunS_idle == mde_mRtu_master_run_status(MASTER_COMP))
            {
                mde_mRtu_master_cmd0x04_transmit(MASTER_COMP,MODBUS_ADDR,0x0000,60);
                aircod_status = mm_aircod_receive_read_only_inf;
            }
            break;
        }
        case mm_aircod_receive_read_only_inf:
        {
            if(mRtuS_master_complete == mde_mRtu_master_reveive_status(MASTER_COMP))
            {
                aircod_status = mm_aircod_tramsimt_read_write_inf;
                pbc_reload_timerClock(&ReadInterval,1000);
                mde_pull_mRtu_register_add_len(MASTER_COMP,&reg_addr,&reg_length);
                App_push_aircod_buff((reg_addr),reg_length);
            }
            else if((mRtuS_master_timeout | mRtuS_master_poterr) & mde_mRtu_master_reveive_status(MASTER_COMP))
            {
                aircod_status = mm_aircod_tramsimt_read_write_inf;
                pbc_reload_timerClock(&ReadInterval,300);
            }
            break;
        }
        case mm_aircod_tramsimt_read_write_inf:
        {//读取写读信息
            if(pbc_pull_timerIsCompleted(&ReadInterval))
            {
                if(mmRunS_idle == mde_mRtu_master_run_status(MASTER_COMP))
                {
                    mde_mRtu_master_cmd0x03_transmit(MASTER_COMP,MODBUS_ADDR,5000,52);
                    aircod_status = mm_aircod_receive_read_write_inf;
                }
            }         
            break;
        }
        case mm_aircod_receive_read_write_inf:
        {
            if(mRtuS_master_complete == mde_mRtu_master_reveive_status(MASTER_COMP))
            {
                aircod_status = mm_aircod_idle;
                mde_pull_mRtu_register_add_len(MASTER_COMP,&reg_addr,&reg_length);
                App_push_aircod_buff((reg_addr),reg_length);
            }
            else if((mRtuS_master_timeout | mRtuS_master_poterr) & mde_mRtu_master_reveive_status(MASTER_COMP))
            {
                aircod_status = mm_aircod_idle;
            }
            break;
        }
        case mm_aircod_tramsimt_set_mode:
        {//设置模式
            if(mmRunS_idle == mde_mRtu_master_run_status(MASTER_COMP))
            {
                APP_pull_aricod_message(DRIVE_BOARD_SET_MODE,&regDetails);
                mde_mRtu_master_cmd0x06_transmit(MASTER_COMP,MODBUS_ADDR,(DRIVE_BOARD_SET_MODE-1),regDetails);
                aircod_status = mm_aircod_receive_set_mode;
            }
            break;
        }
        case mm_aircod_receive_set_mode:
        {
            if(mRtuS_master_complete == mde_mRtu_master_reveive_status(MASTER_COMP))
            {
                updataWord &= (~SET_MODE_BIT);
                aircod_status = mm_aircod_idle;
                pbc_reload_timerClock(&updata_delay,UPDATA_DELAY);
            }
            else if((mRtuS_master_timeout | mRtuS_master_poterr) & mde_mRtu_master_reveive_status(MASTER_COMP))
            {
                updataWord &= (~SET_MODE_BIT);
                aircod_status = mm_aircod_idle;
                pbc_reload_timerClock(&updata_delay,UPDATA_DELAY);
            }
            break;
        }
        case mm_aircod_tramsimt_power:
        {//开关机
            if(mmRunS_idle == mde_mRtu_master_run_status(MASTER_COMP))
            {
                APP_pull_aricod_message(DRIVE_BOARD_POWER_ON_OFF,&regDetails);
                mde_mRtu_master_cmd0x06_transmit(MASTER_COMP,MODBUS_ADDR,(DRIVE_BOARD_POWER_ON_OFF-1),regDetails);
                aircod_status = mm_aircod_receive_power;
            }
            break;
        }
        case mm_aircod_receive_power:
        {
            if(mRtuS_master_complete == mde_mRtu_master_reveive_status(MASTER_COMP))
            {
                updataWord &= (~SET_POWER_BIT);
                aircod_status = mm_aircod_idle;
                pbc_reload_timerClock(&updata_delay,UPDATA_DELAY);
            }
            else if((mRtuS_master_timeout | mRtuS_master_poterr) & mde_mRtu_master_reveive_status(MASTER_COMP))
            {
                updataWord &= (~SET_POWER_BIT);
                aircod_status = mm_aircod_idle;
                pbc_reload_timerClock(&updata_delay,UPDATA_DELAY);
            }
            break;
        }
        case mm_aircod_tramsimt_cold_set_water_temp:
        {//制冷出水设置温度
            if(mmRunS_idle == mde_mRtu_master_run_status(MASTER_COMP))
            {
                APP_pull_aricod_message(DRIVE_BOARD_COOL_BACKWATER_SETTEMP,&regDetails);
                mde_mRtu_master_cmd0x06_transmit(MASTER_COMP,MODBUS_ADDR,(DRIVE_BOARD_COOL_BACKWATER_SETTEMP-1),regDetails);
                aircod_status = mm_aircod_receive_cold_set_water_temp;
            }
            break;
        }
        case mm_aircod_receive_cold_set_water_temp:
        {
            if(mRtuS_master_complete == mde_mRtu_master_reveive_status(MASTER_COMP))
            {
                updataWord &= (~SET_SUPPLY_COLD_BIT);
                aircod_status = mm_aircod_idle;
                 pbc_reload_timerClock(&updata_delay,UPDATA_DELAY);
            }
            else if((mRtuS_master_timeout | mRtuS_master_poterr) & mde_mRtu_master_reveive_status(MASTER_COMP))
            {
                updataWord &= (~SET_SUPPLY_COLD_BIT);
                aircod_status = mm_aircod_idle;
                 pbc_reload_timerClock(&updata_delay,UPDATA_DELAY);
            }
            break;
        }
        case mm_aircod_tramsimt_heat_set_water_temp:
        {//制热出水设置温度
            if(mmRunS_idle == mde_mRtu_master_run_status(MASTER_COMP))
            {
                APP_pull_aricod_message(DRIVE_BOARD_HOT_BACKWATER_SETTEMP,&regDetails);
                mde_mRtu_master_cmd0x06_transmit(MASTER_COMP,MODBUS_ADDR,(DRIVE_BOARD_HOT_BACKWATER_SETTEMP-1),regDetails);
                aircod_status = mm_aircod_receive_heat_set_water_temp;
            }
            break;
        }
        case mm_aircod_receive_heat_set_water_temp:
        {
            if(mRtuS_master_complete == mde_mRtu_master_reveive_status(MASTER_COMP))
            {
                updataWord &= (~SET_SUPPLY_WARM_BIT);
                aircod_status = mm_aircod_idle;
                 pbc_reload_timerClock(&updata_delay,UPDATA_DELAY);
            }
            else if((mRtuS_master_timeout | mRtuS_master_poterr) & mde_mRtu_master_reveive_status(MASTER_COMP))
            {
                updataWord &= (~SET_SUPPLY_WARM_BIT);
                aircod_status = mm_aircod_idle;
                 pbc_reload_timerClock(&updata_delay,UPDATA_DELAY);
            }
            break;
        }
		case mm_aircod_tramsimt_cool_backair_settemp: 
            if(mmRunS_idle == mde_mRtu_master_run_status(MASTER_COMP))
            {
                APP_pull_aricod_message(DRIVE_BOARD_COOL_SET_TEMP,&regDetails);
                mde_mRtu_master_cmd0x06_transmit(MASTER_COMP,MODBUS_ADDR,(DRIVE_BOARD_COOL_SET_TEMP-1),regDetails);
                aircod_status = mm_aircod_receive_cool_backair_settemp;
            }
			break;
		case mm_aircod_receive_cool_backair_settemp: 
            if(mRtuS_master_complete == mde_mRtu_master_reveive_status(MASTER_COMP))
            {
                updataWord &= (~SET_COLD_BACKWIND_SETTEMP_BIT);
                aircod_status = mm_aircod_idle;
                 pbc_reload_timerClock(&updata_delay,UPDATA_DELAY);
            }
            else if((mRtuS_master_timeout | mRtuS_master_poterr) & mde_mRtu_master_reveive_status(MASTER_COMP))
            {
                updataWord &= (~SET_COLD_BACKWIND_SETTEMP_BIT);
                aircod_status = mm_aircod_idle;
                 pbc_reload_timerClock(&updata_delay,UPDATA_DELAY);
            }
			break;

		case mm_aircod_tramsimt_hot_backair_settemp: 
			if(mmRunS_idle == mde_mRtu_master_run_status(MASTER_COMP))
            {
                APP_pull_aricod_message(DRIVE_BOARD_HOT_SET_TEMP,&regDetails);
                mde_mRtu_master_cmd0x06_transmit(MASTER_COMP,MODBUS_ADDR,(DRIVE_BOARD_HOT_SET_TEMP-1),regDetails);
                aircod_status = mm_aircod_receive_hot_backair_settemp;
            }
			break;
		case mm_aircod_receive_hot_backair_settemp: 
            if(mRtuS_master_complete == mde_mRtu_master_reveive_status(MASTER_COMP))
            {
                updataWord &= (~SET_HOT_BACKWIND_SETTEMP_BIT);
                aircod_status = mm_aircod_idle;
                 pbc_reload_timerClock(&updata_delay,UPDATA_DELAY);
            }
            else if((mRtuS_master_timeout | mRtuS_master_poterr) & mde_mRtu_master_reveive_status(MASTER_COMP))
            {
                updataWord &= (~SET_HOT_BACKWIND_SETTEMP_BIT);
                aircod_status = mm_aircod_idle;
                 pbc_reload_timerClock(&updata_delay,UPDATA_DELAY);
            }
			break;
		case mm_aircod_tramsimt_sethumidity: 
			if(mmRunS_idle == mde_mRtu_master_run_status(MASTER_COMP))
            {
                APP_pull_aricod_message(DRIVE_BOARD_HUMIDITY_SET_TEMP,&regDetails);
                mde_mRtu_master_cmd0x06_transmit(MASTER_COMP,MODBUS_ADDR,(DRIVE_BOARD_HUMIDITY_SET_TEMP-1),regDetails);
                aircod_status = mm_aircod_receive_sethumidity;
            }
			break;
		case mm_aircod_receive_sethumidity: 
            if(mRtuS_master_complete == mde_mRtu_master_reveive_status(MASTER_COMP))
            {
                updataWord &= (~SET_HUMIDITY_BIT);
                aircod_status = mm_aircod_idle;
                 pbc_reload_timerClock(&updata_delay,UPDATA_DELAY);
            }
            else if((mRtuS_master_timeout | mRtuS_master_poterr) & mde_mRtu_master_reveive_status(MASTER_COMP))
            {
                updataWord &= (~SET_HUMIDITY_BIT);
                aircod_status = mm_aircod_idle;
                 pbc_reload_timerClock(&updata_delay,UPDATA_DELAY);
            }
			break;
	
		case mm_aircod_tramsimt_lifewater: 
			if(mmRunS_idle == mde_mRtu_master_run_status(MASTER_COMP))
            {
                APP_pull_aricod_message(DRIVE_BOARD_LIFE_HOTWATER_SET_TEMP,&regDetails);
                mde_mRtu_master_cmd0x06_transmit(MASTER_COMP,MODBUS_ADDR,(DRIVE_BOARD_LIFE_HOTWATER_SET_TEMP-1),regDetails);
                aircod_status = mm_aircod_receive_lifewater;
            }
			break;
		case mm_aircod_receive_lifewater:
			if(mRtuS_master_complete == mde_mRtu_master_reveive_status(MASTER_COMP))
			{
				updataWord &= (~SET_LIFE_HOTWATER_BIT);
				aircod_status = mm_aircod_idle;
                                 pbc_reload_timerClock(&updata_delay,UPDATA_DELAY);
			}
			else if((mRtuS_master_timeout | mRtuS_master_poterr) & mde_mRtu_master_reveive_status(MASTER_COMP))
			{
				updataWord &= (~SET_LIFE_HOTWATER_BIT);
				aircod_status = mm_aircod_idle;
                                 pbc_reload_timerClock(&updata_delay,UPDATA_DELAY);
			}
			break;
		
		case mm_aircod_tramsimt_cool_outwater_settemp: 
			if(mmRunS_idle == mde_mRtu_master_run_status(MASTER_COMP))
            {
                APP_pull_aricod_message(DRIVE_BOARD_COOL_OUTWATER_SETTEMP,&regDetails);
                mde_mRtu_master_cmd0x06_transmit(MASTER_COMP,MODBUS_ADDR,(DRIVE_BOARD_COOL_OUTWATER_SETTEMP-1),regDetails);
                aircod_status = mm_aircod_receive_cool_outwater_settemp;
            }
			break;
		case mm_aircod_receive_cool_outwater_settemp: 
			if(mRtuS_master_complete == mde_mRtu_master_reveive_status(MASTER_COMP))
			{
				updataWord &= (~SET_SUPPLY_COLD_OUTWATER_BIT);
				aircod_status = mm_aircod_idle;
                                 pbc_reload_timerClock(&updata_delay,UPDATA_DELAY);
			}
			else if((mRtuS_master_timeout | mRtuS_master_poterr) & mde_mRtu_master_reveive_status(MASTER_COMP))
			{
				updataWord &= (~SET_SUPPLY_COLD_OUTWATER_BIT);
				aircod_status = mm_aircod_idle;
                                 pbc_reload_timerClock(&updata_delay,UPDATA_DELAY);
			}
			break;
		
		case mm_aircod_tramsimt_hot_outwater_settemp: 
			if(mmRunS_idle == mde_mRtu_master_run_status(MASTER_COMP))
            {
                APP_pull_aricod_message(DRIVE_BOARD_HOT_OUTWATER_SETTEMP,&regDetails);
                mde_mRtu_master_cmd0x06_transmit(MASTER_COMP,MODBUS_ADDR,(DRIVE_BOARD_HOT_OUTWATER_SETTEMP-1),regDetails);
                aircod_status = mm_aircod_receive_hot_outwater_settemp;
            }

			break;
		case mm_aircod_receive_hot_outwater_settemp: 
			if(mRtuS_master_complete == mde_mRtu_master_reveive_status(MASTER_COMP))
			{
				updataWord &= (~SET_SUPPLY_HOT_OUTWATER_BIT);
				aircod_status = mm_aircod_idle;
                                 pbc_reload_timerClock(&updata_delay,UPDATA_DELAY);
			}
			else if((mRtuS_master_timeout | mRtuS_master_poterr) & mde_mRtu_master_reveive_status(MASTER_COMP))
			{
				updataWord &= (~SET_SUPPLY_HOT_OUTWATER_BIT);
				aircod_status = mm_aircod_idle;
                                 pbc_reload_timerClock(&updata_delay,UPDATA_DELAY);
			}
			break;

		case mm_aircod_tramsimt_two_way_value_state: 
			if(mmRunS_idle == mde_mRtu_master_run_status(MASTER_COMP))
            {
                APP_pull_aricod_message(DRIVE_BOARD_TWO_WAY_VALUE_STATE,&regDetails);
                mde_mRtu_master_cmd0x06_transmit(MASTER_COMP,MODBUS_ADDR,(DRIVE_BOARD_TWO_WAY_VALUE_STATE-1),regDetails);
                aircod_status = mm_aircod_receive_two_way_value_state;
            }
			break;
		case mm_aircod_receive_two_way_value_state:
			if(mRtuS_master_complete == mde_mRtu_master_reveive_status(MASTER_COMP))
			{
				updataWord &= (~TWO_WAY_STATE_BIT);
				aircod_status = mm_aircod_idle;
                                 pbc_reload_timerClock(&updata_delay,UPDATA_DELAY);
			}
			else if((mRtuS_master_timeout | mRtuS_master_poterr) & mde_mRtu_master_reveive_status(MASTER_COMP))
			{
				updataWord &= (~TWO_WAY_STATE_BIT);
				aircod_status = mm_aircod_idle;
                                 pbc_reload_timerClock(&updata_delay,UPDATA_DELAY);
			}
			break;
		case mm_aircod_tramsimt_indoor_message: 
			if(mmRunS_idle == mde_mRtu_master_run_status(MASTER_COMP))
            {
                APP_pull_aricod_message(DRIVE_BOARD_INDOOR_MESSAGE,&regDetails);
				
                mde_mRtu_master_cmd0x10_transmit(MASTER_COMP,MODBUS_ADDR,(DRIVE_BOARD_INDOOR_MESSAGE-1),4);
                app_push_send_reg_aircod(DRIVE_BOARD_INDOOR_MESSAGE-1,4);
                aircod_status = mm_aircod_receive_indoor_message;
            }
			break;
		case mm_aircod_receive_indoor_message: 
			if(mRtuS_master_complete == mde_mRtu_master_reveive_status(MASTER_COMP))
			{
				updataWord &= (~INSIDE_BACKWIND_HUMIDITY_BIT);
				aircod_status = mm_aircod_idle;
                                 pbc_reload_timerClock(&updata_delay,UPDATA_DELAY);
			}
			else if((mRtuS_master_timeout | mRtuS_master_poterr) & mde_mRtu_master_reveive_status(MASTER_COMP))
			{
				updataWord &= (~INSIDE_BACKWIND_HUMIDITY_BIT);
				aircod_status = mm_aircod_idle;
                                 pbc_reload_timerClock(&updata_delay,UPDATA_DELAY);
			}
			break;
		default:break;

    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

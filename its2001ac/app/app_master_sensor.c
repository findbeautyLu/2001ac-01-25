//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include ".\app_cfg.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define ProductType_AirRS485SENSOR            0x1001   //RS485环境传感器,四方代工
#define MRegaddr_ProductType                  0x2000   //产品类型
#define REG_DEVICE_INF                        0x2000
#define REG_MEASURE_DATA                      0x0006
#define REG_PM25                              0x0010
//-----------------------------------------------------------------------------
#define BitsOfOnceMeasurePM25                 0x0001     //测量一次PM2.5
#define Device_Addr                           0x01
#define MAX_SOILID_NUM                        0x05
//-----------------------------------------------------------------------------
//0x1001 RS485 环境传感器运行状态
typedef enum
{
    RSS1001_Idle                         =0x00,
    RSS1001_Transmit_ReadDevInf_0        =0x01,   //
    RSS1001_Receive_ReadDevInf_0         =0x02, 
    RSS1001_Transmit_ReadMeasureData_0   =0x03,
    RSS1001_Receive_ReadMeasureData_0    =0x04,
    RSS1001_Transmit_EnablePM25_0        =0x05,
    RSS1001_Receive_EnablePM25_0         =0x06,
    RSS1001_Transmit_DisablePM25_0       =0x07,
    RSS1001_Receive_DisablePM25_0        =0x08,
    RSS1001_Transmit_ReadDevInf_1        =0x09,   //
    RSS1001_Receive_ReadDevInf_1         =0x0A, 
    RSS1001_Transmit_ReadMeasureData_1   =0x0B,
    RSS1001_Receive_ReadMeasureData_1    =0x0C,
    RSS1001_Transmit_EnablePM25_1        =0x0D,
    RSS1001_Receive_EnablePM25_1         =0x0E,
    RSS1001_Transmit_DisablePM25_1       =0x0F,
    RSS1001_Receive_DisablePM25_1        =0x10,
    RSS1001_Transmit_ReadDevInf_2        =0x11,   //
    RSS1001_Receive_ReadDevInf_2         =0x12, 
    RSS1001_Transmit_ReadMeasureData_2   =0x13,
    RSS1001_Receive_ReadMeasureData_2    =0x14,
    RSS1001_Transmit_EnablePM25_2        =0x15,
    RSS1001_Receive_EnablePM25_2         =0x16,
    RSS1001_Transmit_DisablePM25_2       =0x17,
    RSS1001_Receive_DisablePM25_2        =0x18,
    RSS1001_Transmit_ReadDevInf_3        =0x19,   //
    RSS1001_Receive_ReadDevInf_3         =0x1A, 
    RSS1001_Transmit_ReadMeasureData_3   =0x1B,
    RSS1001_Receive_ReadMeasureData_3    =0x1C,
    RSS1001_Transmit_EnablePM25_3        =0x1D,
    RSS1001_Receive_EnablePM25_3         =0x1E,
    RSS1001_Transmit_DisablePM25_3       =0x1F,
    RSS1001_Receive_DisablePM25_3        =0x20,
    RSS1001_Transmit_ReadDevInf_4        =0x21,   //
    RSS1001_Receive_ReadDevInf_4         =0x22, 
    RSS1001_Transmit_ReadMeasureData_4   =0x23,
    RSS1001_Receive_ReadMeasureData_4    =0x24,
    RSS1001_Transmit_EnablePM25_4        =0x25,
    RSS1001_Receive_EnablePM25_4         =0x26,
    RSS1001_Transmit_DisablePM25_4       =0x27,
    RSS1001_Receive_DisablePM25_4        =0x28,
}RSS1001H_CommState_Def;

typedef struct
{
    uint16_t                 DeviceInf;       //
    uint8_t                  ReceiveIndex;
    int16_t                  Temperature;    
    uint16_t                 Humidity;
    uint16_t                 CO2Value;
    uint16_t                 PM25Value;
    uint16_t                 MeasureCW;     //
    bool                     PM25_Enable;
    uint8_t                  DevOnline;    //模块在线,0 offline,
}RSS1001HCtl_Def;

RSS1001HCtl_Def  app485Sensor[MAX_SOILID_NUM];
RSS1001H_CommState_Def  app485SensorStatus = RSS1001_Idle;

void Enable_OncePM25Measure(uint8_t in_soildNum)
{
    app485Sensor[in_soildNum].MeasureCW |= BitsOfOnceMeasurePM25;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void MeasurePM2_5Interval(void)
{
    uint8_t i = 0;
    macro_createTimer(measurePm25_delay,timerType_second,0);
    pbc_timerClockRun_task(&measurePm25_delay);
    if(pbc_pull_timerIsCompleted(&measurePm25_delay))
    {
        pbc_reload_timerClock(&measurePm25_delay,900);
        for(i = 0; i < MAX_SOILID_NUM;i++)
        {
            Enable_OncePM25Measure(i); 
        }       
    }
}


bool Push_RSS1001HCommState(RSS1001H_CommState_Def NewState)
{
    if(app485SensorStatus == RSS1001_Idle)
    {
       app485SensorStatus = NewState;
        return true;
    }
    return false;
}

int16_t Pull_RSS1001H_Temperature(uint8_t in_soildNum)
{
    return(app485Sensor[in_soildNum].Temperature);
}
//-----------------------------------------------------------------------------
uint16_t Pull_RSS1001H_Humidity(uint8_t in_soildNum)
{
    return(app485Sensor[in_soildNum].Humidity);
}
//-----------------------------------------------------------------------------
uint16_t Pull_RSS1001H_CO2(uint8_t in_soildNum)
{
    return(app485Sensor[in_soildNum].CO2Value);
}
//-----------------------------------------------------------------------------
uint16_t Pull_RSS1001H_PM25(uint8_t in_soildNum)
{
    return(app485Sensor[in_soildNum].PM25Value);
}
//-----------------------------------------------------------------------------
bool Pull_RSS1001H_OnlineState(uint8_t in_soildNum)
{
    if(0!=app485Sensor[in_soildNum].DevOnline)
    {
        return(true);
    }
    return(false);
}

macro_createTimer(PM25NotVaildTime_0,timerType_second,0);
macro_createTimer(PM25NotVaildTime_1,timerType_second,0);
macro_createTimer(PM25NotVaildTime_2,timerType_second,0);
macro_createTimer(PM25NotVaildTime_3,timerType_second,0);
macro_createTimer(PM25NotVaildTime_4,timerType_second,0);

void app_push_receive_reg_sensor485(uint8_t in_soildNum,uint16_t RegAddr,uint8_t RegLength)
{
    uint16_t regDetails;
    while(RegLength)
    {
        if(mde_pull_mRtu_receiveReg(MASTER_SENSOR,RegAddr,&regDetails))
        {
            switch(app485Sensor[in_soildNum].DeviceInf)
            {
                case ProductType_AirRS485SENSOR:
                {
                    switch(RegAddr)
                    {
                        case 0x0006:   
                        {
                            break;
                        }
                        case 0x0007:   //PM2.5 ,大气
                        {
                            if(in_soildNum == 0)
                            {
                                if((app485Sensor[0].PM25_Enable)&&(pbc_pull_timerIsCompleted(&PM25NotVaildTime_0)))
                                {  
                                    app485Sensor[0].PM25Value=regDetails;                               
                                }
                            }
                            else if(in_soildNum == 1)
                            {
                                if((app485Sensor[1].PM25_Enable)&&(pbc_pull_timerIsCompleted(&PM25NotVaildTime_1)))
                                {  
                                    app485Sensor[1].PM25Value=regDetails;                               
                                }
                            }
                            else if(in_soildNum == 2)
                            {
                                if((app485Sensor[2].PM25_Enable)&&(pbc_pull_timerIsCompleted(&PM25NotVaildTime_2)))
                                {  
                                    app485Sensor[2].PM25Value=regDetails;                               
                                }
                            }
                            else if(in_soildNum == 3)
                            {
                                if((app485Sensor[3].PM25_Enable)&&(pbc_pull_timerIsCompleted(&PM25NotVaildTime_3)))
                                {  
                                    app485Sensor[3].PM25Value=regDetails;                               
                                }
                            }
                            else if(in_soildNum == 4)
                            {
                                if((app485Sensor[4].PM25_Enable)&&(pbc_pull_timerIsCompleted(&PM25NotVaildTime_4)))
                                {  
                                    app485Sensor[4].PM25Value=regDetails;                               
                                }
                            }
                            break;
                        }
                        case 0x0008:  //CO2
                        {
                            app485Sensor[in_soildNum].CO2Value=regDetails;   
                            break;
                        }
                        case 0x0009:
                        {
                            break;
                        } 
                        case 0x000A:
                        { 
                            app485Sensor[in_soildNum].Temperature=regDetails;        
                            app485Sensor[in_soildNum].Temperature = (app485Sensor[in_soildNum].Temperature - 500); //0.1度
                            break;
                        } 
                        case 0x000B:
                        {   
                            app485Sensor[in_soildNum].Humidity = regDetails/10;   //1% 分辨率   
                            if((regDetails%10)>4)
                            {
                                app485Sensor[in_soildNum].Humidity = app485Sensor[in_soildNum].Humidity + 1;
                            }
                            break;
                        } 
                        case MRegaddr_ProductType:
                        { 
                            app485Sensor[in_soildNum].DeviceInf = regDetails;    
                            break;
                        } 
                        default:
                        {
                            break;
                        }
                    }  
                }
                default:
                {
                    switch(RegAddr)
                    {
                        case MRegaddr_ProductType:
                        {
                            app485Sensor[in_soildNum].DeviceInf = regDetails;    
                            break;
                        }     
                        default:
                        {
                            break;
                        }
                    }  
                    break;
                }
            }
        }
        RegAddr++;
        RegLength--;
    }
}

void app_master_sensor_task(void)
{
    sdt_int16u reg_addr,reg_length;
    uint8_t i = 0;
    macro_createTimer(ReadInterval,timerType_millisecond,2000);
    macro_createTimer(PM25MeasureT_0,timerType_second,0);
    macro_createTimer(PM25MeasureT_1,timerType_second,0);
    macro_createTimer(PM25MeasureT_2,timerType_second,0);
    macro_createTimer(PM25MeasureT_3,timerType_second,0);
    macro_createTimer(PM25MeasureT_4,timerType_second,0);

    pbc_timerClockRun_task(&ReadInterval);

    pbc_timerClockRun_task(&PM25MeasureT_0);
    pbc_timerClockRun_task(&PM25NotVaildTime_0);
    pbc_timerClockRun_task(&PM25MeasureT_1);
    pbc_timerClockRun_task(&PM25NotVaildTime_1);
    pbc_timerClockRun_task(&PM25MeasureT_2);
    pbc_timerClockRun_task(&PM25NotVaildTime_2);
    pbc_timerClockRun_task(&PM25MeasureT_3);
    pbc_timerClockRun_task(&PM25NotVaildTime_3);
    pbc_timerClockRun_task(&PM25MeasureT_4);
    pbc_timerClockRun_task(&PM25NotVaildTime_4);

    mde_mRtu_master_task();
    MeasurePM2_5Interval();
    switch (app485SensorStatus)
    {
        case RSS1001_Idle:
        {
            if(pbc_pull_timerIsCompleted(&ReadInterval))
            {
                pbc_reload_timerClock(&ReadInterval,2000);
                app485SensorStatus = RSS1001_Transmit_ReadDevInf_0;
            }
            else
            {
                for(i = 0; i < MAX_SOILID_NUM;i++)
                {
                    if(ProductType_AirRS485SENSOR==app485Sensor[i].DeviceInf)
                    {
                        if(BitsOfOnceMeasurePM25&app485Sensor[i].MeasureCW)
                        {
                            if(i == 0)
                            {
                                if(Push_RSS1001HCommState(RSS1001_Transmit_EnablePM25_0))
                                {
                                    app485Sensor[i].MeasureCW&=~BitsOfOnceMeasurePM25;
                                    app485Sensor[i].PM25_Enable=true;
                                    pbc_reload_timerClock(&PM25MeasureT_0,90);
                                    pbc_reload_timerClock(&PM25NotVaildTime_0,8);
                                }
                            }
                            else if(i == 1)
                            {
                                if(Push_RSS1001HCommState(RSS1001_Transmit_EnablePM25_1))
                                {
                                    app485Sensor[i].MeasureCW&=~BitsOfOnceMeasurePM25;
                                    app485Sensor[i].PM25_Enable=true;
                                    pbc_reload_timerClock(&PM25MeasureT_1,90);
                                    pbc_reload_timerClock(&PM25NotVaildTime_1,8);
                                }
                            }
                            else if(i == 2)
                            {
                                if(Push_RSS1001HCommState(RSS1001_Transmit_EnablePM25_2))
                                {
                                    app485Sensor[i].MeasureCW&=~BitsOfOnceMeasurePM25;
                                    app485Sensor[i].PM25_Enable=true;
                                    pbc_reload_timerClock(&PM25MeasureT_2,90);
                                    pbc_reload_timerClock(&PM25NotVaildTime_2,8);
                                }
                            }
                            else if(i == 3)
                            {
                                if(Push_RSS1001HCommState(RSS1001_Transmit_EnablePM25_3))
                                {
                                    app485Sensor[i].MeasureCW&=~BitsOfOnceMeasurePM25;
                                    app485Sensor[i].PM25_Enable=true;
                                    pbc_reload_timerClock(&PM25MeasureT_3,90);
                                    pbc_reload_timerClock(&PM25NotVaildTime_3,8);
                                }
                            }
                            else if(i == 4)
                            {
                                if(Push_RSS1001HCommState(RSS1001_Transmit_EnablePM25_4))
                                {
                                    app485Sensor[i].MeasureCW&=~BitsOfOnceMeasurePM25;
                                    app485Sensor[i].PM25_Enable=true;
                                    pbc_reload_timerClock(&PM25MeasureT_4,90);
                                    pbc_reload_timerClock(&PM25NotVaildTime_4,8);
                                }
                            }
                        }
                        if(app485Sensor[i].PM25_Enable)
                        {
                            if(i == 0)
                            {
                                if(pbc_pull_timerIsCompleted(&PM25MeasureT_0))
                                {
                                    if(Push_RSS1001HCommState(RSS1001_Transmit_DisablePM25_0))
                                    {
                                        app485Sensor[i].PM25_Enable=false;
                                    }
                                }
                            }
                            else if(i == 1)
                            {
                                if(pbc_pull_timerIsCompleted(&PM25MeasureT_1))
                                {
                                    if(Push_RSS1001HCommState(RSS1001_Transmit_DisablePM25_1))
                                    {
                                        app485Sensor[i].PM25_Enable=false;
                                    }
                                }
                            }
                            else if(i == 2)
                            {
                                if(pbc_pull_timerIsCompleted(&PM25MeasureT_2))
                                {
                                    if(Push_RSS1001HCommState(RSS1001_Transmit_DisablePM25_2))
                                    {
                                        app485Sensor[i].PM25_Enable=false;
                                    }
                                }
                            }
                            else if(i == 3)
                            {
                                if(pbc_pull_timerIsCompleted(&PM25MeasureT_3))
                                {
                                    if(Push_RSS1001HCommState(RSS1001_Transmit_DisablePM25_3))
                                    {
                                        app485Sensor[i].PM25_Enable=false;
                                    }
                                }
                            }
                            else if(i == 4)
                            {
                                if(pbc_pull_timerIsCompleted(&PM25MeasureT_4))
                                {
                                    if(Push_RSS1001HCommState(RSS1001_Transmit_DisablePM25_4))
                                    {
                                        app485Sensor[i].PM25_Enable=false;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            break;
        } 
        case RSS1001_Transmit_ReadDevInf_0:
        {
            if(ProductType_AirRS485SENSOR==app485Sensor[0].DeviceInf)
            {
                app485SensorStatus = RSS1001_Transmit_ReadMeasureData_0;
            }
            else 
            {
                if(mmRunS_idle == mde_mRtu_master_run_status(MASTER_SENSOR))
                {
                    mde_mRtu_master_cmd0x03_transmit(MASTER_SENSOR,0x01,REG_DEVICE_INF,2);
                    app485SensorStatus = RSS1001_Receive_ReadDevInf_0;
                }
            }
            break;
        }
        case RSS1001_Receive_ReadDevInf_0:
        {
            if(mRtuS_master_complete == mde_mRtu_master_reveive_status(MASTER_SENSOR))
            {
                app485SensorStatus = RSS1001_Transmit_ReadDevInf_1;
                app485Sensor[0].DevOnline = 5;
                mde_pull_mRtu_register_add_len(MASTER_SENSOR,&reg_addr,&reg_length);
                app_push_receive_reg_sensor485(0,reg_addr,reg_length);
            }
            else if((mRtuS_master_timeout | mRtuS_master_poterr) & mde_mRtu_master_reveive_status(MASTER_SENSOR))
            {
                app485SensorStatus = RSS1001_Transmit_ReadDevInf_1;
                if(app485Sensor[0].DevOnline)
                {
                    app485Sensor[0].DevOnline--;
                }
            }
            break;
        }
        case RSS1001_Transmit_ReadMeasureData_0:
        {
            if(mmRunS_idle == mde_mRtu_master_run_status(MASTER_SENSOR))
            {
                mde_mRtu_master_cmd0x03_transmit(MASTER_SENSOR,0x01,REG_MEASURE_DATA,10);
                app485SensorStatus = RSS1001_Receive_ReadMeasureData_0;
            }
            break;
        }
        case RSS1001_Receive_ReadMeasureData_0:
        {
            if(mRtuS_master_complete == mde_mRtu_master_reveive_status(MASTER_SENSOR))
            {
                app485SensorStatus = RSS1001_Transmit_ReadDevInf_1;
                app485Sensor[0].DevOnline = 5;
                mde_pull_mRtu_register_add_len(MASTER_SENSOR,&reg_addr,&reg_length);
                app_push_receive_reg_sensor485(0,reg_addr,reg_length);
            }
            else if((mRtuS_master_timeout | mRtuS_master_poterr) & mde_mRtu_master_reveive_status(MASTER_SENSOR))
            {
                app485SensorStatus = RSS1001_Transmit_ReadDevInf_1;
                if(app485Sensor[0].DevOnline)
                {
                    app485Sensor[0].DevOnline--;
                }
            }
            break;
        }
        case RSS1001_Transmit_EnablePM25_0:
        {
            if(mmRunS_idle == mde_mRtu_master_run_status(MASTER_SENSOR))
            {
                mde_mRtu_master_cmd0x06_transmit(MASTER_SENSOR,0x01,REG_PM25,0xff00);
                app485SensorStatus =RSS1001_Receive_EnablePM25_0;
            }
            break;
        }
        case RSS1001_Receive_EnablePM25_0:
        {
            if(mRtuS_master_complete == mde_mRtu_master_reveive_status(MASTER_SENSOR))
            {
                app485SensorStatus = RSS1001_Idle;
                app485Sensor[0].DevOnline = 5;
                mde_pull_mRtu_register_add_len(MASTER_SENSOR,&reg_addr,&reg_length);
                app_push_receive_reg_sensor485(0,reg_addr,reg_length);
            }
            else if((mRtuS_master_timeout | mRtuS_master_poterr) & mde_mRtu_master_reveive_status(MASTER_SENSOR))
            {
                app485SensorStatus = RSS1001_Idle;
                if(app485Sensor[0].DevOnline)
                {
                    app485Sensor[0].DevOnline--;
                }
            }
            break;
        }
        case RSS1001_Transmit_DisablePM25_0:
        {
            if(mmRunS_idle == mde_mRtu_master_run_status(MASTER_SENSOR))
            {
                mde_mRtu_master_cmd0x06_transmit(MASTER_SENSOR,0x01,REG_PM25,0x0000);
                app485SensorStatus = RSS1001_Receive_DisablePM25_0;
            }
            break;
        }
        case RSS1001_Receive_DisablePM25_0:
        {
            if(mRtuS_master_complete == mde_mRtu_master_reveive_status(MASTER_SENSOR))
            {
                app485SensorStatus = RSS1001_Idle;
                app485Sensor[0].DevOnline = 5;
                mde_pull_mRtu_register_add_len(MASTER_SENSOR,&reg_addr,&reg_length);
                app_push_receive_reg_sensor485(0,reg_addr,reg_length);
            }
            else if((mRtuS_master_timeout | mRtuS_master_poterr) & mde_mRtu_master_reveive_status(MASTER_SENSOR))
            {
                app485SensorStatus = RSS1001_Idle;
                if(app485Sensor[0].DevOnline)
                {
                    app485Sensor[0].DevOnline--;
                }
            }
            break;
        }
        case RSS1001_Transmit_ReadDevInf_1:
        {
            if(ProductType_AirRS485SENSOR==app485Sensor[1].DeviceInf)
            {
                app485SensorStatus = RSS1001_Transmit_ReadMeasureData_1;
            }
            else 
            {
                if(mmRunS_idle == mde_mRtu_master_run_status(MASTER_SENSOR))
                {
                    mde_mRtu_master_cmd0x03_transmit(MASTER_SENSOR,0x02,REG_DEVICE_INF,2);
                    app485SensorStatus = RSS1001_Receive_ReadDevInf_1;
                }
            }
            break;
        }
        case RSS1001_Receive_ReadDevInf_1:
        {
            if(mRtuS_master_complete == mde_mRtu_master_reveive_status(MASTER_SENSOR))
            {
                app485SensorStatus = RSS1001_Transmit_ReadDevInf_2;
                app485Sensor[1].DevOnline = 5;
                mde_pull_mRtu_register_add_len(MASTER_SENSOR,&reg_addr,&reg_length);
                app_push_receive_reg_sensor485(1,reg_addr,reg_length);
            }
            else if((mRtuS_master_timeout | mRtuS_master_poterr) & mde_mRtu_master_reveive_status(MASTER_SENSOR))
            {
                app485SensorStatus = RSS1001_Transmit_ReadDevInf_2;
                if(app485Sensor[1].DevOnline)
                {
                    app485Sensor[1].DevOnline--;
                }
            }
            break;
        }
        case RSS1001_Transmit_ReadMeasureData_1:
        {
            if(mmRunS_idle == mde_mRtu_master_run_status(MASTER_SENSOR))
            {
                mde_mRtu_master_cmd0x03_transmit(MASTER_SENSOR,0x02,REG_MEASURE_DATA,10);
                app485SensorStatus = RSS1001_Receive_ReadMeasureData_1;
            }
            break;
        }
        case RSS1001_Receive_ReadMeasureData_1:
        {
            if(mRtuS_master_complete == mde_mRtu_master_reveive_status(MASTER_SENSOR))
            {
                app485SensorStatus = RSS1001_Transmit_ReadDevInf_2;
                app485Sensor[1].DevOnline = 5;
                mde_pull_mRtu_register_add_len(MASTER_SENSOR,&reg_addr,&reg_length);
                app_push_receive_reg_sensor485(1,reg_addr,reg_length);
            }
            else if((mRtuS_master_timeout | mRtuS_master_poterr) & mde_mRtu_master_reveive_status(MASTER_SENSOR))
            {
                app485SensorStatus = RSS1001_Transmit_ReadDevInf_2;
                if(app485Sensor[1].DevOnline)
                {
                    app485Sensor[1].DevOnline--;
                }
            }
            break;
        }
        case RSS1001_Transmit_EnablePM25_1:
        {
            if(mmRunS_idle == mde_mRtu_master_run_status(MASTER_SENSOR))
            {
                mde_mRtu_master_cmd0x06_transmit(MASTER_SENSOR,0x02,REG_PM25,0xff00);
                app485SensorStatus =RSS1001_Receive_EnablePM25_1;
            }
            break;
        }
        case RSS1001_Receive_EnablePM25_1:
        {
            if(mRtuS_master_complete == mde_mRtu_master_reveive_status(MASTER_SENSOR))
            {
                app485SensorStatus = RSS1001_Idle;
                app485Sensor[1].DevOnline = 5;
                mde_pull_mRtu_register_add_len(MASTER_SENSOR,&reg_addr,&reg_length);
                app_push_receive_reg_sensor485(1,reg_addr,reg_length);
            }
            else if((mRtuS_master_timeout | mRtuS_master_poterr) & mde_mRtu_master_reveive_status(MASTER_SENSOR))
            {
                app485SensorStatus = RSS1001_Idle;
                if(app485Sensor[1].DevOnline)
                {
                    app485Sensor[1].DevOnline--;
                }
            }
            break;
        }
        case RSS1001_Transmit_DisablePM25_1:
        {
            if(mmRunS_idle == mde_mRtu_master_run_status(MASTER_SENSOR))
            {
                mde_mRtu_master_cmd0x06_transmit(MASTER_SENSOR,0x02,REG_PM25,0x0000);
                app485SensorStatus = RSS1001_Receive_DisablePM25_1;
            }
            break;
        }
        case RSS1001_Receive_DisablePM25_1:
        {
            if(mRtuS_master_complete == mde_mRtu_master_reveive_status(MASTER_SENSOR))
            {
                app485SensorStatus = RSS1001_Idle;
                app485Sensor[1].DevOnline = 5;
                mde_pull_mRtu_register_add_len(MASTER_SENSOR,&reg_addr,&reg_length);
                app_push_receive_reg_sensor485(1,reg_addr,reg_length);
            }
            else if((mRtuS_master_timeout | mRtuS_master_poterr) & mde_mRtu_master_reveive_status(MASTER_SENSOR))
            {
                app485SensorStatus = RSS1001_Idle;
                if(app485Sensor[1].DevOnline)
                {
                    app485Sensor[1].DevOnline--;
                }
            }
            break;
        }
        case RSS1001_Transmit_ReadDevInf_2:
        {
            if(ProductType_AirRS485SENSOR==app485Sensor[2].DeviceInf)
            {
                app485SensorStatus = RSS1001_Transmit_ReadMeasureData_2;
            }
            else 
            {
                if(mmRunS_idle == mde_mRtu_master_run_status(MASTER_SENSOR))
                {
                    mde_mRtu_master_cmd0x03_transmit(MASTER_SENSOR,0x03,REG_DEVICE_INF,2);
                    app485SensorStatus = RSS1001_Receive_ReadDevInf_2;
                }
            }
            break;
        }
        case RSS1001_Receive_ReadDevInf_2:
        {
            if(mRtuS_master_complete == mde_mRtu_master_reveive_status(MASTER_SENSOR))
            {
                app485SensorStatus = RSS1001_Transmit_ReadDevInf_3;
                app485Sensor[2].DevOnline = 5;
                mde_pull_mRtu_register_add_len(MASTER_SENSOR,&reg_addr,&reg_length);
                app_push_receive_reg_sensor485(2,reg_addr,reg_length);
            }
            else if((mRtuS_master_timeout | mRtuS_master_poterr) & mde_mRtu_master_reveive_status(MASTER_SENSOR))
            {
                app485SensorStatus = RSS1001_Transmit_ReadDevInf_3;
                if(app485Sensor[2].DevOnline)
                {
                    app485Sensor[2].DevOnline--;
                }
            }
            break;
        }
        case RSS1001_Transmit_ReadMeasureData_2:
        {
            if(mmRunS_idle == mde_mRtu_master_run_status(MASTER_SENSOR))
            {
                mde_mRtu_master_cmd0x03_transmit(MASTER_SENSOR,0x03,REG_MEASURE_DATA,10);
                app485SensorStatus = RSS1001_Receive_ReadMeasureData_2;
            }
            break;
        }
        case RSS1001_Receive_ReadMeasureData_2:
        {
            if(mRtuS_master_complete == mde_mRtu_master_reveive_status(MASTER_SENSOR))
            {
                app485SensorStatus = RSS1001_Transmit_ReadDevInf_3;
                app485Sensor[2].DevOnline = 5;
                mde_pull_mRtu_register_add_len(MASTER_SENSOR,&reg_addr,&reg_length);
                app_push_receive_reg_sensor485(2,reg_addr,reg_length);
            }
            else if((mRtuS_master_timeout | mRtuS_master_poterr) & mde_mRtu_master_reveive_status(MASTER_SENSOR))
            {
                app485SensorStatus = RSS1001_Transmit_ReadDevInf_3;
                if(app485Sensor[2].DevOnline)
                {
                    app485Sensor[2].DevOnline--;
                }
            }
            break;
        }
        case RSS1001_Transmit_EnablePM25_2:
        {
            if(mmRunS_idle == mde_mRtu_master_run_status(MASTER_SENSOR))
            {
                mde_mRtu_master_cmd0x06_transmit(MASTER_SENSOR,0x03,REG_PM25,0xff00);
                app485SensorStatus =RSS1001_Receive_EnablePM25_2;
            }
            break;
        }
        case RSS1001_Receive_EnablePM25_2:
        {
            if(mRtuS_master_complete == mde_mRtu_master_reveive_status(MASTER_SENSOR))
            {
                app485SensorStatus = RSS1001_Idle;
                app485Sensor[2].DevOnline = 5;
                mde_pull_mRtu_register_add_len(MASTER_SENSOR,&reg_addr,&reg_length);
                app_push_receive_reg_sensor485(2,reg_addr,reg_length);
            }
            else if((mRtuS_master_timeout | mRtuS_master_poterr) & mde_mRtu_master_reveive_status(MASTER_SENSOR))
            {
                app485SensorStatus = RSS1001_Idle;
                if(app485Sensor[2].DevOnline)
                {
                    app485Sensor[2].DevOnline--;
                }
            }
            break;
        }
        case RSS1001_Transmit_DisablePM25_2:
        {
            if(mmRunS_idle == mde_mRtu_master_run_status(MASTER_SENSOR))
            {
                mde_mRtu_master_cmd0x06_transmit(MASTER_SENSOR,0x03,REG_PM25,0x0000);
                app485SensorStatus = RSS1001_Receive_DisablePM25_2;
            }
            break;
        }
        case RSS1001_Receive_DisablePM25_2:
        {
            if(mRtuS_master_complete == mde_mRtu_master_reveive_status(MASTER_SENSOR))
            {
                app485SensorStatus = RSS1001_Idle;
                app485Sensor[2].DevOnline = 5;
                mde_pull_mRtu_register_add_len(MASTER_SENSOR,&reg_addr,&reg_length);
                app_push_receive_reg_sensor485(2,reg_addr,reg_length);
            }
            else if((mRtuS_master_timeout | mRtuS_master_poterr) & mde_mRtu_master_reveive_status(MASTER_SENSOR))
            {
                app485SensorStatus = RSS1001_Idle;
                if(app485Sensor[2].DevOnline)
                {
                    app485Sensor[2].DevOnline--;
                }
            }
            break;
        }
        case RSS1001_Transmit_ReadDevInf_3:
        {
            if(ProductType_AirRS485SENSOR==app485Sensor[3].DeviceInf)
            {
                app485SensorStatus = RSS1001_Transmit_ReadMeasureData_3;
            }
            else 
            {
                if(mmRunS_idle == mde_mRtu_master_run_status(MASTER_SENSOR))
                {
                    mde_mRtu_master_cmd0x03_transmit(MASTER_SENSOR,0x04,REG_DEVICE_INF,2);
                    app485SensorStatus = RSS1001_Receive_ReadDevInf_3;
                }
            }
            break;
        }
        case RSS1001_Receive_ReadDevInf_3:
        {
            if(mRtuS_master_complete == mde_mRtu_master_reveive_status(MASTER_SENSOR))
            {
                app485SensorStatus = RSS1001_Transmit_ReadDevInf_4;
                app485Sensor[3].DevOnline = 5;
                mde_pull_mRtu_register_add_len(MASTER_SENSOR,&reg_addr,&reg_length);
                app_push_receive_reg_sensor485(3,reg_addr,reg_length);
            }
            else if((mRtuS_master_timeout | mRtuS_master_poterr) & mde_mRtu_master_reveive_status(MASTER_SENSOR))
            {
                app485SensorStatus = RSS1001_Transmit_ReadDevInf_4;
                if(app485Sensor[3].DevOnline)
                {
                    app485Sensor[3].DevOnline--;
                }
            }
            break;
        }
        case RSS1001_Transmit_ReadMeasureData_3:
        {
            if(mmRunS_idle == mde_mRtu_master_run_status(MASTER_SENSOR))
            {
                mde_mRtu_master_cmd0x03_transmit(MASTER_SENSOR,0x04,REG_MEASURE_DATA,10);
                app485SensorStatus = RSS1001_Receive_ReadMeasureData_3;
            }
            break;
        }
        case RSS1001_Receive_ReadMeasureData_3:
        {
            if(mRtuS_master_complete == mde_mRtu_master_reveive_status(MASTER_SENSOR))
            {
                app485SensorStatus = RSS1001_Transmit_ReadDevInf_4;
                app485Sensor[3].DevOnline = 5;
                mde_pull_mRtu_register_add_len(MASTER_SENSOR,&reg_addr,&reg_length);
                app_push_receive_reg_sensor485(3,reg_addr,reg_length);
            }
            else if((mRtuS_master_timeout | mRtuS_master_poterr) & mde_mRtu_master_reveive_status(MASTER_SENSOR))
            {
                app485SensorStatus = RSS1001_Transmit_ReadDevInf_4;
                if(app485Sensor[3].DevOnline)
                {
                    app485Sensor[3].DevOnline--;
                }
            }
            break;
        }
        case RSS1001_Transmit_EnablePM25_3:
        {
            if(mmRunS_idle == mde_mRtu_master_run_status(MASTER_SENSOR))
            {
                mde_mRtu_master_cmd0x06_transmit(MASTER_SENSOR,0x04,REG_PM25,0xff00);
                app485SensorStatus =RSS1001_Receive_EnablePM25_3;
            }
            break;
        }
        case RSS1001_Receive_EnablePM25_3:
        {
            if(mRtuS_master_complete == mde_mRtu_master_reveive_status(MASTER_SENSOR))
            {
                app485SensorStatus = RSS1001_Idle;
                app485Sensor[3].DevOnline = 5;
                mde_pull_mRtu_register_add_len(MASTER_SENSOR,&reg_addr,&reg_length);
                app_push_receive_reg_sensor485(3,reg_addr,reg_length);
            }
            else if((mRtuS_master_timeout | mRtuS_master_poterr) & mde_mRtu_master_reveive_status(MASTER_SENSOR))
            {
                app485SensorStatus = RSS1001_Idle;
                if(app485Sensor[3].DevOnline)
                {
                    app485Sensor[3].DevOnline--;
                }
            }
            break;
        }
        case RSS1001_Transmit_DisablePM25_3:
        {
            if(mmRunS_idle == mde_mRtu_master_run_status(MASTER_SENSOR))
            {
                mde_mRtu_master_cmd0x06_transmit(MASTER_SENSOR,0x04,REG_PM25,0x0000);
                app485SensorStatus = RSS1001_Receive_DisablePM25_3;
            }
            break;
        }
        case RSS1001_Receive_DisablePM25_3:
        {
            if(mRtuS_master_complete == mde_mRtu_master_reveive_status(MASTER_SENSOR))
            {
                app485SensorStatus = RSS1001_Idle;
                app485Sensor[3].DevOnline = 5;
                mde_pull_mRtu_register_add_len(MASTER_SENSOR,&reg_addr,&reg_length);
                app_push_receive_reg_sensor485(3,reg_addr,reg_length);
            }
            else if((mRtuS_master_timeout | mRtuS_master_poterr) & mde_mRtu_master_reveive_status(MASTER_SENSOR))
            {
                app485SensorStatus = RSS1001_Idle;
                if(app485Sensor[3].DevOnline)
                {
                    app485Sensor[3].DevOnline--;
                }
            }
            break;
        }
        case RSS1001_Transmit_ReadDevInf_4:
        {
            if(ProductType_AirRS485SENSOR==app485Sensor[4].DeviceInf)
            {
                app485SensorStatus = RSS1001_Transmit_ReadMeasureData_4;
            }
            else 
            {
                if(mmRunS_idle == mde_mRtu_master_run_status(MASTER_SENSOR))
                {
                    mde_mRtu_master_cmd0x03_transmit(MASTER_SENSOR,0x05,REG_DEVICE_INF,2);
                    app485SensorStatus = RSS1001_Receive_ReadDevInf_4;
                }
            }
            break;
        }
        case RSS1001_Receive_ReadDevInf_4:
        {
            if(mRtuS_master_complete == mde_mRtu_master_reveive_status(MASTER_SENSOR))
            {
                app485SensorStatus = RSS1001_Idle;
                app485Sensor[4].DevOnline = 5;
                mde_pull_mRtu_register_add_len(MASTER_SENSOR,&reg_addr,&reg_length);
                app_push_receive_reg_sensor485(4,reg_addr,reg_length);
            }
            else if((mRtuS_master_timeout | mRtuS_master_poterr) & mde_mRtu_master_reveive_status(MASTER_SENSOR))
            {
                app485SensorStatus = RSS1001_Idle;
                if(app485Sensor[4].DevOnline)
                {
                    app485Sensor[4].DevOnline--;
                }
            }
            break;
        }
        case RSS1001_Transmit_ReadMeasureData_4:
        {
            if(mmRunS_idle == mde_mRtu_master_run_status(MASTER_SENSOR))
            {
                mde_mRtu_master_cmd0x03_transmit(MASTER_SENSOR,0x05,REG_MEASURE_DATA,10);
                app485SensorStatus = RSS1001_Receive_ReadMeasureData_4;
            }
            break;
        }
        case RSS1001_Receive_ReadMeasureData_4:
        {
            if(mRtuS_master_complete == mde_mRtu_master_reveive_status(MASTER_SENSOR))
            {
                app485SensorStatus = RSS1001_Idle;
                app485Sensor[4].DevOnline = 5;
                mde_pull_mRtu_register_add_len(MASTER_SENSOR,&reg_addr,&reg_length);
                app_push_receive_reg_sensor485(4,reg_addr,reg_length);
            }
            else if((mRtuS_master_timeout | mRtuS_master_poterr) & mde_mRtu_master_reveive_status(MASTER_SENSOR))
            {
                app485SensorStatus = RSS1001_Idle;
                if(app485Sensor[4].DevOnline)
                {
                    app485Sensor[4].DevOnline--;
                }
            }
            break;
        }
        case RSS1001_Transmit_EnablePM25_4:
        {
            if(mmRunS_idle == mde_mRtu_master_run_status(MASTER_SENSOR))
            {
                mde_mRtu_master_cmd0x06_transmit(MASTER_SENSOR,0x05,REG_PM25,0xff00);
                app485SensorStatus =RSS1001_Receive_EnablePM25_4;
            }
            break;
        }
        case RSS1001_Receive_EnablePM25_4:
        {
            if(mRtuS_master_complete == mde_mRtu_master_reveive_status(MASTER_SENSOR))
            {
                app485SensorStatus = RSS1001_Idle;
                app485Sensor[4].DevOnline = 5;
                mde_pull_mRtu_register_add_len(MASTER_SENSOR,&reg_addr,&reg_length);
                app_push_receive_reg_sensor485(4,reg_addr,reg_length);
            }
            else if((mRtuS_master_timeout | mRtuS_master_poterr) & mde_mRtu_master_reveive_status(MASTER_SENSOR))
            {
                app485SensorStatus = RSS1001_Idle;
                if(app485Sensor[4].DevOnline)
                {
                    app485Sensor[4].DevOnline--;
                }
            }
            break;
        }
        case RSS1001_Transmit_DisablePM25_4:
        {
            if(mmRunS_idle == mde_mRtu_master_run_status(MASTER_SENSOR))
            {
                mde_mRtu_master_cmd0x06_transmit(MASTER_SENSOR,0x05,REG_PM25,0x0000);
                app485SensorStatus = RSS1001_Receive_DisablePM25_4;
            }
            break;
        }
        case RSS1001_Receive_DisablePM25_4:
        {
            if(mRtuS_master_complete == mde_mRtu_master_reveive_status(MASTER_SENSOR))
            {
                app485SensorStatus = RSS1001_Idle;
                app485Sensor[4].DevOnline = 5;
                mde_pull_mRtu_register_add_len(MASTER_SENSOR,&reg_addr,&reg_length);
                app_push_receive_reg_sensor485(4,reg_addr,reg_length);
            }
            else if((mRtuS_master_timeout | mRtuS_master_poterr) & mde_mRtu_master_reveive_status(MASTER_SENSOR))
            {
                app485SensorStatus = RSS1001_Idle;
                if(app485Sensor[4].DevOnline)
                {
                    app485Sensor[4].DevOnline--;
                }
            }
            break;
        }
        default:
        {
            break;
        }          
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*******************************************************************************
*
*	模块名称 : SD3078底层模块
*	文件名称 : BSP_SD3078.c
*	版    本 : V1.0
*	说    明 : 1、模块独立，不访问其他任何模块
*                  2、供Mod_SD3078调用
                   3、BSP层处理SD3078和IO口有关的操作。
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0   2018-07-7  zhoujh  正式发布
*	Copyright (C), 2015-2022,   menredGroup
*            
*******************************************************************************/
#include "stdint.h"
#include <stdbool.h>
#include "bsp_i2c.h"
#include "bsp_sd3078.h"



uint8_t oneByteHexToBcdCode(uint8_t byte)
{
    uint8_t bcdData=0;
    
    if(byte > 99)
    {
        byte = 99;
    }   
    bcdData = byte / 10;
    bcdData <<= 4;
    bcdData |= (byte % 10);
    return(bcdData);
}

uint8_t oneByteBcdcodeToHex(uint8_t bcdData)
{
    uint8_t HexData;
        
    HexData = 0;
    HexData = bcdData&0x0F;
    HexData += (bcdData >> 4) * 10;
    return(HexData);
}


#define SD3078_REG_RTC_ADD         0x00
#define SD3078_WRITE_MODE          0X64
#define SD3078_READ_MODE           0X65

#define SD0378_REG_UNLOCK_1_ADD    0x10
#define SD0378_CMD_UNLOCK_1_DAT    0x80
#define SD0378_REG_UNLOCK_2_ADD    0x0f
#define SD0378_CMD_UNLOCK_2_DAT    0xff

#define SD0378_REG_LOCK_1_ADD      0x10
#define SD0378_CMD_LOCK_1_DAT      0x00
#define SD0378_REG_LOCK_2_ADD      0x0f
#define SD0378_CMD_LOCK_2_DAT      0x7b


//bool bsp_sd3078_unlock(void)
//{
//    bsp_i2c_start();  
//    if(bsp_i2c_writeOneByte(SD3078_WRITE_MODE)) 
//    {
//        bsp_i2c_writeOneByte(SD0378_REG_UNLOCK_1_ADD);  //0x10
//        bsp_i2c_writeOneByte(SD0378_CMD_UNLOCK_1_ADD);  //0x80
//    }
//    else
//    {
//        
//        return false;
//    }
//     
//    bsp_i2c_start();  
//    if(bsp_i2c_writeOneByte(SD3078_WRITE_MODE)) 
//    {
//        bsp_i2c_writeOneByte(SD0378_REG_UNLOCK_2_ADD);  //0x0f
//        bsp_i2c_writeOneByte(SD0378_CMD_UNLOCK_2_ADD);  //0xff
//    }
//    else
//    {
//        bsp_i2c_stop();
//        return false;
//    }  
//    return true;
//}


bool bsp_sd3078_write3Byte(uint8_t *ptByte)
{
    bsp_i2c_start();
    uint8_t i = 0;
    for(;i < 3;i++)
    {
        if(!bsp_i2c_writeOneByte(ptByte[i]))
        {
            bsp_i2c_stop();
            return false;
        }
    }
    return true;
}

#define UNLOCK_CMD_CNT 6
bool bsp_sd3078_unlock(void)
{
    uint8_t cmdBuf[UNLOCK_CMD_CNT] = {SD3078_WRITE_MODE,SD0378_REG_UNLOCK_1_ADD,\
      SD0378_CMD_UNLOCK_1_DAT,SD3078_WRITE_MODE,SD0378_REG_UNLOCK_2_ADD,SD0378_CMD_UNLOCK_2_DAT};
    if(!bsp_sd3078_write3Byte(&cmdBuf[0]))
    {
        bsp_i2c_stop();
        return false;
    }
    if(!bsp_sd3078_write3Byte(&cmdBuf[UNLOCK_CMD_CNT / 2]))
    {
        bsp_i2c_stop();
        return false;
    }         
    return true;
}

#define LOCK_CMD_CNT 4
bool bsp_sd3078_lock(void)
{
    uint8_t cmdBuf[LOCK_CMD_CNT] = {SD3078_WRITE_MODE,SD0378_REG_LOCK_2_ADD,\
      SD0378_CMD_LOCK_2_DAT,SD0378_CMD_LOCK_1_DAT,};
    bool rightFlag = false;
    if(bsp_sd3078_write3Byte(&cmdBuf[0]))
    {
        bsp_i2c_writeOneByte(cmdBuf[LOCK_CMD_CNT - 1]);
        rightFlag = true;
    }
    bsp_i2c_stop();
    return rightFlag;
}

bool bsp_sd3078_Charge_Enable(void)
{
     if(bsp_sd3078_unlock())            
     { 
        uint8_t cmdBuf[3] = {SD3078_WRITE_MODE,0x18,0x82};
        if(!bsp_sd3078_write3Byte(&cmdBuf[0]))
        {
            bsp_i2c_stop();
            return false;
        }     
        bsp_sd3078_lock();                       //关闭写使能
     }
     return true;
}


bool bsp_sd3078_readRealTime(sd3078Para_t *_out_rtc)
{
    uint8_t buf[7];
    uint8_t i = 0;
    bsp_i2c_start();
    if(bsp_i2c_writeOneByte(SD3078_READ_MODE))
    {
        for(;i < 6;i++)
        {
            buf[i] = bsp_i2c_readOneByte(I2C_ACK);
        }
        buf[6] = bsp_i2c_readOneByte(I2C_NACK);
        buf[2] &= 0x7f;
        
        for(i = 0;i < 7;i++)
        {
            _out_rtc->buf[i] = oneByteBcdcodeToHex(buf[i]);
        }      
        bsp_i2c_stop();
        return true;
    }
    return false;    
}

bool bsp_sd3078_writeRealTime(sd3078Para_t *rtc)
{      
    uint8_t buf[7];
    uint8_t i = 0;
    for(;i < 7;i++)
    {
        buf[i] = oneByteHexToBcdCode(rtc->buf[i]);
    }
    
    if(bsp_sd3078_unlock())
    {
        bsp_i2c_start();
        bsp_i2c_writeOneByte(SD3078_WRITE_MODE);
        bsp_i2c_writeOneByte(SD3078_REG_RTC_ADD);
        buf[2] |= 0x80; 
        for(i = 0;i < 7;i++)
        {       
          bsp_i2c_writeOneByte(buf[i]);
        }   
        bsp_sd3078_lock();
        return true;
    }
    return false;   
}

bool bsp_sd3078_configure(void)
{
    bool rtcDatErrFlag = false;
    sd3078Para_t realRtc;
    bsp_i2c_configure();   
    
    if(!bsp_sd3078_Charge_Enable())
    {
        return false;
    }
    if(bsp_sd3078_readRealTime(&realRtc))
    {
        if(realRtc.second > 59)
        {
            rtcDatErrFlag = true;
        }
          
        if(realRtc.minite > 59)
        {
            rtcDatErrFlag = true;
        }
          
        if(realRtc.hour>23)
        {
            rtcDatErrFlag = true;
        }
          
        if(realRtc.week>06)
        {
            rtcDatErrFlag = true;
        }
          
        if(realRtc.day>31)
        {
            rtcDatErrFlag = true;
        }
          
        if(realRtc.month>12)
        {
            rtcDatErrFlag = true;
        }
          
        if(realRtc.year>99)
        {
            rtcDatErrFlag = true;
        }
          
        if(rtcDatErrFlag)
        {
            realRtc.second = 0; 
            realRtc.minite = 0;
            realRtc.hour   = 12;
            realRtc.week   = 03;
            realRtc.day    = 13;
            realRtc.month  = 1;
            realRtc.year   = 21;
            bsp_sd3078_writeRealTime(&realRtc);
        }
        return true;
    }
    return false;
}



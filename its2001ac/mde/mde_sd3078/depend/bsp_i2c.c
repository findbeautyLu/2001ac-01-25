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

//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#include ".\bsp_i2c.h"
#include "stm32f10x.h"
//------------------------------E N D-------------------------------------------

#define SDA_SD3078_PORT   GPIOC
#define SDA_SD3078_PIN    GPIO_Pin_14

#define SCL_SD3078_PORT   GPIOC
#define SCL_SD3078_PIN    GPIO_Pin_13


void Delay_IIC(uint16_t delay)
{
    while(delay)
    {
        delay--;
    }
}
//------------------------------E N D-------------------------------------------

void bsp_i2c_setSdaOut(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; 

    GPIO_InitStructure.GPIO_Pin =  SDA_SD3078_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(SDA_SD3078_PORT, &GPIO_InitStructure);//推挽输出
	Delay_IIC(20);
}

void bsp_i2c_setSdaIn(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; 

    GPIO_InitStructure.GPIO_Pin =  SDA_SD3078_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(SDA_SD3078_PORT, &GPIO_InitStructure);
	Delay_IIC(20);
}

void bsp_i2c_configure(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;    
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = SDA_SD3078_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(SDA_SD3078_PORT, &GPIO_InitStructure);
	GPIO_SetBits(SDA_SD3078_PORT,SDA_SD3078_PIN);
	
    GPIO_InitStructure.GPIO_Pin = SCL_SD3078_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(SCL_SD3078_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(SCL_SD3078_PORT,SCL_SD3078_PIN);
}


bool bsp_i2c_readSda(void)
{
    if(GPIO_ReadInputDataBit(SDA_SD3078_PORT, SDA_SD3078_PIN))
    {
        return true;
    }
    return false;
}

void bsp_i2c_setSdaHigh(void)
{
    GPIO_SetBits(SDA_SD3078_PORT,SDA_SD3078_PIN);
	Delay_IIC(20);
}

void bsp_i2c_setSdaLow(void)
{
    GPIO_ResetBits(SDA_SD3078_PORT,SDA_SD3078_PIN);
	Delay_IIC(20);
}

void bsp_i2c_setSclHigh(void)
{
    GPIO_SetBits(SCL_SD3078_PORT,SCL_SD3078_PIN);
	Delay_IIC(20);
}

void bsp_i2c_setSclLow(void)
{
    GPIO_ResetBits(SCL_SD3078_PORT,SCL_SD3078_PIN);
	Delay_IIC(20);
}

void bsp_wait(void)
{
    volatile uint16_t i = 0;
    for(;i < 10;i++)
    {
        ;
    }
}

void bsp_i2c_start(void)
{   
    bsp_i2c_setSdaOut();
    bsp_i2c_setSdaHigh();
    bsp_i2c_setSclHigh();
    bsp_wait();
    bsp_i2c_setSdaLow();   
    bsp_wait();
    bsp_i2c_setSclLow();
}

void bsp_i2c_stop(void)
{
    bsp_i2c_setSdaOut();
    bsp_i2c_setSclLow();
    bsp_i2c_setSdaLow();
    bsp_wait();   
    bsp_i2c_setSclHigh();
    bsp_wait();   
    bsp_i2c_setSdaHigh();      
}

bool bsp_i2c_wait_ack(void)
{
    uint8_t i = 0;       
    bsp_i2c_setSdaIn();
    bsp_wait();
    bsp_i2c_setSclHigh();
    bsp_wait();
    while(bsp_i2c_readSda())
    {
        if(++i > 100)
        {
            return false;
        }        
    }   
    bsp_i2c_setSclLow();
    return true;
}

void bsp_i2c_send_ack(void)
{
    bsp_i2c_setSdaOut();
    bsp_i2c_setSdaLow();
    bsp_i2c_setSclHigh();
    bsp_wait();
    bsp_i2c_setSclLow();
    bsp_i2c_setSdaHigh();
}

void bsp_i2c_send_nack(void)
{
   bsp_i2c_setSdaOut();
   bsp_i2c_setSclHigh();
   bsp_wait();
   bsp_i2c_setSclLow();
   bsp_i2c_setSdaLow();
}

bool bsp_i2c_writeOneByte(uint8_t dat)
{
    uint8_t moveBit = 0x80; 
    bsp_i2c_setSdaOut();
    while(moveBit)
    {       
        if(moveBit & dat)
        {
            bsp_i2c_setSdaHigh();
        }
        else
        {       
            bsp_i2c_setSdaLow();
        }
        
        bsp_i2c_setSclHigh();         
        moveBit >>= 1;  
        bsp_wait();
        bsp_i2c_setSclLow();
        bsp_wait();
    }   
    if(bsp_i2c_wait_ack())
    {
        return true;
    }
    else
    {
        return false;
    }   
}

uint8_t bsp_i2c_readOneByte(bool ack)
{    
    uint8_t i = 0;	
    uint8_t data = 0;
    bsp_i2c_setSdaIn(); 
    bsp_wait();
    
    for(;i < 8;i++)
    {	 
        data <<= 1;   
        bsp_i2c_setSclHigh();
        //bsp_wait();
                  
        if(bsp_i2c_readSda())    
        {
            data |= 0x01;
        }
        bsp_i2c_setSclLow();
        bsp_wait();
    }
    if(ack)
    {       
        bsp_i2c_send_ack();
    }
    else
    {
        bsp_i2c_send_nack();
    }
    bsp_wait();
    return data;
}




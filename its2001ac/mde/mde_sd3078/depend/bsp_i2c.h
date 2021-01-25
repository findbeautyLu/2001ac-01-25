//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++++
#ifndef _BSP_I2C_H
#define _BSP_I2C_H
//------------------------------E N D-------------------------------------------
#include "stdint.h"
#include <stdbool.h>



#define I2C_ACK                    true
#define I2C_NACK                   false

void bsp_i2c_configure(void);
void bsp_i2c_start(void);
void bsp_i2c_stop(void);
uint8_t bsp_i2c_readOneByte(bool ack);
bool bsp_i2c_writeOneByte(uint8_t dat);



#endif

//-----------------------BSP_SD3078.h--END------------------------------------

#ifndef _I2C_H
#define _I2C_H

#include "stm32f0xx.h"
#include "uart.h"

void I2C1_Init(void);
uint8_t  I2C_Read(uint8_t DevAddr, uint8_t RegAddr, uint8_t* pBuffer, uint8_t NumByteToRead);
uint8_t  I2C_Write(uint8_t DevAddr, uint8_t RegAddr,uint8_t* pBuffer, uint8_t NumByteToWrite);

#endif 
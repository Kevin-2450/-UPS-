#include "i2c.h"

#define I2C_TIMEOUT 600

// I2C1初始化函数
void I2C1_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef I2C_InitStructure;

    // 使能GPIOA和I2C1的时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    // 配置PA9 (SCL) 和 PA10 (SDA) 为复用开漏输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 将PA9和PA10连接到I2C1
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_4);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_4);


	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;	
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_DigitalFilter = 0x01;
	I2C_InitStructure.I2C_OwnAddress1=0x00;
	//I2C_InitStructure.I2C_Timing = 0x00201D2B;//100Khz
	I2C_InitStructure.I2C_Timing =0x10950C27;//400Khz
	I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
	
	I2C_Init(I2C1, &I2C_InitStructure);	
	I2C_Cmd(I2C1, ENABLE);
} 



uint8_t  I2C_Read(uint8_t DevAddr, uint8_t RegAddr, uint8_t* pBuffer, uint8_t NumByteToRead)
{  
	uint16_t timeout = I2C_TIMEOUT;
	/* While the bus is busy */
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) != RESET)
	{
		if((timeout--) == 0)
		{
			return 0;
		}
	}
	/* Generate start & wait event detection */
	I2C_TransferHandling(I2C1, (DevAddr<<1), 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);
	timeout = I2C_TIMEOUT;
	while (I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET)
	{
		if((timeout--) == 0)
		{   
			return 0;
		}
	}
	I2C_SendData(I2C1, RegAddr);
	timeout = I2C_TIMEOUT;	
	while (I2C_GetFlagStatus(I2C1, I2C_FLAG_TC) == RESET)
	{
		if((timeout--) == 0)
		{
			return 0;
		}
	}
	/* Send STRAT condition a second time */  
	I2C_TransferHandling(I2C1, (DevAddr<<1), NumByteToRead,  I2C_AutoEnd_Mode, I2C_Generate_Start_Read);
	
	/* While there is data to be read */
	while(NumByteToRead)  
	{	
		timeout = I2C_TIMEOUT;
		
		while(I2C_GetFlagStatus(I2C1, I2C_FLAG_RXNE) == RESET)
		{
			if((timeout--) == 0)
			{
				return 0;
			}
		}
		/* Read a byte from the EEPROM */
		*pBuffer = I2C_ReceiveData(I2C1);
		/* Point to the next location where the byte read will be saved */
		pBuffer++; 
		/* Decrement the read bytes counter */
		NumByteToRead--;        
	}
	
	/* Enable Acknowledgement to be ready for another reception */
	timeout = I2C_TIMEOUT;
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF) == RESET)
	{
		if((timeout--) == 0)
		{
			return 0;
		}
	}
	return 1;
}



uint8_t  I2C_Write(uint8_t DevAddr, uint8_t RegAddr,uint8_t* pBuffer, uint8_t NumByteToWrite)
{
	uint16_t timeout = I2C_TIMEOUT;
	
	/* While the bus is busy */
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) != RESET)
	{
		if((timeout--) == 0)
		{
			return 0;
		}
	}
	
	/* Send Touch address for write */	
	I2C_TransferHandling(I2C1, (DevAddr<<1), 1, I2C_Reload_Mode, I2C_Generate_Start_Write);
	
	timeout = I2C_TIMEOUT;
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS)==RESET)
	{
		if((timeout--) == 0)
		{
			return 0;
		}
	}
	
	I2C_SendData(I2C1, RegAddr);
 
	timeout = I2C_TIMEOUT;
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TCR) == RESET)
	{
		if((timeout--) == 0)
		{
			return 0;
		}
	}
	
	I2C_TransferHandling(I2C1, (DevAddr<<1), NumByteToWrite, I2C_AutoEnd_Mode, I2C_No_StartStop);
	
	/* While there is data to be written */
	while(NumByteToWrite--)  
	{
		timeout = I2C_TIMEOUT;
		/* Test on EV8 and clear it */
		while (I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET)
		{
			if((timeout--) == 0)
			{
				return 0;
			}
		}
		/* Send the current byte */
		I2C_SendData(I2C1, *pBuffer); 
 
		/* Point to the next byte to be written */
		pBuffer++; 		
	}
	/* Send STOP condition */
	timeout = I2C_TIMEOUT;
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF) == RESET)
	{
		if((timeout--) == 0)
		{
			return 0;
		}
	}
	
	return 1;
	
}




#ifndef _UART_H
#define _UART_H

#include "stm32f0xx.h"
#include <stdio.h>

#define USART_REC_LEN 200

void UART1_Init(uint32_t baudrate);

#endif
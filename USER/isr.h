#ifndef _ISR_H
#define _ISR_H

#include "stm32f0xx.h"

#include "delay.h"

#include "key.h"
#include "led.h"

#include "sc8886.h"
#include "husb238.h"

#define OUTPUT   GPIOA->ODR&1<<4


typedef enum{
    ADAPTER_RUN,
    NO_ADAPTER_RUN,
    STOP
} SYSTEM_MODE;


extern SYSTEM_MODE mode;

void IIN_auto_set(void);
void ICHG_auto_set(void);

#endif


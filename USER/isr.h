#ifndef _ISR_H
#define _ISR_H

#include "stm32f0xx.h"

#include "delay.h"

#include "key.h"

#include "sc8886.h"
#include "husb238.h"

#define OUTPUT   GPIOA->ODR&1<<4


typedef enum{
    RUN,
    STOP
} SYSTEM_MODE;



#endif


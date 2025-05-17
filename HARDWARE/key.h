#ifndef _KEY_H
#define _KEY_H

#include "stm32f0xx.h"

#include "delay.h"

#define KEY    (GPIOA->IDR&1<<5)

void key_init(void);


#endif

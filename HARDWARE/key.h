#ifndef _KEY_H
#define _KEY_H

#include "stm32f0xx.h"

#include "delay.h"

#define KEY    (GPIOB->IDR&1<<1)

void key_init(void);
void Key_Task(void);

extern uint8_t key_press_flag;

#endif

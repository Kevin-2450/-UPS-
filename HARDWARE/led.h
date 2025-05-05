#ifndef _LED_H
#define _LED_H

#include "stm32f0xx.h"

void LED_Init(void);

void LED1_On(void);
void LED2_On(void);
void LED1_Off(void);
void LED2_Off(void);
void LED1_Toggle(void);
void LED2_Toggle(void);


#endif
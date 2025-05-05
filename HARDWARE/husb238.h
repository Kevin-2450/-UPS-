#ifndef _HUSB238_H
#define _HUSB238_H

#include "i2c.h"
#include "uart.h"
#include "sc8886.h"

#define HUSB238Adress 0x08

extern float pd_current,current_5V;
extern uint8_t pd_voltage;
extern uint8_t pd_require_voltage;

void husb238_read_all_reg(void);
void husb238_analysis(void);
void show_adapter_info(void);
void husb238_require_highest_voltage(void);

#endif

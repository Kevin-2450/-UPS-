#ifndef _SC8886_H
#define _SC8886_H


#include "stm32f0xx.h"
#include "i2c.h"
#include "uart.h"

#define SC8886Adress 0x6B
#define CHRG_OK (GPIOA->IDR & 1<<5)
#define CMPOUT  (GPIOA->IDR & 1<<0)

#define sc8886_read_reg(regaddress, num_to_read)    I2C_Read(SC8886Adress, regaddress, &sc8886_reg_cache[regaddress], num_to_read)
#define sc8886_write_reg(regaddress, num_to_write)  I2C_Write(SC8886Adress, regaddress, &sc8886_reg_cache[regaddress], num_to_write)


extern float VSYS, VBAT, VBUS, ICHG, IDCHG, IIN, CMP_IN;

void sc8886_init(void);

void sc8886_read_all_reg(void);
void sc8886_update_all_reg(void);
void print_cache_reg(void);

void sc8886_adc_start(void);
void sc8886_adc_read(void);

void set_charge_current(float current_ma);
void sc8886_set_iin(uint16_t ma);

void sc8886_performance_mode_enable(void);
void sc8886_performance_mode_disable(void);

#endif


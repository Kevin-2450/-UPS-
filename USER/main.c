#include "stm32f0xx.h"
#include "stm32f0xx_conf.h"

#include "led.h"

#include "tim.h"
#include "key.h" 

#include "isr.h"
#include "delay.h"
#include "uart.h"

#include "husb238.h"
#include "sc8886.h"

int main(){
    delay_init(SystemCoreClock/1000000);
    UART1_Init(115200);
    
    I2C1_Init();
    LED_Init();
    key_init();
    
    delay_ms(100);
    
    sc8886_init();
    TIM1_Init();

    printf("init successful @%dMHz\n",SystemCoreClock/1000000);
    
    while(1){

        
    }
}


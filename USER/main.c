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

void LP_Ctrl(void);

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
        LP_Ctrl();
        __WFI();    // 主循环等待中断，进入低功耗模式
    }
}


void LP_Ctrl(void){
    if(mode == NO_ADAPTER_RUN)
    {
        sc8886_performance_mode_enable();
    }
    else if(mode == STOP)
    {
        sc8886_performance_mode_disable();
        LED1_Off();
        LED2_Off();
        PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
        
        SystemInit();
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
        if(CHRG_OK){
            mode = ADAPTER_RUN;
        }
        else mode = NO_ADAPTER_RUN;
    }
}



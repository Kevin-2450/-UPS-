#include "isr.h"

SYSTEM_MODE mode = STOP;


// 适配器插入中断
void EXTI0_1_IRQHandler(void){
    if(EXTI_GetITStatus(EXTI_Line0) != RESET) {
        if(CHRG_OK){
            // printf("adapter plug in\n");
            mode = ADAPTER_RUN;
            TIM1->ARR = 999;
            TIM1->CNT = 0;
        }
        else {
            // printf("adapter plug out\n");
            if(OUTPUT){
                mode = NO_ADAPTER_RUN;
            }
            else mode = STOP;
            
            TIM1->ARR = 9999; 
            LED2_Off();
        }
        // 清除中断挂起标志
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}


// 按钮中断
void EXTI4_15_IRQHandler(void) {
    if(EXTI_GetITStatus(EXTI_Line5) != RESET) {
        // 用户中断处理代码（如清除标志、执行操作）

        delay_ms(5);
        if(KEY == 0){
            GPIOA->ODR ^= 1<<4;
        }
        if(~OUTPUT){
            if(CHRG_OK){
                mode = ADAPTER_RUN;
            }
            else mode = STOP;
        }
        // 清除中断挂起标志（必须！）
        EXTI_ClearITPendingBit(EXTI_Line5);
    }
}

// 定时器1更新中断服务函数
void TIM1_BRK_UP_TRG_COM_IRQHandler(void) {
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) {
//        sc8886_adc_start();
//        GPIOA->ODR ^= 1<<7;
//        LED1_Toggle();

        if(CHRG_OK){
            husb238_read_all_reg();
            husb238_analysis();
            husb238_require_highest_voltage();
            sc8886_adc_read();

            // 根据适配器能力设置输入限流
            IIN_auto_set();
            ICHG_auto_set();
            
            // 控制LED
            if(ICHG>0.1){
                LED2_On();
                LED1_Off();
            }
            else {
                LED1_On();
                LED2_Off();
            }
        } 
        else if(OUTPUT){
            sc8886_adc_read();
        }
        
        // 控制风扇
        if(IIN >1.1 || IDCHG > 1.1){
            GPIOA->ODR |= 1<<1;
        }
        else{
            GPIOA->ODR &= ~(1<<1);
        }


        // 清除中断标志
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }
}

// 定时器3更新中断
void TIM3_IRQHandler(void){
    // 检查中断标志
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){
        if(OUTPUT){
            show_adapter_info();
            printf("VBUS\tVSYS\tVBAT\tIIN\tICHG\tIDCHG\n%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n",VBUS,VSYS,VBAT,IIN,ICHG,IDCHG);
        }
        
        // 清除中断标志
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
    
}



void IIN_auto_set(void){
    if(pd_voltage!=0){
        // sc8886_set_iin((uint16_t)(pd_current*1000));
        uint8_t i = 6;
        while(pd_voltage != voltage_values[i])i--;
        sc8886_set_iin((uint16_t)(PD_Cur_Capability[i-1]*1000));
    }
    else sc8886_set_iin((uint16_t)(current_5V*1000));
}

void ICHG_auto_set(void){
    if(VBAT<16.77){
        set_charge_current(7000);
    }
    else {
        set_charge_current(0);
    }
    
}

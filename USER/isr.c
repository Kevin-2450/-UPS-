#include "isr.h"


void EXTI4_15_IRQHandler(void) {
    if(EXTI_GetITStatus(EXTI_Line5) != RESET) {
        // 用户中断处理代码（如清除标志、执行操作）
        if(CHRG_OK){
            printf("adapter plug in\n");
            
            GPIOA->ODR &= ~(1<<6);
//            delay_ms(1000);
            //set_charge_current(512);
        }
        else {
            printf("adapter plug out\n");
            GPIOA->ODR |= 1<<6;
        }
        // 清除中断挂起标志（必须！）
        EXTI_ClearITPendingBit(EXTI_Line5);
    }
}

// 定时器1更新中断服务函数
void TIM1_BRK_UP_TRG_COM_IRQHandler(void) {
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) {
        //sc8886_adc_start();

        if(CHRG_OK){
            husb238_read_all_reg();
            husb238_analysis();
            husb238_require_highest_voltage();
            
            // 根据适配器能力设置输入限流
            if(pd_voltage!=0){
                sc8886_set_iin((uint16_t)(pd_current*1000));
            }
            else sc8886_set_iin((uint16_t)(current_5V*1000));
        }
        
        sc8886_adc_read();

        // 清除中断标志
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }
}


// 按钮中断
void EXTI0_1_IRQHandler(void){
    delay_ms(5);
    if(EXTI_GetITStatus(EXTI_Line1) != RESET) {
        // 用户中断处理代码（如清除标志、执行操作）
        GPIOA->ODR ^= 1<<4;
        show_adapter_info();
        printf("VBUS\tVSYS\tVBAT\tIIN\tICHG\tIDCHG\tCMPIN\n%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n",VBUS,VSYS,VBAT,IIN,ICHG,IDCHG,CMP_IN);
        
        // 清除中断挂起标志（必须！）
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
//    if(EXTI_GetITStatus(EXTI_Line0) != RESET){
//        GPIOA->ODR ^= 1<<7;
//        EXTI_ClearITPendingBit(EXTI_Line1);
//    }
}
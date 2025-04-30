#include "isr.h"


void EXTI4_15_IRQHandler(void) {
    if(EXTI_GetITStatus(EXTI_Line5) != RESET) {
        // �û��жϴ�����루�������־��ִ�в�����
        if(CHRG_OK){
            printf("adapter plug in\n");
            
            GPIOA->ODR &= ~(1<<6);
        }
        else {
            printf("adapter plug out\n");
            GPIOA->ODR |= 1<<6;
        }
        // ����жϹ����־�����룡��
        EXTI_ClearITPendingBit(EXTI_Line5);
    }
}

// ��ʱ��1�����жϷ�����
void TIM1_BRK_UP_TRG_COM_IRQHandler(void) {
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) {
        //sc8886_adc_start();

        if(CHRG_OK){
            husb238_read_all_reg();
            husb238_analysis();
            husb238_require_highest_voltage();
            set_charge_current(4096);
            // ��������������������������
            if(pd_voltage!=0){
                sc8886_set_iin((uint16_t)(pd_current*1000));
            }
            else sc8886_set_iin((uint16_t)(current_5V*1000));
        }
        
        sc8886_adc_read();
        
        if(OUTPUT){
            show_adapter_info();
            printf("VBUS\tVSYS\tVBAT\tIIN\tICHG\tIDCHG\tCMPIN\n%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n",VBUS,VSYS,VBAT,IIN,ICHG,IDCHG,CMP_IN);
        }

        // ����жϱ�־
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }
}


// ��ť�ж�
void EXTI0_1_IRQHandler(void){
    delay_ms(5);
    if(EXTI_GetITStatus(EXTI_Line1) != RESET) {
        key_press_flag = 1;

        // ����жϹ����־
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}
#include "isr.h"

SYSTEM_MODE mode = STOP;

// �����������ж�
void EXTI4_15_IRQHandler(void) {
    if(EXTI_GetITStatus(EXTI_Line5) != RESET) {
        // �û��жϴ�����루�������־��ִ�в�����
        if(CHRG_OK){
            mode = ADAPTER_RUN;
            TIM1->ARR = 999;
            TIM1->CNT = 0;
        }
        else {
            if(OUTPUT){
                mode = NO_ADAPTER_RUN;
            }
            else mode = STOP;
            
            TIM1->ARR = 9999;
        }
        // ����жϹ����־�����룡��
        EXTI_ClearITPendingBit(EXTI_Line5);
    }
}

// ��ʱ��1�����жϷ�����
void TIM1_BRK_UP_TRG_COM_IRQHandler(void) {
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) {
//        sc8886_adc_start();
//        GPIOA->ODR ^= 1<<7;

        if(CHRG_OK){
            husb238_read_all_reg();
            husb238_analysis();
            husb238_require_highest_voltage();
            sc8886_adc_read();

            // ��������������������������
            IIN_auto_set();
            ICHG_auto_set();
            
            if(ICHG>0.1){
                LED1_On();
                LED2_Off();
            }
            else {
                LED2_On();
                LED1_Off();
            }
        }
        
        if(OUTPUT){
            sc8886_adc_read();
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
        if(KEY == 0){
            GPIOA->ODR ^= 1<<4;
        }
        if(~OUTPUT){
            if(CHRG_OK){
                mode = ADAPTER_RUN;
            }
            else mode = STOP;
        }
        // ����жϹ����־
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}


void IIN_auto_set(void){
    if(pd_voltage!=0){
        sc8886_set_iin((uint16_t)(pd_current*1000));
    }
    else sc8886_set_iin((uint16_t)(current_5V*1000));
}

void ICHG_auto_set(void){
    if(VBAT<16.77){
        set_charge_current(4000);
    }
    else {
        set_charge_current(0);
    }
    
}

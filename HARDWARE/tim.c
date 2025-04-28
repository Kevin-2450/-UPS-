#include "tim.h"

// ��ʱ��1��ʼ������
void TIM1_Init(void) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // ʹ�ܶ�ʱ��1��ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    // ��ʱ��1������������
    TIM_TimeBaseStructure.TIM_Period = 9999; // �Զ���װ��ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = 4799; // Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    // ʹ�ܶ�ʱ��1�����ж�
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

    // ����NVIC
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_UP_TRG_COM_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // ʹ�ܶ�ʱ��1
    TIM_Cmd(TIM1, ENABLE);
}





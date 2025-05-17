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
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // ʹ�ܶ�ʱ��1
    TIM_Cmd(TIM1, ENABLE);
}

void TIM3_Init(void) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // ʹ�ܶ�ʱ��1��ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    // ��ʱ��1������������
    TIM_TimeBaseStructure.TIM_Period = 9999; // �Զ���װ��ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = 4799; // Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    // ʹ�ܶ�ʱ��1�����ж�
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    // ����NVIC
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // ʹ�ܶ�ʱ��1
    TIM_Cmd(TIM3, ENABLE);
}

void TIM14_Init(void){
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // ʹ�ܶ�ʱ��1��ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);

    // ��ʱ��1������������
    TIM_TimeBaseStructure.TIM_Period = 9999; // �Զ���װ��ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = 4799; // Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);

    // ʹ�ܶ�ʱ��1�����ж�
    TIM_ITConfig(TIM14, TIM_IT_Update, ENABLE);

    // ����NVIC
    NVIC_InitStructure.NVIC_IRQChannel = TIM14_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // ʹ�ܶ�ʱ��1
    TIM_Cmd(TIM14, ENABLE);

}





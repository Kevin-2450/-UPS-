#include "key.h"


void key_init(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    
    // 1. ʹ��GPIOAʱ��
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    
    // 2. ����PA6��PA7
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;           // ����ģʽ
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;        // ��������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_1;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    // ����PB1�ж�
    // ʹ��SYSCFGʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    
    // ��PA5���ӵ�EXTI5
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource1);


    EXTI_InitStruct.EXTI_Line    = EXTI_Line1;          // ѡ��EXTI1
    EXTI_InitStruct.EXTI_Mode    = EXTI_Mode_Interrupt; // �ж�ģʽ
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; // �½��ش���
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;              // ʹ��EXTI��
    EXTI_Init(&EXTI_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel = EXTI0_1_IRQn;    
    NVIC_InitStruct.NVIC_IRQChannelPriority = 0x03;     // ���ȼ���0-3��
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;        // ʹ���ж�
    NVIC_Init(&NVIC_InitStruct);
}

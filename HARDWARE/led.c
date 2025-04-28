#include "led.h"

void LED_Init(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 1. ʹ��GPIOAʱ��
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    
    // 2. ����PA6��PA7
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_6 | GPIO_Pin_7;  // ͬʱ������������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;           // ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          // �������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;       // ����ٶ�
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;        // ��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 3. ��ѡ����ʼ�������ƽ
    GPIO_WriteBit(GPIOA, GPIO_Pin_6 | GPIO_Pin_7, Bit_SET); 
    GPIO_WriteBit(GPIOA,  GPIO_Pin_4 | GPIO_Pin_1, Bit_RESET);
}


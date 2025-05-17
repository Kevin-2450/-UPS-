#include "key.h"


void key_init(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    
    // 1. 使能GPIOA时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    
    // 2. 配置PA5
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;           // 输入模式
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;        // 无上下拉
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_1;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 配置PA5中断
    // 使能SYSCFG时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    
    // 将PA5连接到EXTI5
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource5);


    EXTI_InitStruct.EXTI_Line    = EXTI_Line5;          // 选择EXTI1
    EXTI_InitStruct.EXTI_Mode    = EXTI_Mode_Interrupt; // 中断模式
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; // 下降沿触发
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;              // 使能EXTI线
    EXTI_Init(&EXTI_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel = EXTI4_15_IRQn;    
    NVIC_InitStruct.NVIC_IRQChannelPriority = 0x03;     // 优先级（0-3）
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;        // 使能中断
    NVIC_Init(&NVIC_InitStruct);
}

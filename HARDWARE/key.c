#include "key.h"


void key_init(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    
    // 1. 使能GPIOA时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    
    // 2. 配置PA6和PA7
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;           // 输入模式
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;        // 无上下拉
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_1;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    // 配置PB1中断
    // 使能SYSCFG时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    
    // 将PA5连接到EXTI5
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource1);


    EXTI_InitStruct.EXTI_Line    = EXTI_Line1;          // 选择EXTI1
    EXTI_InitStruct.EXTI_Mode    = EXTI_Mode_Interrupt; // 中断模式
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; // 下降沿触发
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;              // 使能EXTI线
    EXTI_Init(&EXTI_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel = EXTI0_1_IRQn;    
    NVIC_InitStruct.NVIC_IRQChannelPriority = 0x03;     // 优先级（0-3）
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;        // 使能中断
    NVIC_Init(&NVIC_InitStruct);
}

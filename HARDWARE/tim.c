#include "tim.h"

// 定时器1初始化函数
void TIM1_Init(void) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 使能定时器1的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    // 定时器1基本参数配置
    TIM_TimeBaseStructure.TIM_Period = 9999; // 自动重装载值
    TIM_TimeBaseStructure.TIM_Prescaler = 4799; // 预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    // 使能定时器1更新中断
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

    // 配置NVIC
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_UP_TRG_COM_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 使能定时器1
    TIM_Cmd(TIM1, ENABLE);
}





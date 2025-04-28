#include "led.h"

void LED_Init(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 1. 使能GPIOA时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    
    // 2. 配置PA6和PA7
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_6 | GPIO_Pin_7;  // 同时配置两个引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;           // 输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;       // 输出速度
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;        // 无上下拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 3. 可选：初始化输出电平
    GPIO_WriteBit(GPIOA, GPIO_Pin_6 | GPIO_Pin_7, Bit_SET); 
    GPIO_WriteBit(GPIOA,  GPIO_Pin_4 | GPIO_Pin_1, Bit_RESET);
}


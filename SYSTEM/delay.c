#include "delay.h"

static uint32_t g_fac_us = 0;       /* us延时倍乘数 */

void delay_init(uint16_t sysclk)
    {    
    SysTick->CTRL |= (1 << 2);              /* SYSTICK使用内部时钟源,频率为HCLK*/    
g_fac_us = sysclk;                      /* 不论是否使用OS,g_fac_us都需要使用 */    
SysTick->CTRL |= 1 << 0;                /* 使能Systick */    
        SysTick->LOAD = 0X0FFFFFFF;             /* 注意systick计数器24位，所以这里设置最大重装载值 */
}
    
void delay_us(uint32_t nus)
    {    
        uint32_t ticks;    
        uint32_t told, tnow, tcnt = 0;    
        uint32_t reload = SysTick->LOAD;        /* LOAD的值 */    
        ticks = nus * g_fac_us;                 /* 需要的节拍数 */        
        told = SysTick->VAL;                    /* 刚进入时的计数器值 */    
    while (1)    
    {        
    tnow = SysTick->VAL;        
    if (tnow != told)        
    {            
    if (tnow < told)            
    {                
    tcnt += told - tnow;        /* 这里注意一下SYSTICK是一个递减的计数器就可以了 */            
    }            
    else            
    {                
    tcnt += reload - tnow + told;            
    }            
    told = tnow;            
    if (tcnt >= ticks)             
    {                
    break;                      /* 时间超过/等于要延迟的时间,则退出 */            
    }        
    }    
    }
    }

void delay_ms(uint16_t nms)
{    
    delay_us((uint32_t)(nms * 1000));                   /* 普通方式延时 */
}


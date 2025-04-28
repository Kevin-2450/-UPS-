#include "uart.h"

// 接收缓冲区
#define RX_BUFFER_SIZE 256
volatile uint8_t USART_RX_BUF[USART_REC_LEN];
volatile uint16_t USART_RX_STA = 0;

// UART1初始化函数
void UART1_Init(uint32_t baudrate) {
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 使能GPIOA和USART1的时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    // 配置PA2 (TX) 和 PA3 (RX) 为复用功能
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 将PA2和PA3连接到USART1
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);

    // 配置USART1
    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

    // 使能USART1接收中断
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    // 配置NVIC
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 使能USART1
    USART_Cmd(USART1, ENABLE);
}


int fputc(int ch, FILE *f)
{    
    // 等待发送缓冲区为空    
    while (!(USART1->ISR & USART_ISR_TXE));    
    // 发送字符    
    USART1->TDR = (ch & 0xFF);    
    return ch;
}


// USART1接收中断处理函数
void USART1_IRQHandler(void) {
    if (USART1->ISR & USART_ISR_RXNE)  
    // 检查接收缓冲区非空标志    
    {        
        uint8_t Res = USART1->RDR;  
        // 读取接收到的数据        
        if ((USART_RX_STA & 0x8000) == 0)  
        // 接收未完成        
        {            
            if (USART_RX_STA & 0x4000)  
            // 接收到了0x0d            
            {                
                if (Res != 0x0a)                   
                USART_RX_STA = 0;  // 接收错误，重新开始                
                else                    
                USART_RX_STA |= 0x8000;  // 接收完成了            
            }            
            else  // 还没收到0X0D            
            {                
                if (Res == 0x0d)                    
                USART_RX_STA |= 0x4000;                
                else                
                {                    
                    USART_RX_BUF[USART_RX_STA & 0X3FFF] = Res;                    
                    USART_RX_STA++;                    
                    if (USART_RX_STA > (USART_REC_LEN - 1))                        
                        USART_RX_STA = 0;  // 接收数据错误，重新开始接收                
                }            
            }        
        }    
    }
}
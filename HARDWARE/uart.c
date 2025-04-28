#include "uart.h"

// ���ջ�����
#define RX_BUFFER_SIZE 256
volatile uint8_t USART_RX_BUF[USART_REC_LEN];
volatile uint16_t USART_RX_STA = 0;

// UART1��ʼ������
void UART1_Init(uint32_t baudrate) {
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // ʹ��GPIOA��USART1��ʱ��
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    // ����PA2 (TX) �� PA3 (RX) Ϊ���ù���
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // ��PA2��PA3���ӵ�USART1
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);

    // ����USART1
    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

    // ʹ��USART1�����ж�
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    // ����NVIC
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // ʹ��USART1
    USART_Cmd(USART1, ENABLE);
}


int fputc(int ch, FILE *f)
{    
    // �ȴ����ͻ�����Ϊ��    
    while (!(USART1->ISR & USART_ISR_TXE));    
    // �����ַ�    
    USART1->TDR = (ch & 0xFF);    
    return ch;
}


// USART1�����жϴ�����
void USART1_IRQHandler(void) {
    if (USART1->ISR & USART_ISR_RXNE)  
    // �����ջ������ǿձ�־    
    {        
        uint8_t Res = USART1->RDR;  
        // ��ȡ���յ�������        
        if ((USART_RX_STA & 0x8000) == 0)  
        // ����δ���        
        {            
            if (USART_RX_STA & 0x4000)  
            // ���յ���0x0d            
            {                
                if (Res != 0x0a)                   
                USART_RX_STA = 0;  // ���մ������¿�ʼ                
                else                    
                USART_RX_STA |= 0x8000;  // ���������            
            }            
            else  // ��û�յ�0X0D            
            {                
                if (Res == 0x0d)                    
                USART_RX_STA |= 0x4000;                
                else                
                {                    
                    USART_RX_BUF[USART_RX_STA & 0X3FFF] = Res;                    
                    USART_RX_STA++;                    
                    if (USART_RX_STA > (USART_REC_LEN - 1))                        
                        USART_RX_STA = 0;  // �������ݴ������¿�ʼ����                
                }            
            }        
        }    
    }
}
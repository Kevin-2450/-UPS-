#include "sc8886.h"

uint8_t sc8886_reg_cache[60] = {0};

float VSYS, VBAT, VBUS, ICHG, IDCHG, IIN, CMP_IN;

void sc8886_reset(void){
    uint8_t tmp = 3<<5;
    I2C_Write(SC8886Adress,0x35,&tmp,1);
}

void sc8886_read_all_reg(void){
    I2C_Read(SC8886Adress,0x00,&sc8886_reg_cache,60);
    //sc8886_read_reg(0x00,60);
}


void sc8886_update_all_reg(void){
    //I2C_Write(SC8886Adress,0x00,&sc8886_reg_cache,60);
    sc8886_write_reg(0x00,60);
}


void print_cache_reg(void){
    for(uint16_t i =0;i<60;i++){
        printf("%d:%d\n",i,sc8886_reg_cache[i]);
    }
    printf("\n");
}


void set_charge_current(float current_ma){
    if(current_ma>8128)current_ma = 8128;
    else if(current_ma<64)current_ma = 64;
    
    uint8_t tmp = current_ma/64;
    
    sc8886_reg_cache[0x02] = tmp<<6;
    sc8886_reg_cache[0x03] = tmp>>2;
    
    //I2C_Write(SC8886Adress,0x02,&sc8886_reg_cache[0x02],2);
    sc8886_write_reg(0x02,2);
}


void sc8886_adc_start(void){
    //I2C_Write(SC8886Adress,0x3B,&sc8886_reg_cache[0x3B],1);
    sc8886_reg_cache[0x3B] |= 1<<6;
    sc8886_write_reg(0x3B,1);
}


void sc8886_adc_read(void){
    //I2C_Read(SC8886Adress,0x27,&sc8886_reg_cache[0x27],7);
    sc8886_read_reg(0x27,7);
    
    VBUS = 3.2 + (float)sc8886_reg_cache[0x27]*0.064;
    VSYS = 2.88 + (float)sc8886_reg_cache[0x2D]*0.064;
    VBAT = 2.88 + (float)sc8886_reg_cache[0x2C]*0.064;
    
    CMP_IN = (float)sc8886_reg_cache[0x2A]*0.012;
    
    IIN = (float)sc8886_reg_cache[0x2B]*0.05;
    ICHG = (float)sc8886_reg_cache[0x29]*0.064;
    IDCHG = (float)sc8886_reg_cache[0x28]*0.256;
}

void sc8886_performance_mode_enable(void){
    sc8886_reg_cache[0x01] &= ~(1<<7);
    //I2C_Write(SC8886Adress,0x01,&sc8886_reg_cache[0x01],1);
    sc8886_write_reg(0x01,1);
}

void sc8886_performance_mode_disable(void){
    sc8886_reg_cache[0x01] |= 1<<7;
    //I2C_Write(SC8886Adress,0x01,&sc8886_reg_cache[0x01],1);
    sc8886_write_reg(0x01,1);
}

void sc8886_set_iin(uint16_t ma){
    uint8_t tmp = ma/50 - 1;
    sc8886_reg_cache[0xF] = tmp&0x7F;
    sc8886_write_reg(0xF,1);
}

void sc8886_init(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    
    // 1. 使能GPIOA时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    
    // 2. 配置PA0和PA5
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;           // 输入模式
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;        // 无上下拉
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_1;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 配置PA5中断
    // 使能SYSCFG时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    
    // 将PA5连接到EXTI5
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource5);


    EXTI_InitStruct.EXTI_Line    = EXTI_Line5;          // 选择EXTI5
    EXTI_InitStruct.EXTI_Mode    = EXTI_Mode_Interrupt; // 中断模式
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling; // 双边沿触发
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;              // 使能EXTI线
    EXTI_Init(&EXTI_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel = EXTI4_15_IRQn;    // PA5使用EXTI4_15通道
    NVIC_InitStruct.NVIC_IRQChannelPriority = 0x01;     // 最高优先级（0-3）
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;        // 使能中断
    NVIC_Init(&NVIC_InitStruct);

//    // 配置PA0中断
//    // 使能SYSCFG时钟
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
//    
//    // 将PA5连接到EXTI5
//    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);


//    EXTI_InitStruct.EXTI_Line    = EXTI_Line0;          // 选择EXTI0
//    EXTI_InitStruct.EXTI_Mode    = EXTI_Mode_Interrupt; // 中断模式
//    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling; // 双边沿触发
//    EXTI_InitStruct.EXTI_LineCmd = ENABLE;              // 使能EXTI线
//    EXTI_Init(&EXTI_InitStruct);

//    NVIC_InitStruct.NVIC_IRQChannel = EXTI0_1_IRQn;    // PA0使用EXTI0_1通道
//    NVIC_InitStruct.NVIC_IRQChannelPriority = 0x03;     // 优先级（0-3）
//    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;        // 使能中断
//    NVIC_Init(&NVIC_InitStruct);
    
    // 初始化SC8886寄存器
    sc8886_reset();
    sc8886_read_all_reg();   // 先读进寄存器再进行其它操作       
    
//    sc8886_reg_cache[0x1] &= ~(1<<7);    // 使能performance模式
    sc8886_reg_cache[0x1] &= ~(1<<2);       // 关闭PFMmode
//    sc8886_reg_cache[0x31] |= 3<<2;      // 设置电流采样电阻5毫欧
    sc8886_reg_cache[0x0F] |= 20;        // 设置输入电流限制1A
//    sc8886_reg_cache[0x31] |= 1<<7;      // 设置IBAT输出
//    sc8886_reg_cache[0x35] |= 1<<3;       // 使能ICO模式
    sc8886_reg_cache[0x3A] |= 0x5F;      // adc使能
    sc8886_reg_cache[0x3B] |= 3<<6;      // 设置ADC循环转换
    sc8886_update_all_reg();
    
//    sc8886_read_all_reg();
//    print_cache_reg();
}


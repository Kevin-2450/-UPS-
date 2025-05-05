#include "husb238.h"

uint8_t husb238_reg_cache[10];
uint8_t pd_voltage = 0;             // ��ǰPD��ѹ
uint8_t pd_require_voltage = 0;     // ��ǰ�����PD��ѹ
float pd_current = 0;               // ��ǰPD������
float current_5V = 0;               // ��ǰ5V������

// �����ѹ���飬������Ӧ��ѹ����
const uint8_t voltage_values[] = {0, 5, 9, 12, 15, 18, 20};
// ����������飬������Ӧ��������
const float current_values[] = {0.5, 0.7, 1.0, 1.25, 1.5, 1.75, 2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 4.0, 4.5, 5.0};
// ����5V��������
const float current_values_5V[] = {1, 1.5, 2.4, 3};

uint8_t PD_Vol_Capability = 0;      // �洢֧�ֵ�PD��ѹ��λ 
float PD_Cur_Capability[6] = {0};   // �洢��ӦPD��λ֧�ֵ�������

const uint8_t pdo_select_value[] = {0, 1, 2, 3, 8, 9, 10};  


// ��ȡhusb238���мĴ���
void husb238_read_all_reg(void){
    if(!I2C_Read(HUSB238Adress,0x00,&husb238_reg_cache,10))printf("husb238 read err\n");
}


// ����husb238�Ĵ�����ֵ
void husb238_analysis(void){
    // ʶ��PD��ѹ����
    unsigned char voltage_code = (husb238_reg_cache[0] >> 4) & 0x0F;
    unsigned char current_code = husb238_reg_cache[0] & 0x0F;
    uint8_t current_code_5V = husb238_reg_cache[1] & 0x3;
    
    pd_voltage = voltage_values[voltage_code];
    pd_current = current_values[current_code];
    current_5V = current_values_5V[current_code_5V];
    
    for(uint8_t i = 0; i < 6; i++){
        // ��ȡPD��ѹ����
        PD_Vol_Capability |= (husb238_reg_cache[i+2]>>7)<<i;
        PD_Cur_Capability[i] = current_values[husb238_reg_cache[i+2]&0xF];
    }
}


void husb238_require_highest_voltage(void){
    if(pd_voltage!=0){
        uint8_t i = 5;
        while(!(PD_Vol_Capability&(1<<i)))i--;          // Ѱ����ߵ�ѹ
        pd_require_voltage = voltage_values[i+1];
        if(pd_require_voltage>pd_voltage){
            husb238_reg_cache[0x08] = pdo_select_value[i+1] << 4; 
            husb238_reg_cache[0x09] = 1<<0;
            I2C_Write(HUSB238Adress,0x08,&husb238_reg_cache[0x08],2);
        }
    }
    
}


void show_adapter_info(void){
    if(CHRG_OK){
        printf("\n----------------------\n");
        if(pd_voltage != 0){
            printf("PD Connected %dV@%.2fA\n",pd_voltage,pd_current);
            printf("PD Capability:\n");
            for(uint8_t i = 0; i<6; i++){
                if(PD_Vol_Capability & (1<<i)){
                    printf("\t%dV\t%.2fA\n",voltage_values[i+1],PD_Cur_Capability[i]);
                }
            }
        }
        else{
            printf("No PD Connection\n");
        }
        printf("5V Capability: %.1fA\n",current_5V);
        printf("----------------------\n");
    }
    else printf("No Adapter Connected\n");
}


#ifndef MQ2_H
#define MQ2_H

#include <stdint.h>

#define RL_VALUE 1000.0f               // Điện trở tải 1k Ohm
#define RO_CLEAN_AIR_FACTOR 9.8f       // Tỉ lệ Rs/Ro trong không khí sạch

extern float Ro;

void ADC1_Init(void);
uint16_t ADC1_Read(void);
float calculate_Rs(uint16_t adc_value);
float read_rs(void);
float get_ppm(float rs);

#endif

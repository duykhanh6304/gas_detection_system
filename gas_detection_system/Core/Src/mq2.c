#include "stm32f4xx.h"
#include "mq2.h"
#include "util.h"
#include <math.h>

float Ro = 1100;

// --- ADC ---
void ADC1_Init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;


    GPIOA->MODER |= (3 << (0 * 2)); // PA0 analog
    ADC1->SQR3 = 0;
    ADC1->SMPR2 |= (7 << 0);
    ADC1->CR2 |= ADC_CR2_ADON;
}

uint16_t ADC1_Read(void) {
    ADC1->CR2 |= ADC_CR2_SWSTART;
    while(!(ADC1->SR & ADC_SR_EOC));
    return ADC1->DR;
}

float calculate_Rs(uint16_t adc_value) {
    float voltage = (adc_value / 4095.0f) * 3.3f;
    if (voltage < 0.001f) voltage = 0.001f;
    return (float)RL_VALUE * (5.0f - voltage) / voltage;
}

float read_rs(void) {
    float rs_sum = 0;
    for (int i = 0; i < 50; i++) {
        rs_sum += calculate_Rs(ADC1_Read());
        delay_ms(5);
    }
    return (float)rs_sum / (50.0f);
}

float get_ppm(float rs) {

    float ratio = rs / (Ro);

    float ppm = 590.2f *powf(ratio,-2.041f);
    if(ppm > 20000) return 20000;
    return  ppm;
}


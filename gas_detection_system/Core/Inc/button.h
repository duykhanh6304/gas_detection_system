#ifndef BUTTON_H
#define BUTTON_H

#include "stm32f4xx.h"

// Button Pins
#define SW1_PIN  4
#define SW2_PIN  5

void Button_EXTI_Init(void);
void EXTI4_IRQHandler(void);
void EXTI9_5_IRQHandler(void);

#endif

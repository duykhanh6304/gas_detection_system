#ifndef BUZZER_H
#define BUZZER_H

#include "stm32f4xx.h"

#define BUZZER_PIN 6

void Buzzer_Init(void);
void Buzzer_ON(void);
void Buzzer_OFF(void);

#endif

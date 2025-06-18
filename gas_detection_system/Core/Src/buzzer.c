#include "stm32f4xx.h"
#include "buzzer.h"

void Buzzer_Init(void) {
    RCC->AHB1ENR |= (1 << 1); // Enable clock cho GPIOB

    // Set mode = output
    GPIOB->MODER &= ~(3 << (BUZZER_PIN * 2));
    GPIOB->MODER |=  (1 << (BUZZER_PIN * 2));

    // Push-pull
    GPIOB->OTYPER &= ~(1 << BUZZER_PIN);

    // High speed
    GPIOB->OSPEEDR |= (3 << (BUZZER_PIN * 2));

    // No pull
    GPIOB->PUPDR &= ~(3 << (BUZZER_PIN * 2));

    // Tắt mặc định
    Buzzer_OFF();
}

void Buzzer_ON (void){
	GPIOB->ODR |= (1 << BUZZER_PIN);
}

void Buzzer_OFF (void){
	GPIOB->ODR &= ~(1 << BUZZER_PIN);
}

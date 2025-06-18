#include "stm32f4xx.h"
#include "relay.h"

void Relay_Init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // Bật clock cho GPIOB

    // Cấu hình chân PB7 làm output
    GPIOB->MODER &= ~(3 << (RELAY_PIN * 2));  // Clear mode
    GPIOB->MODER |=  (1 << (RELAY_PIN * 2));  // Output mode

    GPIOB->OTYPER &= ~(1 << RELAY_PIN);       // Push-pull
    GPIOB->OSPEEDR |= (3 << (RELAY_PIN * 2)); // High speed
    GPIOB->PUPDR &= ~(3 << (RELAY_PIN * 2));  // No pull-up/pull-down

    Relay_OFF(); // Mặc định tắt
}

void Relay_ON(void) {
    GPIOB->ODR |= (1 << RELAY_PIN);
}

void Relay_OFF(void) {
    GPIOB->ODR &= ~(1 << RELAY_PIN);
}

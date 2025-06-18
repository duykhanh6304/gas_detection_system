#include "stm32f4xx.h"
#include "button.h"
#include "state.h"
#include "util.h"

extern void Buzzer_OFF(void);
extern void Relay_OFF(void);


void Button_EXTI_Init(void) {

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    // Cấu hình PB4, PB5 là input và pull-up
    GPIOB->MODER &= ~((3 << (SW1_PIN * 2)) | (3 << (SW2_PIN * 2)));   // Input mode
    GPIOB->PUPDR &= ~((3 << (SW1_PIN * 2)) | (3 << (SW2_PIN * 2)));   // Clear pull
    GPIOB->PUPDR |=  ((1 << (SW1_PIN * 2)) | (1 << (SW2_PIN * 2)));   // Pull-up

    // SYSCFG cấu hình EXTI4 và EXTI5 cho port B
    SYSCFG->EXTICR[1] &= ~((0xF << 0) | (0xF << 4));  // clear EXTI4, EXTI5
    SYSCFG->EXTICR[1] |=  ((1 << 0) | (1 << 4));      // EXTI4 = PB4, EXTI5 = PB5

    // Cấu hình EXTI4 và EXTI5 ngắt cạnh xuống
    EXTI->IMR  |= (1 << SW1_PIN) | (1 << SW2_PIN);  // Unmask EXTI4 và EXTI5
    EXTI->FTSR |= (1 << SW1_PIN) | (1 << SW2_PIN);  // Falling edge

    // Enable ngắt trong NVIC
    NVIC_EnableIRQ(EXTI4_IRQn);        // Cho EXTI4
    NVIC_EnableIRQ(EXTI9_5_IRQn);      // Cho EXTI5 đến EXTI9
    NVIC_SetPriority(EXTI4_IRQn, 0);   // Độ ưu tiên cao nhất
    NVIC_SetPriority(EXTI9_5_IRQn, 1); // Độ ưu tiên cao nhất
}

// Xử lý nút PB4 (SW1)
void EXTI4_IRQHandler(void) {
    if (EXTI->PR & (1 << SW1_PIN)) {
    	system_state ^= 1;
        EXTI->PR |= (1 << SW1_PIN);                  // Xóa cờ
    }
}
// Xử lý nút PB5 (SW2)
void EXTI9_5_IRQHandler(void) {
    if (EXTI->PR & (1 << SW2_PIN)) {
        EXTI->PR |= (1 << SW2_PIN);  // Xoá cờ ngắt EXTI5
        // Xử lý nút SW2 (PB5)
        system_state = STATE_STOPPED;
        Buzzer_OFF();
        Relay_OFF();
    }
}

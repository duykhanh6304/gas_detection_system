#include "stm32f4xx.h"
#include "led.h"
#include "util.h"

extern uint8_t system_state;
extern uint8_t warning_level;

void LED_Init(void) {
    RCC->AHB1ENR |= (1 << 1);  // Enable clock cho GPIOB

    for (int i = LED_BLUE_PIN; i <= LED_GREEN_PIN; i++) {
    	GPIOB->MODER &= ~(3 << (i * 2));  // clear 2 bit
		GPIOB->MODER |= (1 << (i * 2));
	}
}

void blink_red_led(float freq_hz, int duration_ms) {

    int total_blinks = (int)(freq_hz * duration_ms / 1000.0f);
    int delay_half = (int)(500.0f / freq_hz); // ms mỗi nửa chu kỳ

    // Tắt các LED khác
    GPIOA->ODR &= ~((1 << 6) | (1 << 7) | (1 << 8));

    for (int i = 0; i < total_blinks; i++) {
        GPIOB->ODR ^= (1 << 2); // Đảo trạng thái led đỏ
        delay_ms(delay_half);
        GPIOB->ODR ^= (1 << 2);
        delay_ms(delay_half);
    }
    GPIOB->ODR &= ~(1 << 2); // Đảm bảo tắt led đỏ khi xong
}

// Hàm tính tần số nháy LED đỏ dựa vào ppm khí gas so với ngưỡng nguy hiểm
float get_freq(float ppm, float ppm_danger) {
    float k = (ppm - ppm_danger) / (ppm_danger);  // 0 ~ tăng dần
    float freq = 2.0f + k * 8.0f;                 // 2Hz đến 10Hz
    if (freq > 10.0f) freq = 10.0f;
    if (freq < 2.0f) freq = 2.0f;
    return freq;
}

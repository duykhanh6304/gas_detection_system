#include "stm32f4xx.h"
#include "lcd_i2c.h"
#include "led.h"
#include "buzzer.h"
#include "relay.h"
#include "state.h"
#include "mq2.h"
#include "button.h"
#include "util.h"

float ppm = 0.0f;

void update_sensor_readings(void);
void update_lcd_display(void);
void update_outputs(void);

void update_sensor_readings(void) {
	if (system_state == STATE_STOPPED) {
	        ppm = 0.0f;
	        return;
	}
    float rs = read_rs();
    ppm = get_ppm(rs);
    warning_level = get_warning_level(ppm);
}

void update_lcd_display(void) {
    char line1[17], line2[17];
    sprintf(line1, "Gas: %.0f ppm", ppm);
    sprintf(line2, "Sys:%d Warn:%d", system_state, warning_level);

    LCD_SetCursor(0, 0); LCD_Print("                ");
    LCD_SetCursor(1, 0); LCD_Print("                ");
    LCD_SetCursor(0, 0); LCD_Print(line1);
    LCD_SetCursor(1, 0); LCD_Print(line2);
}

void update_outputs(void) {
	// Tắt hết các LED trước khi bật đúng cái cần thiết
	GPIOB->ODR &= ~((1 << LED_BLUE_PIN) | (1 << LED_YELLOW_PIN) | (1 << LED_RED_PIN) | (1 << LED_GREEN_PIN));
	GPIOB->ODR &= ~(1 << BUZZER_PIN);

	if (system_state == STATE_STOPPED) {
		GPIOB->ODR |= (1 << LED_GREEN_PIN);
		Relay_OFF();
		Buzzer_OFF();
		return;
	}

	if (warning_level == WARNING_HIGH) {
		Relay_ON();
		Buzzer_ON();
	} else {
		Relay_OFF();
		Buzzer_OFF();
	}

	switch (warning_level) {
		case WARNING_NONE:
			GPIOB->ODR |= (1 << LED_BLUE_PIN);
			break;
		case WARNING_LOW:
			GPIOB->ODR |= (1 << LED_YELLOW_PIN);
			break;
		case WARNING_MEDIUM:
		case WARNING_HIGH:
			break;
	}
}

int main(void) {
    // Khởi tạo hệ thống
    I2C1_Init();
    LCD_Init();
    LED_Init();
    Buzzer_Init();
    Relay_Init();
    ADC1_Init();
    Button_EXTI_Init();

    while (1) {
        update_sensor_readings();
        update_lcd_display();
        update_outputs();

		if (system_state == STATE_RUNNING && warning_level == WARNING_HIGH) {
			float freq = get_freq(ppm, 2000.0f);
			blink_red_led(freq, 1000);
		}
		if(system_state == STATE_RUNNING && warning_level == WARNING_MEDIUM){
			blink_red_led(1.0f, 1000);
		}
        delay_ms(20);
    }
}

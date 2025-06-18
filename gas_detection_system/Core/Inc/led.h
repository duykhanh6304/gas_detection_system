#ifndef LED_H
#define LED_H

#include <stdint.h>

#define LED_BLUE_PIN   0
#define LED_YELLOW_PIN 1
#define LED_RED_PIN    2
#define LED_GREEN_PIN  3

void LED_Init(void);
void blink_red_led(float freq_hz, int duration_ms);
float get_freq(float ppm, float ppm_danger);

#endif

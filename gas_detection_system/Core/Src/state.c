#include "stm32f4xx.h"
#include "state.h"

uint8_t volatile system_state = STATE_RUNNING;
uint8_t volatile warning_level;

uint8_t get_warning_level(float ppm) {
    if (ppm < 100) return WARNING_NONE;
    else if (ppm < 1000) return WARNING_LOW;
    else if (ppm < 2000) return WARNING_MEDIUM;
    else return WARNING_HIGH;
}

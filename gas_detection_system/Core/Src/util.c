#include "stm32f4xx.h"
#include "util.h"

void delay_ms(uint32_t ms) {
    SysTick->LOAD = 16000 - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = 5;
    for(uint32_t i = 0; i < ms; i++) {
        while((SysTick->CTRL & 0x10000) == 0);
    }
    SysTick->CTRL = 0;
}

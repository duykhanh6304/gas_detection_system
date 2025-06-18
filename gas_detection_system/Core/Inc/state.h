#ifndef STATE_H
#define STATE_H

#include <stdint.h>

// Các trạng thái hệ thống
#define STATE_STOPPED  0
#define STATE_RUNNING  1

// Cảnh báo gas
#define WARNING_NONE     0
#define WARNING_LOW      1
#define WARNING_MEDIUM   2
#define WARNING_HIGH     3

extern volatile uint8_t system_state;
extern volatile uint8_t warning_level;     // Mức độ cảnh báo hiện tại

uint8_t get_warning_level(float ppm);

#endif

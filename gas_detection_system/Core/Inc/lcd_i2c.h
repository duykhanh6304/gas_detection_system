#ifndef LCD_H
#define LCD_H

#include <stdint.h>

void I2C1_Init(void);
void I2C1_Write(uint8_t addr, uint8_t *data, uint8_t len);
void LCD_SendNibble(uint8_t nibble, uint8_t control);
void LCD_SendByte(uint8_t byte, uint8_t mode);
void LCD_Command(uint8_t cmd);
void LCD_Data(uint8_t data);
void LCD_Init(void);
void LCD_SetCursor(uint8_t row, uint8_t col);
void LCD_Print(char *str);

#endif

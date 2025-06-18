#include "stm32f4xx.h"
#include "lcd_i2c.h"
#include <string.h>

#define LCD_ADDR         0x27  // Địa chỉ I2C dịch trái 1 bit

// LCD Control Bits
#define LCD_BACKLIGHT    0x08
#define ENABLE           0x04
#define RW               0x00
#define RS               0x01

extern void delay_ms(uint32_t ms); // Hàm delay riêng

void I2C1_Init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    // PB8 -> SCL, PB9 -> SDA (AF4)
    GPIOB->MODER &= ~((3 << (8*2)) | (3 << (9*2)));
    GPIOB->MODER |=  (2 << (8*2)) | (2 << (9*2));  // AF
    GPIOB->AFR[1] |= (4 << ((8-8)*4)) | (4 << ((9-8)*4)); // AF4
    GPIOB->OTYPER |= (1 << 8) | (1 << 9); // Open-drain
    GPIOB->OSPEEDR |= (3 << (8*2)) | (3 << (9*2)); // High speed
    GPIOB->PUPDR |= (1 << (8*2)) | (1 << (9*2)); // Pull-up

    I2C1->CR1 &= ~I2C_CR1_PE;
    I2C1->CR2 = 16;
    I2C1->CCR = 80;
    I2C1->TRISE = 17;
    I2C1->CR1 |= I2C_CR1_PE;
}
// --- I2C send byte ---
void I2C1_Write(uint8_t addr, uint8_t *data, uint8_t len) {
    while(I2C1->SR2 & I2C_SR2_BUSY);
    I2C1->CR1 |= I2C_CR1_START;
    while(!(I2C1->SR1 & I2C_SR1_SB));
    (void)I2C1->SR1;
    I2C1->DR = addr << 1;
    while(!(I2C1->SR1 & I2C_SR1_ADDR));
    (void)I2C1->SR2;
    for(uint8_t i = 0; i < len; i++) {
        while(!(I2C1->SR1 & I2C_SR1_TXE));
        I2C1->DR = data[i];
    }
    while(!(I2C1->SR1 & I2C_SR1_BTF));
    I2C1->CR1 |= I2C_CR1_STOP;
}

// --- LCD lower-level functions ---
void LCD_SendNibble(uint8_t nibble, uint8_t control) {
    uint8_t data[2];
    data[0] = nibble | LCD_BACKLIGHT | control | ENABLE;
    data[1] = nibble | LCD_BACKLIGHT | control;
    I2C1_Write(LCD_ADDR, data, 2);
}

void LCD_SendByte(uint8_t byte, uint8_t mode) {
    LCD_SendNibble(byte & 0xF0, mode);
    LCD_SendNibble((byte << 4) & 0xF0, mode);
}

void LCD_Command(uint8_t cmd) {
    LCD_SendByte(cmd, 0x00);
    delay_ms(2);
}

void LCD_Data(uint8_t data) {
    LCD_SendByte(data, RS);
}

void LCD_Init(void) {
    delay_ms(50);
    LCD_SendNibble(0x30, 0);
    delay_ms(5);
    LCD_SendNibble(0x30, 0);
    delay_ms(1);
    LCD_SendNibble(0x30, 0);
    LCD_SendNibble(0x20, 0); // 4-bit mode

    LCD_Command(0x28); // 4-bit, 2 line
    LCD_Command(0x0C); // Display ON
    LCD_Command(0x06); // Entry mode
    LCD_Command(0x01); // Clear
}

void LCD_SetCursor(uint8_t row, uint8_t col) {
    uint8_t addr = (row == 0) ? 0x00 : 0x40;
    LCD_Command(0x80 | (addr + col));
}

void LCD_Print(char *str) {
    while(*str) LCD_Data(*str++);
}

/**
 * @file lcd_i2c.h
 * @brief Declarations for LCD with PCF8574T device driver.
 *
 * This file contains function prototypes, constants, and data structures
 * for interfacing with the LCD with I2C to 8 bit converter via I2C.
 * The LCD is set to 4bit mode. This is because PCF8574T has 8 GPIO pins and
 * would need 10 to 12 to transmit 8 data bits at once. That is why, each 8 byte is
 * transmitted in 2 nibbles (high first, low then). Each byte is composed by
 * Bit 0: register select (0 for instruction, 1 for data)
 * Bit 1: read/write mode (0 for write, 1 for read)
 * Bit 2: enable (0 for not enabled, 1 for enabled)
 * Bit 3: backlight (0 for backlight off, 1 for backlight on)
 * Bit 4-7: nibble sent
 * It relies on the hardware abstraction layer provided by port.h.
 */

#ifndef LCDI2C_H
#define LCDI2C_H

/**
 * @brief Includes HAL wrappers definitions.
 */
#include <portI2C.h>

/**
 * @brief Mask for turning the backlight on
 */
#define BACKLIGHT  (1<<3)

/**
 * @brief Amount of bytes sent to the PCF8574T per byte sent to the LCD
 */
#define BYTES_PER_BYTE 4

/**
 * @brief Mask for LCD enable bit
 */
#define ENABLE  (1<<2)

/**
 * @brief Mask for keeping the high nibble
 */
#define HIGH_NIBBLE_MASK 0xF0

/**
 * @brief I2C address of the PCF8574T device, right-shifted for HAL compatibility.
 */
#define LCD_ADDR (0x27 << 1)


/**
 * @brief Mask for LCD data register
 */
#define REGISTER_SELECT  (1<<0)

/**
 * @brief Mask for LCD write operation
 */
#define READWRITE  (0<<1)

/**
 * @brief Mask for setting the cursor in the second column
 */
#define SECOND_ROW 0x40

/**
 * @brief Command for setting the DDRAM
 */
#define SET_DDRAM 0x80

/**
 * @function LCD_I2C_Clear
 * @brief Function to clear the display.
 * @param None
 * @retval None
 */
void LCD_I2C_Clear();

/**
 * @function LCD_I2C_ClearWrite
 * @brief Function to clear a row and print a new string in it
 * @param str: pointer to the string to show
 * @param row: number of row to clear and write
 * @param col: col to start the printing of the string
 * @retval None
 */
void LCD_I2C_ClearWrite(char *str,uint8_t row, uint8_t col);

/**
 * @function LCD_I2C_Init
 * @brief Function to initialize the display in 4bit mode.
 * @param None
 * @retval None
 */
void LCD_I2C_Init();

/**
 * @function LCD_I2C_Send
 * @brief Function to send data to the LCD.
 * @param data: Byte of data to send to the LCD
 * @param rs: Register select (instruction or data)
 * @retval None
 */
void LCD_I2C_Send(uint8_t data, uint8_t rs);

/**
 * @function LCD_I2C_SendControlByte
 * @brief Function to send one control/instruction byte to the LCD.
 * @param data: control byte to send
 * @retval None
 */
void LCD_I2C_SendControlByte(uint8_t data);

/**
 * @function LCD_I2C_SetCursor
 * @brief Function to set the cursor on the display.
 * @param row: row position for the cursor
 * @param col: column position for the cursor
 * @retval None
 */
void LCD_I2C_SetCursor(uint8_t row, uint8_t col);

/**
 * @function LCD_I2C_WriteString
 * @brief Function to show a string on the LCD
 * @param str: pointer to the string to show
 * @retval None
 */
void LCD_I2C_WriteString(char *str);

#endif

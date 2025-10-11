/**
 * @file lcd_i2c.c
 * @brief Implementation of the LCD with PCF8574T device driver functions.
 *
 * Contains the function definitions declared in lcd_i2c.h.
 * Implements I2C read/write operations specific to LCD with PCF8574T.
 * Uses port.h functions for hardware access.
 */

/**
 * @brief Includes the header file of this library.
 */
/**
 * @file lcd_i2c.c
 * @brief Implementation of the LCD with PCF8574T device driver functions.
 *
 * Contains the function definitions declared in lcd_i2c.h.
 * Implements I2C read/write operations specific to LCD with PCF8574T.
 * Uses port.h functions for hardware access.
 */

/**
 * @brief Includes the header file of this library.
 */
#include "lcd_i2c.h"

/*Send one 8-bit byte to the display. Declared in header file*/
void LCD_I2C_Send(uint8_t data, uint8_t rs) {
    uint8_t data_u, data_l;
    uint8_t data_t[BYTES_PER_BYTE];
    data_u = (data&HIGH_NIBBLE_MASK)|rs|BL; /* masking the upper nibble and adding the control command */
    data_l = ((data<<4)&HIGH_NIBBLE_MASK)|rs|BL;  /* masking the lower nibble and adding the control command */
    data_t[0] = data_u|EN;  /* include the Enable pulse */
    data_t[1] = data_u & ~EN;  /* Turn off the Enable pulse */
    data_t[2] = data_l|EN;  /* include the Enable pulse */
    data_t[3] = data_l & ~EN;  /* Turn off the Enable pulse */
    I2CMasterTransmit(LCD_ADDR, data_t, BYTES_PER_BYTE);
}

/*Send one 8-bit instruction byte to the display. Declared in header file*/
void LCD_I2C_SendControlByte(uint8_t data) {
    LCD_I2C_Send(data,0);
}

/*Initializes the LCD. Declared in header file*/
void LCD_I2C_Init() {
    I2CDelay(50);  // Wait for power-up (adjust if needed)

    // Initialization sequence (adapted for 4-bit mode)
    LCD_I2C_SendControlByte(0x30);  // Function set: 8-bit mode (initialization)
    I2CDelay(5);
    LCD_I2C_SendControlByte(0x30);  // Function set: 8-bit mode (initialization)
    I2CDelay(1);
    LCD_I2C_SendControlByte(0x30);  // Function set: 8-bit mode (initialization)
    I2CDelay(1);
    LCD_I2C_SendControlByte(0x20);  // Function set: 4-bit mode
    I2CDelay(1);

    LCD_I2C_SendControlByte(0x20);  // Function set: 4-bit, 2 lines, 5x8 font
    LCD_I2C_SendControlByte(0x80);
    I2CDelay(1);

    LCD_I2C_SendControlByte(0x00);  // Display control: Display off, Cursor off, Blink off
    LCD_I2C_SendControlByte(0xF0);
    I2CDelay(1);

    LCD_I2C_SendControlByte(0x00);  // Entry mode set: Increment, no shift
    LCD_I2C_SendControlByte(0x60);
    I2CDelay(1);

    LCD_I2C_Clear(); // Clear the display
}

/*Clears the display. Declared in header file*/
void LCD_I2C_Clear() {
    LCD_I2C_SendControlByte(0x00); // Clear display command
    LCD_I2C_SendControlByte(0x10);
    I2CDelay(20);
}

/*Writes a string in the display. Declared in header file*/
void LCD_I2C_WriteString(char *str) {
    while (*str) {
        LCD_I2C_Send(*str, RS);  // Send character with RS=1 (data)
        str++;
    }
}

void LCD_Clear_Write(char *str,uint8_t row, uint8_t col){
	LCD_I2C_SetCursor(row, 0);
	I2CDelay(5);
	LCD_I2C_WriteString("                ");
	I2CDelay(5);
	LCD_I2C_SetCursor(row,col);
	I2CDelay(5);
	LCD_I2C_WriteString(str);
	I2CDelay(5);
}

/*Sets the cursor. Declared in header file*/
void LCD_I2C_SetCursor(uint8_t row, uint8_t col) {
    uint8_t address;
    if (row == 0) {
        address = 0x00 + col;
    } else {
        address = 0x40 + col;
    }
    LCD_I2C_SendControlByte(0x80|(address)); // Set DDRAM address command
}


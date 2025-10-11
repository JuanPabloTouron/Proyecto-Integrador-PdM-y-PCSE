/**
 * @file app.h
 * @brief Declarations for MEF functions.
 *
 * This file contains function prototypes, constants, and data structures
 * for running the main application. According to the  LCD with RTC.
 * It relies on the ds3231.h and lcd_i2c.h.
 */
#ifndef APP_H
#define APP_H

#include "ds3231.h"
#include "lcd_i2c.h"
#include "stdio.h"
#include "portButtons.h"

#define rightButton GPIO_PIN_6 //D3, PB3, GPIO3
#define menuButton GPIO_PIN_7 //D4, PB5, GPIO4
#define leftButton GPIO_PIN_8 //D5, PB4, GPIO5
#define enterButton GPIO_PIN_9 //D6, PB10, GPIO10

void appInit();
void appUpdate();
void buttonPressed(uint16_t GPIO_PIN);

#endif

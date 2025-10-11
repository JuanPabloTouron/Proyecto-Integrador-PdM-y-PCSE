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

#define MAX_BUFFER 100
#define rightButton GPIO_PIN_6 //PA6, D12
#define menuButton GPIO_PIN_7 //PA7, D11
#define leftButton GPIO_PIN_8 //PA8, D7
#define enterButton GPIO_PIN_9 //PA9, D8

void appInit();
void appUpdate();
void buttonPressed(uint16_t GPIO_PIN);

#endif

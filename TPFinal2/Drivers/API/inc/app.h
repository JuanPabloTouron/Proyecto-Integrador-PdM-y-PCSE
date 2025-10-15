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

/**
 * @brief Includes functions for interfacing with DS3231.
 */
#include "ds3231.h"

/**
 * @brief Includes functions for interfacing with LCD display.
 */
#include "lcd_i2c.h"

/**
 * @brief Includes functions for defining GPIO as buttons.
 */
#include "portButtons.h"


/*BORRAR*/
/**
 * @brief Includes functions for non-blocking delaysd.
 */
#include "stdio.h"

/**
 * @brief Includes functions for non-blocking delaysd.
 */
#include "string.h"

extern UART_HandleTypeDef huart2;
/*FIN BORRAR*/


/**
 * @brief Definition of the Enter button.
 */
#define ENTER_BUTTON ENTER_PIN

/**
 * @brief Definition of the Left button.
 */
#define LEFT_BUTTON LEFT_PIN

/**
 * @brief Maximum amount of buttons to store.
 */
#define MAX_BUFFER 100

/**
 * @brief Maximum amount of buttons to store.
 */
#define MAX_CHARS 25

/**
 * @brief Definition of the Menu button.
 */
#define MENU_BUTTON MENU_PIN

/**
 * @brief Definition of the Right button.
 */
#define RIGHT_BUTTON RIGHT_PIN

/**
 * @function AppInit
 * @brief Initializes the main app FSM.
 * @param none
 * @retval none
 */
void AppInit();

/**
 * @function AppUpdate
 * @brief Updates the main app FSM
 * @param none
 * @retval none
 */
void AppUpdate();

/**
 * @function ButtonPressed
 * @brief This function is called when a button is pressed (after considering debounce)
 * @param GPIO_Pin: number of the Pin that was pressed
 * @retval none
 */
void ButtonPressed(uint16_t GPIO_PIN);

#endif

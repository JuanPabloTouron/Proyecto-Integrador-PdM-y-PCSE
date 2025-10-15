/**
 * @file portButtons.h
 * @brief Declarations for the wrapper GPIO HAL functions.
 *
 * This file contains function prototypes and constants for defining GPIO
 * for buttons with external interrupts. It relies on the HAL functions
 * provided by stm32f4xx_hal.h.
 */
#ifndef PORTBUTTONS_H
#define PORTBUTTONS_H

/**
 * @brief Includes functions for non-blocking delaysd.
 */
#include "API_delay.h"

/**
 * @brief Includes STM32 HAL functions.
 */
#include "stm32f4xx_hal.h"


/**
 * @brief Time for debounce delay.
 */
#define DELAY 40

/**
 * @brief Definition of the external interruption line for Enter button.
 */
#define ENTER_EXTI_IRQN EXTI9_5_IRQn

/**
 * @brief Definition of the port of the GPIO Pin for Enter button.
 */
#define ENTER_GPIO_PORT GPIOA

/**
 * @brief Definition of the GPIO Pin for Enter button.
 */
#define ENTER_PIN GPIO_PIN_9

/**
 * @brief Definition of the external interruption line for Left button.
 */
#define LEFT_EXTI_IRQN EXTI9_5_IRQn

/**
 * @brief Definition of the port of the GPIO Pin for Left button.
 */
#define LEFT_GPIO_PORT GPIOA

/**
 * @brief Definition of the GPIO Pin for Left button.
 */
#define LEFT_PIN GPIO_PIN_8

/**
 * @brief Definition of the external interruption line for Menu button.
 */
#define MENU_EXTI_IRQN EXTI9_5_IRQn

/**
 * @brief Definition of the port of the GPIO Pin for Menu button.
 */
#define MENU_GPIO_PORT GPIOA

/**
 * @brief Definition of the GPIO Pin for Menu button.
 */
#define MENU_PIN GPIO_PIN_7

/**
 * @brief Number of buttons defined.
 */
#define NUMBER_OF_BUTTONS 4

/**
 * @brief Definition of the external interruption line for Right button.
 */
#define RIGHT_EXTI_IRQN EXTI9_5_IRQn

/**
 * @brief Definition of the port of the GPIO Pin for Right button.
 */
#define RIGHT_GPIO_PORT GPIOA

/**
 * @brief Definition of the GPIO Pin for Right button.
 */
#define RIGHT_PIN GPIO_PIN_6


/**
 * @function ButtonPressed
 * @brief External function that is called when a button is pressed (after considering debounce)
 * @param GPIO_Pin: number of the Pin that was pressed
 * @retval none
 */
extern void ButtonPressed(uint16_t GPIO_Pin);

/**
 * @function ButtonsInit
 * @brief Function that initializes GPIO as buttons
 * @param none
 * @retval none
 */
void ButtonsInit(void);

/**
 * @function HAL_GPIO_EXTI_Callback
 * @brief Function that activate when a button is pressed
 * @param GPIO_Pin: number of the Pin that triggered the interruption function
 * @retval none
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

#endif // PORTBUTTONS_H

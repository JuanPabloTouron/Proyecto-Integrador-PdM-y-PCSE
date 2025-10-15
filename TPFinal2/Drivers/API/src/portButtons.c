/**
 * @file portButtons.c
 * @brief Implementations of the wrapper GPIO HAL functions.
 *
 * This file contains function implementations for defining GPIO
 * for buttons with external interrupts. It relies on the HAL functions
 * provided by stm32f4xx_hal.h.
 */
#include "portButtons.h"

/**
 * @brief Type defined for button debounce.
 *
 * Pressed turns true when it is pressed for the first time; delay contains a delay struct to check the delay to the next pressing.
 */
typedef struct{
	bool_t pressed;
	delay_t delay;
} buttonDebounce;

/**
 * @brief Array of NUMBER_OF_BUTTONS (4 in this case) buttonDebounce instances.
 */
static buttonDebounce buttons[NUMBER_OF_BUTTONS];

/**
 * @brief Position of the button in the buttons array.
 */
static tick_t pos = 0;

/**
 * @brief Declaration of the function to reset button state.
 * @param buttonNumber: number of the button in the buttons array
 * @retval none
 */
static void buttonReset(tick_t buttonNumber);

/**
 * @brief Declaration of the function to reset all the buttons state.
 * @param none
 * @retval none
 */
static void buttonReset(tick_t buttonNumber);

/**
 * @brief Definition of the function to reset button state. It changes "pressed" to false and initializes the delay
 */
static void buttonReset(tick_t buttonNumber){
	buttons[buttonNumber].pressed = false;
	delayInit(&(buttons[buttonNumber].delay),DELAY);
}

/**
 * @brief Loops over buttons and resets them
 */
static void buttonsReset(void){
	for (tick_t i = 0;i<NUMBER_OF_BUTTONS;i++){
		buttonReset(i);
	}
}

/*Initializes the buttons. Declared in header file*/
void ButtonsInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  GPIO_InitStruct.Pin = RIGHT_PIN|MENU_PIN|LEFT_PIN|ENTER_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(ENTER_GPIO_PORT, &GPIO_InitStruct); /**< In this case I use GPIO for Enter button but it's the same for any of them */

  HAL_NVIC_SetPriority(ENTER_EXTI_IRQN, 0, 0); /**< In this case I use the EXTI line for Enter button but it's the same for any of them */
  HAL_NVIC_EnableIRQ(ENTER_EXTI_IRQN); /**< If there was more than one EXTI line used, all of them should be initialized */

  buttonsReset();
}

/*Checks if button is pressed (taking debounce into account) and invokes callback function. Declared in header file*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == RIGHT_PIN) pos = 0;
    else if (GPIO_Pin == MENU_PIN) pos = 1;
    else if (GPIO_Pin == LEFT_PIN) pos = 2;
    else if (GPIO_Pin == ENTER_PIN) pos = 3;
    else return;

    if (!buttons[pos].pressed) {
        buttons[pos].pressed = true;
        delayRead(&(buttons[pos].delay));
        return;
    }

    if (delayRead(&(buttons[pos].delay))) {
        if (HAL_GPIO_ReadPin(ENTER_GPIO_PORT, GPIO_Pin) == GPIO_PIN_RESET) {
            ButtonPressed(GPIO_Pin);
        }

        buttonReset(pos);
    }
}

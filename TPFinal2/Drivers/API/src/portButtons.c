#include "portButtons.h"

typedef struct{
	bool_t pressed;
	delay_t delay;
} buttonDebounce;

static buttonDebounce buttons[NUMBER_OF_BUTTONS];
static tick_t pos = 0;

static void buttonReset(tick_t buttonNumber);

static void buttonReset(tick_t buttonNumber){
	buttons[buttonNumber].pressed = false;
	delayInit(&(buttons[buttonNumber].delay),DELAY);
}

static void buttonsReset(void){
	for (tick_t i = 0;i<NUMBER_OF_BUTTONS;i++){
		buttonReset(i);
	}
}



void buttonsInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pins : PA6 PA7 PA8 PA9 */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  buttonsReset();
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

    // Find the button index
	if (GPIO_Pin == GPIO_PIN_6){
		pos = 0;
	}
	if (GPIO_Pin == GPIO_PIN_7){
		pos = 1;
	}
	if (GPIO_Pin == GPIO_PIN_8){
		pos = 2;
	}
	if (GPIO_Pin == GPIO_PIN_9){
		pos = 3;
	}

    // Only start debounce if not already pressed and debounce delay not active
    if (!buttons[pos].pressed) {
        buttons[pos].pressed = true;
        delayRead(&(buttons[pos].delay)); // Start debounce timer
        return; // Exit, wait for debounce delay to expire
    }

    // Check if the debounce delay expired before confirming button
    if (delayRead(&(buttons[pos].delay))) {
        // Delay expired, verify the button is still pressed
        // For safety, you can directly read pin state
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_Pin) == GPIO_PIN_RESET) { // adjust GPIO and pin accordingly
            buttonPressed(GPIO_Pin);
        }
        // Reset button state for next press
        buttonReset(pos);
    }
}



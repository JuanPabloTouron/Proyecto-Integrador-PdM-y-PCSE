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
    // Map GPIO_Pin to index
    if (GPIO_Pin == GPIO_PIN_6) pos = 0;
    else if (GPIO_Pin == GPIO_PIN_7) pos = 1;
    else if (GPIO_Pin == GPIO_PIN_8) pos = 2;
    else if (GPIO_Pin == GPIO_PIN_9) pos = 3;
    else return; // Not one of our buttons

    // If button not already marked as pressed, start debounce timer
    if (!buttons[pos].pressed) {
        buttons[pos].pressed = true; // Prevent re-triggering
        delayRead(&(buttons[pos].delay)); // Start 20-50ms debounce timer
        return; // Wait for debounce to expire
    }

    // When the callback fires again, check if debounce time elapsed
    if (delayRead(&(buttons[pos].delay))) {
        // Example: assuming all on GPIOA
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_Pin) == GPIO_PIN_RESET) {
            buttonPressed(GPIO_Pin); // Confirmed button press
        }

        // Reset for next event
        buttonReset(pos);
    }
}


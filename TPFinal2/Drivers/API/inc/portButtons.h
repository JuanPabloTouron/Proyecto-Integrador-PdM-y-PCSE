#ifndef PORTBUTTONS_H
#define PORTBUTTONS_H

#include "stm32f4xx_hal.h"
#include "API_delay.h"

#define NUMBER_OF_BUTTONS 4

#define MIN_DEPHASE 6

#define DELAY 60

#define Right_Pin GPIO_PIN_6
#define Right_GPIO_Port GPIOA
#define Right_EXTI_IRQn EXTI9_5_IRQn
#define Menu_Pin GPIO_PIN_7
#define Menu_GPIO_Port GPIOA
#define Menu_EXTI_IRQn EXTI9_5_IRQn
#define Left_Pin GPIO_PIN_8
#define Left_GPIO_Port GPIOA
#define Left_EXTI_IRQn EXTI9_5_IRQn
#define Enter_Pin GPIO_PIN_9
#define Enter_GPIO_Port GPIOA
#define Enter_EXTI_IRQn EXTI9_5_IRQn

extern void buttonPressed(uint16_t GPIO_Pin);

void buttonsInit(void);

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

#endif // PORTBUTTONS_H

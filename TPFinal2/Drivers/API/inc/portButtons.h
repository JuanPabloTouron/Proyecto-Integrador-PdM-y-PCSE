#ifndef PORTBUTTONS_H
#define PORTBUTTONS_H

#include "stm32f4xx_hal.h"
#include "API_delay.h"

#define NUMBER_OF_BUTTONS 4

#define MIN_DEPHASE 6

#define DELAY 40

extern void buttonPressed(uint16_t GPIO_Pin);

void buttonsInit(void);

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

#endif // PORTBUTTONS_H

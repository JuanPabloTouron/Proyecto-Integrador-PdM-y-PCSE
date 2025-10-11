#ifndef PORTBUTTONS_H
#define PORTBUTTONS_H

#include "stm32f4xx_hal.h"

extern void buttonPressed(uint16_t GPIO_Pin);

void buttonsInit(void);
//void buttonCallback(uint16_t GPIO_Pin);
/*
void EXTI3_IRQHandler(void);
void EXTI4_IRQHandler(void);
void EXTI5_IRQHandler(void);
void EXTI10_IRQHandler(void);
*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

#endif // PORTBUTTONS_H

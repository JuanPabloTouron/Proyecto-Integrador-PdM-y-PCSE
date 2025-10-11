#include <stdint.h>
#include <stdbool.h>
#include "API_delay.h"

#ifndef LD2_GPIO_Port
#define LD2_GPIO_Port GPIOA
#endif

#ifndef LD2_Pin
#define LD2_Pin GPIO_PIN_5
#endif

#ifndef B1_Pin
#define B1_Pin GPIO_PIN_13
#endif

#ifndef B1_GPIO_Port
#define B1_GPIO_Port GPIOC
#endif



void debounceFSM_init();
void debounceFSM_update();
bool_t readKey();

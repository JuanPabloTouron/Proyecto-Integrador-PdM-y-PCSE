#include "stm32f4xx_hal.h"
#include "API_delay.h"

/* USER CODE BEGIN 4 */

/*
 * @brief Checks whether the delay is running or jot
 * @param delay: Pointer to the delay
 * @retval Boolean with the value of "running"
 * @author Juan Pablo Touron
 * */
static bool_t delayIsRunning(delay_t* delay){
	return delay->running;
}

/*
 * @brief Initializes the delay
 * @param delay: Pointer to the delay
 * @param duration: Duration of the delay
 * @retval None
 * @author Juan Pablo Touron
 * */
void delayInit(delay_t* delay, tick_t duration){
	delay->duration = duration;
	delay->running = false;
}

/*
 * @brief Checks whether the delay has ended or not. If not running, it starts the delay
 * @param delay: Pointer to the delay
 * @retval Boolean with the state of the delay (running -> true, not running -> false)
 * @author Juan Pablo Touron
 * */
bool_t delayRead(delay_t* delay){
	if (delayIsRunning(delay)){
		if ((HAL_GetTick() - delay->startTime) >= delay->duration){
			delay->running = false;
			return true;
		}
	}
	else{
		delay->startTime = HAL_GetTick();
		delay->running = true;
	}
	return false;
}

/*
 * @brief Modifies the duration of the delay
 * @param delay: Pointer to the delay
 * @param duration: Duration of the delay
 * @retval None
 * @author Juan Pablo Touron
 * */
void delayWrite(delay_t* delay, tick_t duration){
	if (!delayIsRunning(delay)){
		delay->duration = duration;
	}
	else{
		Error_Handler(); //Si falla, dejo el led encendido
	}
}


/* USER CODE END 4 */

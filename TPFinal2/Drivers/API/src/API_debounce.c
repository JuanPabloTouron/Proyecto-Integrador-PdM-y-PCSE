#include "API_debounce.h"
#include "stm32f4xx_hal.h"


#ifndef PRESS_TIME
#define PRESS_TIME 40
#endif

typedef enum{
	BUTTON_UP,
	BUTTON_FALLING,
	BUTTON_DOWN,
	BUTTON_RAISING,
} debounceState_t;

typedef bool button_t;

static button_t enterButton;
static button_t menuButton;
static button_t rightButton;
static button_t leftButton;


static bool_t readButton();
static void buttonPressed();
static void buttonReleased();


static debounceState_t state;
static bool_t keyState = false;

/*
 * @brief Initializes the debounces
 * @param None
 * @retval None
 * @author Juan Pablo Touron
 * */
void debounceFSM_init(){
	state = BUTTON_UP;
}

/*
 * @brief Reads the state of the button (0 if down)
 * @param None
 * @retval Boolean with the state of the button
 * @author Juan Pablo Touron
 * */
bool readButton(){
	return HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin);
}

/*
 * @brief Reads keyState and returns its value. If true, resets it
 * @param None
 * retval Boolean indicating if the button was pressed
 * @author Juan Pablo Touron
 * */
bool_t readKey(){
	bool_t auxState = keyState;
	if (keyState){
		keyState = false;
	}
	return auxState;
}

/*
 * @brief Turns the LED on (activated when button pressed)
 * @param None
 * @retval None
 * @author Juan Pablo Touron
 * */
void buttonPressed(){
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin,GPIO_PIN_SET);
}

/*
 * @brief Turns the LED off (activated when button pressed)
 * @param None
 * @retval None
 * @author Juan Pablo Touron
 * */
void buttonReleased(){
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin,GPIO_PIN_RESET);
}


/*
 * @brief Updates the state of the debounce
 * @param None
 * @retval None
 * @author Juan Pablo Touron
 * */
void debounceFSM_update(){
	bool_t buttonState = readButton();
	static delay_t delay; //Para que mantenga el valor entre llamadas a la función
	switch (state){
	case BUTTON_UP:
		if (!buttonState){
			state = BUTTON_FALLING;
			delayInit(&delay,PRESS_TIME);
			delayRead(&delay); //Tengo que llamar a delayRead para que arranque el delay
		}
		break;
	case BUTTON_FALLING:
		if (!buttonState){
			if (delayRead(&delay)){
				keyState = true;
				state = BUTTON_DOWN;
				buttonPressed();
			}
		}
		else{
			state = BUTTON_FALLING;
		}
		break;
	case BUTTON_DOWN:
		if (buttonState){
			state = BUTTON_RAISING;
			delayInit(&delay,PRESS_TIME);
			delayRead(&delay);
		}
		break;
	case BUTTON_RAISING:
		if (buttonState){
			if (delayRead(&delay)){
				state = BUTTON_UP;
				buttonReleased();
			}
		}
		else{
			state = BUTTON_DOWN;
		}
		break;
	default:
		Error_Handler();
		break;
	}
}



/**
 * @file app.c
 * @brief Implementation of the main app functions.
 *
 * Contains the function definitions declared in app.h.
 * Implements MEFs to read buttons and switch display text or
 * configure datetime or alarms. Uses lcd_i2c.h and ds3231.h functions.
 */
#include "app.h"

typedef enum{
	SHOWTIME,
	SETTIME,
	SETALARM,
	MENU
} app_t;

typedef enum{
	SHOWTIME_M,
	SETTIME_M,
	SETALARM_M
} menu_t;

static app_t app;
static menu_t menu;
char datetext[64];
char timetext[64];
char lastTime[64] = "";
char lastDate[64] = "";

uint16_t buttonBuffer[100];

static void showTimeMode();
static void setTimeMode(uint16_t currentButton);
static void setAlarmMode(uint16_t currentButton);
static void showOptions();

DS3231_DateTime time;


uint16_t buffer[MAX_BUFFER];
int head = 0, tail = 0;

//Agrego un valor al buffer
int addToQueue(uint16_t value) {
    int nextTail = (tail + 1) % MAX_BUFFER;
    if (nextTail == head) {
        // Buffer full
        return -1;
    }
    buffer[tail] = value;
    tail = nextTail;
    return 0;
}

// Saco un valor de la fila
int removeFromQueue(void) {
    if (head == tail) {
        // Buffer empty
        return -1;
    }
    uint16_t val = buffer[head];
    head = (head + 1) % MAX_BUFFER;
    return val;
}

// Obtengo un valor de la fila
int getFromQueue(uint16_t* value) {
    if (head == tail) {
        return -1; // empty
    }
    *value = buffer[head];
    return 0;
}


static void menuInit(){
	menu = SHOWTIME_M;
}

static void menuUpdate(uint16_t button){
	switch(menu){
	case SHOWTIME_M:
		showOptions();
		if (button == rightButton) menu = SETTIME_M;
		else if (button == leftButton) menu = SETALARM_M;
		else if (button == enterButton){
			app = SHOWTIME;
			lastTime[64] = " "; //
			lastDate[64] = " "; //Porque la fecha queda guardada y si no
		}
		break;
	case SETTIME_M:
		showOptions();
		if (button == rightButton) menu = SETALARM_M;
		else if (button == leftButton) menu = SHOWTIME_M;
		else if (button == enterButton)	app = SETTIME;
		break;
	case SETALARM_M:
		showOptions();
		if (button == rightButton) menu = SHOWTIME_M;
		else if (button == leftButton) menu = SETTIME_M;
		else if (button == enterButton)	app = SETALARM;
		break;
	default:
		break;
	}
}



void appInit(){

	LCD_I2C_Init();
	HAL_Delay(1000);

	LCD_Clear_Write("Bienvenido",0,3);
	HAL_Delay(2000);
	LCD_Clear_Write("",0,0);
	LCD_Clear_Write("",1,0);
	menuInit();
	app = SHOWTIME;
}

void appUpdate(){
	uint16_t currentButton;
	if (getFromQueue(&currentButton)) currentButton = 0;
	switch(app){
	case SHOWTIME:
		if (currentButton == menuButton) app = MENU;
		else showTimeMode();
		break;
	case SETTIME:
		if (currentButton == menuButton) app = MENU;
		else setTimeMode(currentButton);
		break;
	case SETALARM:
		if (currentButton == menuButton) app = MENU;
		else setAlarmMode(currentButton);
		break;
	case MENU:
		menuUpdate(currentButton);
		break;
	default:
		break;
	}
	removeFromQueue();
}

static void showTimeMode(){
	  GetTime(&time);


	  sprintf(timetext, "%02d:%02d:%02d",time.Hours, time.Minutes, time.Seconds);
	  sprintf(datetext, "%02d/%02d/%04d",time.Date, time.Month, time.Year);

	  if (strcmp(timetext, lastTime) != 0) {
		  LCD_Clear_Write(timetext, 0, 4);
	      strcpy(lastTime, timetext);
	  }

	  if (strcmp(datetext, lastDate) != 0) {
	      LCD_Clear_Write(datetext, 1, 3);
	      strcpy(lastDate, datetext);
	  }
}

static void setTimeMode(uint16_t currentButton){
	  LCD_Clear_Write("Proxima-",0,4);
	  LCD_Clear_Write("mente.",1,5);
}

static void setAlarmMode(uint16_t currentButton){
	  LCD_Clear_Write("Por el momento",0,0);
	  LCD_Clear_Write("no disponible.",1,1);
}

static void showOptions(){
	switch(menu){
	case SHOWTIME_M:
		LCD_Clear_Write("1) Ver",0,5);
		LCD_Clear_Write("fecha y hora",1,2);
		break;
	case SETTIME_M:
		LCD_Clear_Write("2) Ajsutar",0,2);
		LCD_Clear_Write("hora y fecha",1,2);
		break;
	case SETALARM_M:
		LCD_Clear_Write("3) Poner",0,4);
		LCD_Clear_Write("alarma",1,5);
		break;
	default:
		break;
	}
}

void buttonPressed(uint16_t GPIO_Pin){
	char msg[50];
	int len = snprintf(msg, sizeof(msg), "GPIO_Pin: %u, Menu: %d, App: %d\r\n", GPIO_Pin, menu, app);	HAL_UART_Transmit(&huart2, (uint8_t*)msg, len, HAL_MAX_DELAY);
	addToQueue(GPIO_Pin);
}

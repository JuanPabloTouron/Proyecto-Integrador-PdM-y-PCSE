/**
 * @file app.c
 * @brief Implementation of the main app functions.
 *
 * Contains the function definitions declared in app.h.
 * Implements MEFs to read buttons and switch display text or
 * configure datetime or alarms. Uses lcd_i2c.h and ds3231.h functions.
 */
#include "app.h"

typedef enum {
	Sun,
	Mon,
	Tue,
	Wed,
	Thu,
	Fri,
	Sat
}day_t;

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

typedef enum{
	HOUR_DT,
	MINUTE_DT,
	SECOND_DT,
	DATE_DT,
	MONTH_DT,
	YEAR_DT
}datetime_t;

static app_t app;
static menu_t menu;
static datetime_t datetimeSet;

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
DS3231_DateTime timeToSet;


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

static void timeSetInit(){
	datetimeSet = HOUR_DT;
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
		else if (button == enterButton){
			app = SETTIME;
			timeSetInit();
			GetTime(&time);
			CopyTime(&time,&timeToSet);
		}
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

bool_t checkLeapYear(uint8_t year){
	//chequear año bisiesto
	if ((year%400 ==0)|((year%4==0)&(year%100 !=0))) return true;
	else return false;
}

bool_t timeSetUpdate(DS3231_DateTime *timeSet, uint16_t button){
	uint8_t maxDay[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
	if (checkLeapYear(timeSet->Year)) maxDay[1] = 29;
	switch(datetimeSet){
	case HOUR_DT:
		if (button == enterButton) datetimeSet = MINUTE_DT;
		else if (button == rightButton){
			if (timeSet->Hours == 23){
				timeSet->Hours = 0;
			}
			else timeSet->Hours++;
		}
		else if (button == leftButton){
			if (timeSet->Hours == 0){
				timeSet->Hours = 23;
			}
			else timeSet->Hours--;
		}
		break;
	case MINUTE_DT:
		if (button == enterButton) datetimeSet = SECOND_DT;
		else if (button == rightButton){
			if (timeSet->Minutes == 59){
				timeSet->Minutes = 0;
			}
			else timeSet->Minutes++;
		}
		else if (button == leftButton){
			if (timeSet->Minutes == 0){
				timeSet->Minutes = 59;
			}
			else timeSet->Minutes--;
		}
		break;
	case SECOND_DT:
		if (button == enterButton) datetimeSet = YEAR_DT;
		else if (button == rightButton){
			if (timeSet->Seconds == 59){
				timeSet->Seconds = 0;
			}
			else timeSet->Seconds++;
		}
		else if (button == leftButton){
			if (timeSet->Seconds == 0){
				timeSet->Seconds = 59;
			}
			else timeSet->Seconds--;
		}
		break;
	case YEAR_DT:
		if (button == enterButton) datetimeSet = MONTH_DT;
		else if (button == rightButton){
			if (timeSet->Year == 99){
				timeSet->Year = 0;
			}
			else timeSet->Year++;
		}
		else if (button == leftButton){
			if (timeSet->Year == 0){
				timeSet->Year = 99;
			}
			else timeSet->Year--;
		}
		break;
	case MONTH_DT:
		if (button == enterButton) datetimeSet = DATE_DT;
		else if (button == rightButton){
			if (timeSet->Month == 12){
				timeSet->Month = 1;
			}
			else timeSet->Month++;
		}
		else if (button == leftButton){
			if (timeSet->Month == 1){
				timeSet->Month = 12;
			}
			else timeSet->Month--;
		}
		break;
	case DATE_DT:
		if (button == enterButton) return true;
		else if (button == rightButton){
			if (timeSet->Date == maxDay[(timeSet->Month)-1]){
				timeSet->Date = 1;
			}
			else timeSet->Date++;
		}
		else if (button == leftButton){
			if (timeSet->Date == 1){
				timeSet->Date = maxDay[(timeSet->Month)-1];
			}
			else timeSet->Date--;
		}
		break;
		break;
	}
	return false;
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

	sprintf(timetext, "%02d:%02d:%02d",timeToSet.Hours, timeToSet.Minutes, timeToSet.Seconds);
	sprintf(datetext, "%02d/%02d/%04d",timeToSet.Date, timeToSet.Month, timeToSet.Year);

	LCD_Clear_Write(timetext, 0, 4);
	LCD_Clear_Write(datetext, 1, 3);

	if (timeSetUpdate(&timeToSet,currentButton)){
		SetTime(&timeToSet);
		LCD_Clear_Write("Hora",0,6);
		LCD_Clear_Write("actualizada.",1,2);
		I2CDelay(1000);

		char msg[20];
		int len = snprintf(msg, sizeof(msg), "Hora actualizada\r\n");	HAL_UART_Transmit(&huart2, (uint8_t*)msg, len, HAL_MAX_DELAY);

		app = SHOWTIME;
		menu = SHOWTIME_M;
		lastTime[64] = " "; //
		lastDate[64] = " "; //Porque la fecha queda guardada y si no
	}

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
		LCD_Clear_Write("2) Ajustar",0,2);
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

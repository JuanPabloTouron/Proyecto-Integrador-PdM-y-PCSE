/**
 * @file app.c
 * @brief Implementation of the main app functions.
 *
 * Contains the function definitions declared in app.h.
 * Implements MEFs to read buttons and switch display text or
 * configure datetime or alarms. Uses lcd_i2c.h and ds3231.h functions.
 */

/**
 * @brief Includes definitions and declarations for this module.
 */
#include "app.h"

/**
 * @brief States of the main FSM
 *
 * The main app has four possible states (corresponding to the screens to display): ShowTime, SetTime, SetAlarm and Menu.
 */
typedef enum{
	SHOWTIME,
	SETTIME,
	SETALARM,
	MENU
} app_t;

/**
 * @brief States of the menu FSM
 *
 * The menu has three possible states (corresponding to the modes of functioning): ShowTimeMode, SetTimeMode and SetAlarmMode.
 */
typedef enum{
	SHOWTIME_M,
	SETTIME_M,
	SETALARM_M
} menu_t;

/**
 * @brief States of the datetime (for time or alarm) setting FSM
 *
 * The datetime has seven possible states (corresponding to a value to set): Hours, Minutes, Seconds, Day, Date, Month and Year.
 * Alarm setting FSM only uses Hours, Minutes and Day.
 */
typedef enum{
	HOUR_DT,
	MINUTE_DT,
	SECOND_DT,
	DAY_DT,
	DATE_DT,
	MONTH_DT,
	YEAR_DT
}datetime_t;

/**
 * @brief Instance of app_t for the main FSM.
 */
static app_t app;

/**
 * @brief Instance of menu_t for the menu FSM.
 */
static menu_t menu;

/**
 * @brief Instance of datetime_t for datetime setting in SetTimeMode.
 */
static datetime_t datetimeSet;

/**
 * @brief Instance of datetime_t for alarm setting in SetAlarmMode.
 */
static datetime_t alarmSet;

/**
 * @brief Flag to check whether an alarm is set
 */
static bool_t alarmIsSet;

/**
 * @brief Variable to store the date before displaying it in the LCD.
 */
static char datetext[MAX_CHARS];

/**
 * @brief Variable to store the time before displaying it in the LCD.
 */
static char timetext[MAX_CHARS];

/**
 * @brief Variable to store the last time. In this way, I can avoid printing again the same string again.
 */
static char lastTime[MAX_CHARS] = "";

/**
 * @brief Variable to store the last date. In this way, I can avoid printing again the same string again.
 */
static char lastDate[MAX_CHARS] = "";

/**
 * @brief Array of strings (array of chars) to display in the LCD.
 */
static char *dayOfWeek[LAST_DAY] = {"Dom","Lun","Mar","Mie","Jue","Vie","Sab"};

/**
 * @brief Variable to store the buttons pressed in order.
 */
static uint16_t buttonBuffer[MAX_BUFFER];

/**
 * @brief DS3231 datetime object to store current time.
 */
DS3231_DateTime time;

/**
 * @brief DS3231 datetime object to store current alarm.
 */
DS3231_DateTime alarm;

/**
 * @brief DS3231 datetime object to store time to set.
 */
DS3231_DateTime timeToSet;

/**
 * @brief DS3231 datetime object to store alarm to set.
 */
DS3231_DateTime alarmToSet;

/**
 * @brief DS3231 datetime object to store alarm to set.
 */
int head = 0, tail = 0;

/**
 * @function ShowTimeMode.
 * @brief Displays time in the first row in "hh:mm:ss" format and date in the second row in "day dd/mm/aaaa" format.
 * @param none
 * @retval none
 */
static void ShowTimeMode();

/**
 * @function SetTimeMode.
 * @brief Allow to set date and time. The cursor blinks to the left of the unit to set.
 * @param currentButton: button pressed
 * @retval none
 */
static void SetTimeMode(uint16_t currentButton);

/**
 * @function SetAlarmMode.
 * @brief Allow to set hour, minutes and day for alarm. The cursor blinks to the left of the unit to set.
 * @param currentButton: button pressed
 * @retval none
 */
static void SetAlarmMode(uint16_t currentButton);

/**
 * @function ShowOptions.
 * @brief Shows menu options according to menu current state.
 * @param none
 * @retval none
 */
static void ShowOptions();

/**
 * @function AddToQueue
 * @brief Adds button pressed to button buffer.
 * @param value: button pressed
 * @retval none
 */
static void AddToQueue(uint16_t value) {
    int nextTail = (tail + 1) % MAX_BUFFER;
    if (nextTail == head) return; /*If buffer is full*/
    buttonBuffer[tail] = value;
    tail = nextTail;
    return;
}

/**
 * @function GetFromQueue
 * @brief Gets the next value from a queue and removes it.
 * @param none
 * @retval val: the next value of the queue
 */
static uint16_t GetFromQueue(void) {
    if (head == tail) return -1; /*If buffer is empty, return -1*/
    uint16_t val = buttonBuffer[head];
    head = (head + 1) % MAX_BUFFER;
    return val;
}

/**
 * @function MenuInit
 * @brief Initializes the menu to show time state.
 * @param none
 * @retval none
 */
static void MenuInit(){
	menu = SHOWTIME_M;
}

/**
 * @function TimeSetInit
 * @brief Initializes the time set object to hour setting.
 * @param none
 * @retval none
 */
static void TimeSetInit(datetime_t dt){
	dt = HOUR_DT;
}

/**
 * @function MenuUpdate
 * @brief Updates the menu state according to the button pressed.
 * @param button: button pressed
 * @retval none
 */
static void MenuUpdate(uint16_t button){
	switch(menu){
	case SHOWTIME_M:
		ShowOptions();
		if (button == RIGHT_BUTTON) menu = SETTIME_M;
		else if (button == LEFT_BUTTON) menu = SETALARM_M;
		else if (button == ENTER_BUTTON){
			app = SHOWTIME;
			lastTime[MAX_CHARS] = "";
			lastDate[MAX_CHARS] = ""; /*Restart these two variables so that they are printed to the LCD*/
		}
		break;
	case SETTIME_M:
		ShowOptions();
		if (button == RIGHT_BUTTON) menu = SETALARM_M;
		else if (button == LEFT_BUTTON) menu = SHOWTIME_M;
		else if (button == ENTER_BUTTON){
			app = SETTIME;
			TimeSetInit(datetimeSet);
			GetTime(&time);
			CopyTime(&time,&timeToSet);
		}
		break;
	case SETALARM_M:
		ShowOptions();
		if (button == RIGHT_BUTTON) menu = SHOWTIME_M;
		else if (button == LEFT_BUTTON) menu = SETTIME_M;
		else if (button == ENTER_BUTTON){
			app = SETALARM;
			TimeSetInit(alarmSet);
			InitTime(&alarmToSet);
		}
		break;
	default:
		break;
	}
}

bool_t CheckLeapYear(uint8_t year){
	if ((year%400 ==0)|((year%4==0)&(year%100 !=0))) return true;
	else return false;
}

void SwitchCursor(datetime_t dt){
	switch(dt){
	case HOUR_DT:
		if (app == SETTIME) LCD_I2C_SetCursor(0,3);
		else LCD_I2C_SetCursor(0,4);
		break;
	case MINUTE_DT:
		if (app == SETTIME) LCD_I2C_SetCursor(0,6);
		else LCD_I2C_SetCursor(0,7);
		break;
	case SECOND_DT: LCD_I2C_SetCursor(0,9); break;
	case DAY_DT:
		if (app == SETTIME) LCD_I2C_SetCursor(1,0);
		else LCD_I2C_SetCursor(1,5);
		break;
	case DATE_DT: LCD_I2C_SetCursor(1,4); break;
	case MONTH_DT: LCD_I2C_SetCursor(1,7); break;
	case YEAR_DT: LCD_I2C_SetCursor(1,10); break;
	}
}

bool_t TimeSetUpdate(DS3231_DateTime *timeSet, uint16_t button){
	uint8_t maxDay[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
	if (CheckLeapYear(timeSet->Year)) maxDay[1] = 29;
	SwitchCursor(datetimeSet);
	switch(datetimeSet){
	case HOUR_DT:
		if (button == ENTER_BUTTON) datetimeSet = MINUTE_DT;
		else if (button == RIGHT_BUTTON){
			if (timeSet->Hours == 23){
				timeSet->Hours = 0;
			}
			else timeSet->Hours++;
		}
		else if (button == LEFT_BUTTON){
			if (timeSet->Hours == 0){
				timeSet->Hours = 23;
			}
			else timeSet->Hours--;
		}
		break;
	case MINUTE_DT:
		if (button == ENTER_BUTTON){
			if (app == SETTIME) datetimeSet = SECOND_DT;
			if (app == SETALARM) datetimeSet = DAY_DT;
		}
		else if (button == RIGHT_BUTTON){
			if (timeSet->Minutes == 59){
				timeSet->Minutes = 0;
			}
			else timeSet->Minutes++;
		}
		else if (button == LEFT_BUTTON){
			if (timeSet->Minutes == 0){
				timeSet->Minutes = 59;
			}
			else timeSet->Minutes--;
		}
		break;
	case SECOND_DT:
		if (button == ENTER_BUTTON) datetimeSet = YEAR_DT;
		else if (button == RIGHT_BUTTON){
			if (timeSet->Seconds == 59){
				timeSet->Seconds = 0;
			}
			else timeSet->Seconds++;
		}
		else if (button == LEFT_BUTTON){
			if (timeSet->Seconds == 0){
				timeSet->Seconds = 59;
			}
			else timeSet->Seconds--;
		}
		break;
	case YEAR_DT:
		if (button == ENTER_BUTTON) datetimeSet = MONTH_DT;
		else if (button == RIGHT_BUTTON){
			if (timeSet->Year == 99){
				timeSet->Year = 0;
			}
			else timeSet->Year++;
		}
		else if (button == LEFT_BUTTON){
			if (timeSet->Year == 0){
				timeSet->Year = 99;
			}
			else timeSet->Year--;
		}
		break;
	case MONTH_DT:
		if (button == ENTER_BUTTON) datetimeSet = DATE_DT;
		else if (button == RIGHT_BUTTON){
			if (timeSet->Month == 12){
				timeSet->Month = 1;
			}
			else timeSet->Month++;
		}
		else if (button == LEFT_BUTTON){
			if (timeSet->Month == 1){
				timeSet->Month = 12;
			}
			else timeSet->Month--;
		}
		break;
	case DATE_DT:
		if (button == ENTER_BUTTON) datetimeSet = DAY_DT;
		else if (button == RIGHT_BUTTON){
			if (timeSet->Date == maxDay[(timeSet->Month)-1]){
				timeSet->Date = 1;
			}
			else timeSet->Date++;
		}
		else if (button == LEFT_BUTTON){
			if (timeSet->Date == 1){
				timeSet->Date = maxDay[(timeSet->Month)-1];
			}
			else timeSet->Date--;
		}
		break;
	case DAY_DT:
		if (button == ENTER_BUTTON) return true;
		else if (button == RIGHT_BUTTON){
			if (timeSet->Day == 7){
				timeSet->Day = 1;
			}
			else timeSet->Day++;
		}
		else if (button == LEFT_BUTTON){
			if (timeSet->Day == 1){
				timeSet->Day = 7;
			}
			else timeSet->Day--;
		}
		break;
	default:
		break;
	}
	return false;
}

void AppInit(){

	LCD_I2C_Init();
	I2CDelay(1000);

	LCD_I2C_ClearWrite("Bienvenido",0,3);
	I2CDelay(2000);
	LCD_I2C_ClearWrite("",0,0);
	LCD_I2C_ClearWrite("",1,0);
	MenuInit();
	GetAlarm(&alarm);
	alarmIsSet = IsAlarmEmpty(&alarm);
	app = SHOWTIME;
}

void AppUpdate(){
	uint16_t currentButton;
	currentButton = GetFromQueue();
	//if (GetFromQueue(&currentButton)) currentButton = 0;
	if (currentButton == -1) currentButton = 0;
	switch(app){
	case SHOWTIME:
		if (currentButton == MENU_BUTTON) app = MENU;
		else ShowTimeMode();
		break;
	case SETTIME:
		if (currentButton == MENU_BUTTON) app = MENU;
		else SetTimeMode(currentButton);
		break;
	case SETALARM:
		if (currentButton == MENU_BUTTON) app = MENU;
		else SetAlarmMode(currentButton);
		break;
	case MENU:
		MenuUpdate(currentButton);
		break;
	default:
		break;
	}
	//RemoveFromQueue();
}


static void ShowTimeMode(){
	  GetTime(&time);
	  uint8_t col;

	  if (alarmSet){
		  sprintf(timetext, "A   %02d:%02d:%02d",time.Hours, time.Minutes, time.Seconds);
		  col = 0;
	  }
	  else{
		  sprintf(timetext, "%02d:%02d:%02d",time.Hours, time.Minutes, time.Seconds);
		  col=4;
	  }

	  sprintf(datetext, "%s %02d/%02d/%04d",dayOfWeek[time.Day-1],time.Date, time.Month, time.Year);

	  if (strcmp(timetext, lastTime) != 0) {
		  LCD_I2C_ClearWrite(timetext, 0, col);
	      strcpy(lastTime, timetext);
	  }

	  if (strcmp(datetext, lastDate) != 0) {
		  LCD_I2C_ClearWrite(datetext, 1, 1);
	      strcpy(lastDate, datetext);
	  }
}

static void SetTimeMode(uint16_t currentButton){

	sprintf(timetext, "%02d:%02d:%02d",timeToSet.Hours, timeToSet.Minutes, timeToSet.Seconds);
	sprintf(datetext, "%s %02d/%02d/%04d",dayOfWeek[timeToSet.Day-1],timeToSet.Date, timeToSet.Month, timeToSet.Year);

	LCD_I2C_ClearWrite(timetext, 0, 4);
	LCD_I2C_ClearWrite(datetext, 1, 1);

	if (TimeSetUpdate(&timeToSet,currentButton)){
		SetTime(&timeToSet);
		LCD_I2C_ClearWrite("Hora",0,6);
		LCD_I2C_ClearWrite("actualizada.",1,2);
		I2CDelay(1000);

		char msg[20];
		int len = snprintf(msg, sizeof(msg), "Hora actualizada\r\n");	HAL_UART_Transmit(&huart2, (uint8_t*)msg, len, HAL_MAX_DELAY);

		app = SHOWTIME;
		menu = SHOWTIME_M;
		lastTime[MAX_CHARS] = " "; //
		lastDate[MAX_CHARS] = " "; //Porque la fecha queda guardada y si no
	}

}

static void SetAlarmMode(uint16_t currentButton){
	sprintf(timetext, "%02d:%02d",alarmToSet.Hours, alarmToSet.Minutes);
	sprintf(datetext, "%s",dayOfWeek[alarmToSet.Day-1]);

	LCD_I2C_ClearWrite(timetext, 0, 5);
	LCD_I2C_ClearWrite(datetext, 1, 6);

	if (TimeSetUpdate(&alarmToSet,currentButton)){
		SetAlarm(&alarmToSet);
		LCD_I2C_ClearWrite("Alarma",0,5);
		LCD_I2C_ClearWrite("guardada.",1,4);
		I2CDelay(1000);

		char msg[MAX_CHARS];
		int len = snprintf(msg, sizeof(msg), "Alarma guardad\r\n");	HAL_UART_Transmit(&huart2, (uint8_t*)msg, len, HAL_MAX_DELAY);

		app = SHOWTIME;
		menu = SHOWTIME_M;
		lastTime[MAX_CHARS] = " "; //
		lastDate[MAX_CHARS] = " ";//Porque la fecha queda guardada y si no
		alarmSet = true;
	}

}

static void ShowOptions(){
	switch(menu){
	case SHOWTIME_M:
		LCD_I2C_ClearWrite("1) Ver",0,5);
		LCD_I2C_ClearWrite("fecha y hora",1,2);
		break;
	case SETTIME_M:
		LCD_I2C_ClearWrite("2) Ajustar",0,2);
		LCD_I2C_ClearWrite("hora y fecha",1,2);
		break;
	case SETALARM_M:
		LCD_I2C_ClearWrite("3) Poner",0,4);
		LCD_I2C_ClearWrite("alarma",1,5);
		break;
	default:
		break;
	}
}

void ButtonPressed(uint16_t GPIO_Pin){
	/*BORRAR*/
	//char msg[MAX_CHARS];
	//int len = snprintf(msg, sizeof(msg), "GPIO_Pin: %u, Menu: %d, App: %d\r\n", GPIO_Pin, menu, app);	HAL_UART_Transmit(&huart2, (uint8_t*)msg, len, HAL_MAX_DELAY);
	/*FIN BORRAR*/
	AddToQueue(GPIO_Pin);
}

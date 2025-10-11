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

uint16_t buttonBuffer[100];

static void showTimeMode();
static void setTimeMode(uint16_t currentButton);
static void setAlarmMode(uint16_t currentButton);
static void showOptions();
DS3231_DateTime time;


uint16_t buffer[MAX_BUFFER];
int head = 0;
int tail = 0;

//Agrego un valor al buffer
int enqueue(uint16_t value) {
    int next_tail = (tail + 1) % MAX_BUFFER;
    if (next_tail == head) {
        // Buffer full
        return -1;
    }
    buffer[tail] = value;
    tail = next_tail;
    return 0;
}

// Saco un valor de la fila
int pop(void) {
    if (head == tail) {
        // Buffer empty
        return -1;
    }
    uint16_t val = buffer[head];
    head = (head + 1) % MAX_BUFFER;
    return val;
}

// Obtengo un valor de la fila
int peek(uint16_t* value) {
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
	showOptions();
	switch(menu){
	case SHOWTIME_M:
		if (button == rightButton){
			menu = SETTIME_M;
		}
		else{
			if (button == leftButton){
				menu = SETALARM_M;
			}
			else{
				if (button == enterButton){
					app = SHOWTIME;
				}
			}
		}
		break;
	case SETTIME_M:
		if (button == rightButton){
			menu = SETALARM_M;
		}
		else{
			if (button == leftButton){
				menu = SHOWTIME_M;
			}
			else{
				if (button == enterButton){
					app = SETTIME;
				}
			}
		}
		break;
	case SETALARM_M:
		if (button == rightButton){
			menu = SHOWTIME_M;
		}
		else{
			if (button == leftButton){
				menu = SETTIME_M;
			}
			else{
				if (button == enterButton){
					app = SETALARM;
				}
			}
		}
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
	if (peek(&currentButton)){
		currentButton = 0;
	}
	switch(app){
	case SHOWTIME:
		if (currentButton == menuButton){
			app = MENU;
		}
		else{
			showTimeMode();
		}
		break;
	case SETTIME:
		if (currentButton == menuButton){
			app = MENU;
		}
		else{
			setTimeMode(currentButton);
		}
		break;
	case SETALARM:
		if (currentButton == menuButton){
			app = MENU;
		}
		else{
			setAlarmMode(currentButton);
		}
		break;
	case MENU:
		menuUpdate(currentButton);
		break;
	default:
		break;
	}
	pop();
}

static void showTimeMode(){
	  GetTime(&time);


	  sprintf(timetext, "%02d:%02d:%02d",time.Hours, time.Minutes, time.Seconds);
	  sprintf(datetext, "%02d/%02d/%04d",time.Date, time.Month, time.Year);

	  LCD_Clear_Write(timetext,0,4);
	  LCD_Clear_Write(datetext,1,3);
}

static void setTimeMode(uint16_t currentButton){
	  GetTime(&time);


	  sprintf(timetext, "%02d:%02d:%02d", time.Seconds, time.Hours, time.Minutes);
	  sprintf(datetext, "%02d/%02d/%04d",time.Date, time.Month, time.Year);

	  LCD_Clear_Write(timetext,0,4);
	  LCD_Clear_Write(datetext,1,3);
}

static void setAlarmMode(uint16_t currentButton){
	  GetTime(&time);


	  sprintf(timetext, "%02d:%02d:%02d", time.Minutes, time.Seconds, time.Hours);
	  sprintf(datetext, "%02d/%02d/%04d",time.Date, time.Month, time.Year);

	  LCD_Clear_Write(timetext,0,4);
	  LCD_Clear_Write(datetext,1,3);
}

static void showOptions(){
	switch(menu){
	case SHOWTIME_M:
		LCD_Clear_Write("1) Ver",0,5);
		LCD_Clear_Write("fecha y hora",1,2);
		break;
	case SETTIME_M:
		LCD_Clear_Write("2) Ajsutar",1,2);
		LCD_Clear_Write("fecha y hora",1,2);
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
	enqueue(GPIO_Pin);
}

/*
void buttonPressed2(uint16_t GPIO_Pin)
{
    switch(GPIO_Pin)
    {
        case rightButton:
        	LCD_I2C_SetCursor(0, 14);
        	LCD_I2C_WriteString("AB");
        	if (app == MENU){
            	if (menu == SHOWTIME_M){
            		menu = SETTIME_M;
            	}
            	if (menu == SETTIME_M){
            		menu = SETALARM_M;
            	}
            	if (menu == SETALARM_M){
            		menu = SHOWTIME_M;
            	}
            }
            if (app == SETTIME){
               	//Do something
            }
            if (app == SETALARM){
            	//Do something
            }
            break;
        case leftButton:
        	LCD_I2C_SetCursor(0, 14);
        	LCD_I2C_WriteString("CD");
            if (app == MENU){
            	if (menu == SHOWTIME_M){
            		menu = SETALARM_M;
            	}
            	if (menu == SETTIME_M){
            		menu = SHOWTIME_M;
            	}
            	if (menu == SETALARM_M){
            		menu = SETTIME_M;
            	}
            }
            if (app == SETTIME){
               	//Do something
            }
            if (app == SETALARM){
            	//Do something
            }
            break;
        case menuButton:
        	LCD_I2C_SetCursor(0, 14);
        	LCD_I2C_WriteString("EF");
            app = MENU;
            break;
        case enterButton:
        	LCD_I2C_SetCursor(0, 14);
        	LCD_I2C_WriteString("GH");
            if (app == MENU){
                if (menu == SHOWTIME_M){
                	app = SHOWTIME;
                }
                if (menu == SETTIME_M){
                	app = SETTIME;
                }
                if (menu == SETALARM_M){
                	app = SETALARM;
                }
            }
            if (app == SETTIME){
               	//Do something
            }
            if (app == SETALARM){
            	//Do something
            }
            break;
        default:
            break;
    }
}
*/

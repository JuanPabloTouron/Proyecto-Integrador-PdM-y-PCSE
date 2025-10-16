/**
 * @file ds3231.c
 * @brief Implementation of the DS3231 RTC device driver functions.
 *
 * Contains the function definitions declared in ds3231.h.
 * Implements I2C read/write operations specific to DS3231.
 * Uses port.h functions for hardware access.
 */

/**
 * @brief Includes the header file of this library.
 */
#include "ds3231.h"

/*Convert a BCD-encoded value to decimal. Declared in header file*/
uint8_t BcdToDec(uint8_t val) {
    return ((val >> NIBBLE_SIZE) * 10) + (val & LOW_NIBBLE_MASK); /**< Not necessary to consider special cases because of the solution addressed*/
}

/*Copies the info of a time struct into another. Declared in header file*/
void CopyTime(DS3231_DateTime *timeToCopy, DS3231_DateTime *timeToPaste){
	timeToPaste->Seconds = timeToCopy->Seconds;
	timeToPaste->Minutes = timeToCopy->Minutes;
	timeToPaste->Hours = timeToCopy->Hours;
	timeToPaste->Day = timeToCopy->Day;
	timeToPaste->Date = timeToCopy->Date;
	timeToPaste->Month = timeToCopy->Month;
	timeToPaste->Year = timeToCopy->Year;
}

/*Convert a decimal value to BCD-encoded. Declared in header file*/
uint8_t DecToBcd(uint8_t val) {
    return ((val / 10) << NIBBLE_SIZE) | (val % 10); /**< Not necessary to consider special cases because of the solution addressed*/
}

/*Gets the alarm from the DS3231 RTC. Declared in header file*/
void GetAlarm(DS3231_DateTime *time) {
    uint8_t buffer[ALARM_SIZE-1]; /**< In this case, it is not necessary to indicate start register in the data to transmit cause MemRead is used*/
    I2CReadMemory(ALARM_START_REGISTER,DS3231_ADDR,buffer,ALARM_SIZE-1);

    time->Minutes = BcdToDec(buffer[0]);
    time->Hours = BcdToDec(buffer[1] & MODE24_MASK); /**< Implemented only in 24H mode*/
    time->Day   = BcdToDec(buffer[2] & LOW_NIBBLE_MASK);  /**< Implemented only in Day of the week alarm mode*/
}

/*Gets the time from the DS3231 RTC. Declared in header file*/
void GetTime(DS3231_DateTime *time) {
    uint8_t buffer[TIME_SIZE-1]; /**< In this case, it is not necesary to indicate start register in the data to transmit cause MemRead is used*/
    I2CReadMemory(TIME_START_REGISTER,DS3231_ADDR,buffer,TIME_SIZE-1);

    time->Seconds = BcdToDec(buffer[0]);
    time->Minutes = BcdToDec(buffer[1]);
    time->Hours   = BcdToDec(buffer[2] & MODE24_MASK);  /**< Implemented only in 24H mode*/
    time->Day     = BcdToDec(buffer[3]);
    time->Date    = BcdToDec(buffer[4]);
    time->Month   = BcdToDec(buffer[5] & CENTURY_MASK); /**< Cleans the MSB that is associated to the century change*/
    time->Year    = YEAR_CORRECTION + BcdToDec(buffer[6]);
}

/*Initializes a time struct. Declared in header file*/
void InitTime(DS3231_DateTime *time){
	time->Seconds = BcdToDec(MIDNIGHT);
	time->Minutes = BcdToDec(MIDNIGHT);
	time->Hours = BcdToDec(MIDNIGHT);
	time->Day = BcdToDec(FIRST_DAY);
	time->Date = BcdToDec(FIRST);
	time->Month = BcdToDec(JANUARY);
	time->Year = YEAR_CORRECTION + BcdToDec(Y2K);
}

/*Checks whether there is an alarm set to the DS3231 RTC. Declared in header file*/
bool IsAlarmSet(DS3231_DateTime *alarm) {
    return ((alarm->Day >= FIRST_DAY)&&(alarm->Day <= LAST_DAY)); /**< If alarm is set, Day byte varies from 1 to 7. Otherwise, no alarm is set*/
}

/*Sets the alarm of the DS3231 RTC. Declared in header file*/
void SetAlarm(DS3231_DateTime *time){
	uint8_t buffer[ALARM_SIZE];

	buffer[0] = ALARM_START_REGISTER;
	buffer[1] = DecToBcd(time->Minutes);
	buffer[2] = DecToBcd(time->Hours) & MODE24_MASK; /**< Implemented only in 24H mode*/
	buffer[3] = DecToBcd(time->Day) & LOW_NIBBLE_MASK; /**< Implemented only in Day of the week alarm mode*/

	I2CMasterTransmit(DS3231_ADDR, buffer, ALARM_SIZE);
}

/*Sets the time of the DS3231 RTC. Declared in header file*/
void SetTime(DS3231_DateTime *time) {
    uint8_t buffer[TIME_SIZE];

    buffer[0] = TIME_START_REGISTER;
    buffer[1] = DecToBcd(time->Seconds) & CONTROL_REGISTER_MASK;  /**< Cleans the MSB that if set to 1 indicates to reset RTC*/
    buffer[2] = DecToBcd(time->Minutes);
    buffer[3] = DecToBcd(time->Hours) & MODE24_MASK; /**< Implemented only in 24H mode*/
    buffer[4] = DecToBcd(time->Day);
    buffer[5] = DecToBcd(time->Date);
    buffer[6] = DecToBcd(time->Month) & CENTURY_MASK;  /**< Cleans the MSB that is associated to the century change*/
    buffer[7] = DecToBcd(time->Year - YEAR_CORRECTION);

    I2CMasterTransmit(DS3231_ADDR, buffer, TIME_SIZE);
}

/**
 * @file ds3231.h
 * @brief Declarations for DS3231 RTC device driver.
 *
 * This file contains function prototypes, constants, and data structures
 * for interfacing with the DS3231 real-time clock via I2C.
 * It relies on the hardware abstraction layer provided by port.h.
 */
#ifndef DS3231_H
#define DS3231_H

/**
 * @brief Includes HAL wrappers definitions.
 */
#include <portI2C.h>

/**
 * @brief Includes boolean type definitions.
 */
#include <stdbool.h>

/**
 * @brief Includes integer type definitions.
 */
#include <stdint.h>


/**
 * @brief Size (in bytes) of the buffer to transmit/receive data for alarms
 */
#define ALARM_SIZE 4

/**
 * @brief Register to start to write to or read from to set the alarm. In the DS3231, it contains data
 * about the seconds.
 */
#define ALARM_START_REGISTER 0x08

/**
 * @brief Mask for keeping the 5 LSB. Used in month byte to avoid the century bit
 */
#define CENTURY_MASK 0x1F

/**
 * @brief Mask for keeping all the bits but the MSB. Used in seconds byte to avoid the control register
 */
#define CONTROL_REGISTER_MASK 0x7F

/**
 * @brief I2C address of the DS3231 RTC device, right-shifted for HAL compatibility.
 */
#define DS3231_ADDR (0x68 << 1)

/**
 * @brief First day of the month. Used to initialize a time struct
 */
#define FIRST 1

/**
 * @brief First month of the year. Used to initialize a time struct
 */
#define JANUARY 1

/**
 * @brief Mask for keeping the low nibble
 */
#define LOW_NIBBLE_MASK 0x0F

/**
 * @brief Midnight hour. Used to initialize a time struct
 */
#define MIDNIGHT 0

/**
 * @brief Mask for keeping the 5 LSB. Used in month byte to avoid the century bit
 */
#define MODE24_MASK 0x3F

/**
 * @brief Size (in bits) of a nibble
 */
#define NIBBLE_SIZE 4

/**
 * @brief Saturday in the time struct.
 */
#define LAST_DAY 7

/**
 * @brief Sunday in the time struct. Used to initialize a time struct
 */
#define FIRST_DAY 1

/**
 * @brief Size (in bytes) of the buffer to transmit/receive data for time
 */
#define TIME_SIZE 8

/**
 * @brief Register to start to write to or read from to set datetime. In the DS3231, it contains data
 * about the seconds.
 */
#define TIME_START_REGISTER 0x00

/**
 * @brief First year of the 2000s. Used to initialize a time struct
 */
#define Y2K 0

/**
 * @brief Register to start to write to or read from. In the DS3231, it contains data
 * about the seconds.
 */
#define YEAR_CORRECTION 2000


/**
 * @typedef DateTime
 * @brief Struct that represents a specific DS3231 date and time.
 *
 * It contains detailed information of date and time
 */
typedef struct {
    uint8_t Seconds;/**< Seconds (0-59) */
    uint8_t Minutes;/**< Minutes (0-59) */
    uint8_t Hours;	/**< Hours (0-23) or (1-12) depending on the mode */
    uint8_t Day;    /**< Day of week (1-7) starting on Sunday*/
    uint8_t Date;   /**< Day of month (1-31) depending on the month (considers leap year) */
    uint8_t Month;  /**< Month (1-12) */
    uint16_t Year;  /**< Complete year (e.g.: 2025) */
} DS3231_DateTime;


/**
 * @function bcd2dec
 * @brief Function to convert a number in BCD to decimal.
 * @param val: value to convert in BCD format
 * @retval value converted to decimal
 */
uint8_t BcdToDec(uint8_t val);

/**
 * @function CopyTime
 * @brief Function that copies the fields of one Datetime object to another.
 * @param timeToCopy: pointer to the DS3231_DateTime to be copied
 * @param timeToPaste: pointer to the DS3231_DateTime to complete
 * @retval none
 */
void CopyTime(DS3231_DateTime *timeToCopy, DS3231_DateTime *timeToPaste);

/**
 * @function BcdToDec
 * @brief Function to convert a number in decimal to BCD.
 * @param val: value to convert in decimal
 * @retval value converted to BCD format
 */
uint8_t DecToBcd(uint8_t val);

/**
 * @function SetAlarm
 * @brief Function that gets an alarm of the DS3231.
 * @param time: pointer to the DateTime struct that will store the alarm to get
 * @retval none
 */
void GetAlarm(DS3231_DateTime *time);

/**
 * @function GetTime
 * @brief Function that get the date and time from the DS3231.
 * @param time: pointer to the DateTime struct that will store the date and time
 * @retval none
 */
void GetTime(DS3231_DateTime *time);

/**
 * @function InitTime
 * @brief Function that initialize the fields of one Datetime object.
 * @param time: pointer to the DS3231_DateTime to initialize
 * @retval none
 */
void InitTime(DS3231_DateTime *time);

/**
 * @function IsAlarmSet
 * @brief Function that checks whether an alarm is set.
 * @param alarm: pointer to the DateTime with the alarm
 * @retval boolean that indicates if an alarm is set
 */
bool IsAlarmSet(DS3231_DateTime *alarm);

/**
 * @function SetAlarm
 * @brief Function that sets an alarm of the DS3231.
 * @param time: pointer to the DateTime struct that store the alarm to set
 * @retval none
 */
void SetAlarm(DS3231_DateTime *time);

/**
 * @function SetTime
 * @brief Function that sets the date and time of the DS3231.
 * @param time: pointer to the DateTime struct that store the date and time to set
 * @retval none
 */
void SetTime(DS3231_DateTime *time);

#endif

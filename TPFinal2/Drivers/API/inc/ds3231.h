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
 * @brief Includes integer type definitions.
 */
#include <portI2C.h>
#include <stdint.h>

/**
 * @brief Includes HAL wrappers definitions.
 */


/**
 * @brief I2C address of the DS3231 RTC device, right-shifted for HAL compatibility.
 */
#define DS3231_ADDR (0x68 << 1)

/**
 * @brief Size (in bytes) of the buffer to transmit/receive data
 */
#define BUFFER_SIZE 8

/**
 * @brief Size (in bits) of a nibble
 */
#define NIBBLE_SIZE 4

/**
 * @brief Mask for keeping the low nibble
 */
#define LOW_NIBBLE_MASK 0x0F

/**
 * @brief Mask for keeping the 5 LSB. Used in month byte to avoid the century bit
 */
#define CENTURY_MASK 0x1F

/**
 * @brief Mask for keeping the 5 LSB. Used in month byte to avoid the century bit
 */
#define MODE24_MASK 0x3F

/**
 * @brief Register to start to write to or read from. In the DS3231, it contains data
 * about the seconds.
 */
#define START_REGISTER 0x00

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

/* Declaration of the UART external handle. Declared in the main */
extern UART_HandleTypeDef huart2;

/**
 * @function bcd2dec
 * @brief Function to convert a number in BCD to decimal.
 * @param val: value to convert in BCD format
 * @retval value converted to decimal
 */
uint8_t BcdToDec(uint8_t val);

/**
 * @function BcdToDec
 * @brief Function to convert a number in decimal to BCD.
 * @param val: value to convert in decimal
 * @retval value converted to BCD format
 */
uint8_t DecToBcd(uint8_t val);

/**
 * @function DecToBcd
 * @brief Function that sets the date and time of the DS3231.
 * @param hi2c: pointer to the I2C handle that will communicate STM32 to DS3231
 * @param time: pointer to the DateTime struct that store the date and time to set
 * @retval none
 */
void SetTime(DS3231_DateTime *time);

/**
 * @function GetTime
 * @brief Function that get the date and time from the DS3231.
 * @param hi2c: pointer to the I2C handle that will communicate STM32 to DS3231
 * @param time: pointer to the DateTime struct that will store the date and time
 * @retval none
 */
void GetTime(DS3231_DateTime *time);

#endif

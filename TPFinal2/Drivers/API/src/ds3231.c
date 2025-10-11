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
    return ((val >> NIBBLE_SIZE) * 10) + (val & LOW_NIBBLE_MASK);
}

/*Convert a decimal value to BCD-encoded. Declared in header file*/
uint8_t DecToBcd(uint8_t val) {
    return ((val / 10) << NIBBLE_SIZE) | (val % 10);
}

/*Sets the time of the DS3231 RTC. Declared in header file*/
void SetTime(DS3231_DateTime *time) {
    uint8_t buffer[BUFFER_SIZE];

    buffer[0] = START_REGISTER;
    buffer[1] = DecToBcd(time->Seconds) & 0x7F;   // limpiar CH
    buffer[2] = DecToBcd(time->Minutes);
    buffer[3] = DecToBcd(time->Hours) & MODE24_MASK;     // 24h mode
    buffer[4] = DecToBcd(time->Day);
    buffer[5] = DecToBcd(time->Date);
    buffer[6] = DecToBcd(time->Month) & CENTURY_MASK;     // descartar century
    buffer[7] = DecToBcd(time->Year - YEAR_CORRECTION);

    I2CMasterTransmit(DS3231_ADDR, buffer, BUFFER_SIZE);
}

/*Gets the time from the DS3231 RTC. Declared in header file*/
void GetTime(DS3231_DateTime *time) {
    uint8_t buffer[BUFFER_SIZE-1];
    I2CReadMemory(START_REGISTER,DS3231_ADDR,buffer,BUFFER_SIZE-1);

    time->Seconds = BcdToDec(buffer[0]);
    time->Minutes = BcdToDec(buffer[1]);
    time->Hours   = BcdToDec(buffer[2] & MODE24_MASK);
    time->Day     = BcdToDec(buffer[3]);
    time->Date    = BcdToDec(buffer[4]);
    time->Month   = BcdToDec(buffer[5] & CENTURY_MASK);
    time->Year    = YEAR_CORRECTION + BcdToDec(buffer[6]);
}

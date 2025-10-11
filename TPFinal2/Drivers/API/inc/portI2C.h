#ifndef PORT_H
#define PORT_H
/**
 * @file port.h
 * @brief Declarations for the wrapper I2C HAL functions.
 *
 * This file contains function prototypes and constants for interfacing
 * with any integrated circuit via I2C. It relies on the HAL functions
 * provided by stm32f4xx_hal.h.
 */

/**
 * @brief Includes STM32 HAL functions.
 */
#include "stm32f4xx_hal.h"

/**
 * @brief Timeout for the HAL I2C functions.
 */
#define TIMEOUT HAL_MAX_DELAY

/**
 * @brief Size (in bits) of the memory address.
 */
#define REG_SIZE I2C_MEMADD_SIZE_8BIT

/* Declaration of the external error handler function. Declared in the main */
extern void Error_Handler();

/**
 * @function I2CInit
 * @brief Function that initializes the I2C protocol handle.
 * @param None
 * @retval None
 */
void I2CInit(void);

/**
 * @function I2CDelay
 * @brief Function that delays the main program for a specified number of ms.
 * @param delayTime: time for delay
 * @retval None
 */
void I2CDelay(uint32_t delayTime);

/**
 * @function I2CMasterTransmit
 * @brief Function to transmit data to a specified slave.
 * @param devAddr: number of the I2C device address
 * @param buffer: pointer to data buffer to be written. The first element is the memory register
 * @param size: size of the buffer
 * @retval none
 */
void I2CMasterTransmit(uint16_t devAddr, uint8_t *buffer, uint16_t size);

/**
 * @function I2CReadMemory
 * @brief Function to read specific memory registers for a given IC.
 * @param devAddr: number of the I2C device address
 * @param startReg: memory register address to start reading
 * @param buffer: pointer to buffer to store the read data
 * @param size: size of the buffer
 * @retval none
 */
void I2CReadMemory(uint16_t startReg, uint16_t devAddr, uint8_t *buffer, uint16_t size);

#endif

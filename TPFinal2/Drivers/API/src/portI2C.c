/**
 * @file port.c
 * @brief Implementation of the wrapper I2C HAL functions.
 *
 * Contains the function definitions declared in port.h.
 * Implements I2C read/write operations for any integrated circuit.
 * Wraps HAL functions for other libraries' access.
 */

/**
 * @brief Includes the header file of this library.
 */
#include <portI2C.h>

/* Declaration of the I2C external handle. Declared in the main */
I2C_HandleTypeDef hi2c1;

/*Initialize the I2C protocol handle. Declared in header file*/
void I2CInit(){

  __HAL_RCC_I2C1_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
}

/*Delays the app for delayTime miliseconds. Declared in header file*/
void I2CDelay(uint32_t delayTime){
	HAL_Delay(delayTime);
}

/*Writes the data buffer to the slave. Declared in header file*/
void I2CMasterTransmit(uint16_t devAddr, uint8_t *buffer, uint16_t size){
	HAL_I2C_Master_Transmit(&hi2c1, devAddr, buffer, size, TIMEOUT);
}

/*Reads specific memory registers from a given IC. Declared in header file*/
void I2CReadMemory(uint16_t startReg, uint16_t devAddr, uint8_t *buffer, uint16_t size){
	HAL_I2C_Mem_Read(&hi2c1, devAddr, startReg, REG_SIZE, buffer, size, TIMEOUT);
}

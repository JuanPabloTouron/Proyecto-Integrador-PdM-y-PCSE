################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/API/src/API_debounce.c \
../Drivers/API/src/API_delay.c \
../Drivers/API/src/app.c \
../Drivers/API/src/ds3231.c \
../Drivers/API/src/lcd_i2c.c \
../Drivers/API/src/portButtons.c \
../Drivers/API/src/portI2C.c 

OBJS += \
./Drivers/API/src/API_debounce.o \
./Drivers/API/src/API_delay.o \
./Drivers/API/src/app.o \
./Drivers/API/src/ds3231.o \
./Drivers/API/src/lcd_i2c.o \
./Drivers/API/src/portButtons.o \
./Drivers/API/src/portI2C.o 

C_DEPS += \
./Drivers/API/src/API_debounce.d \
./Drivers/API/src/API_delay.d \
./Drivers/API/src/app.d \
./Drivers/API/src/ds3231.d \
./Drivers/API/src/lcd_i2c.d \
./Drivers/API/src/portButtons.d \
./Drivers/API/src/portI2C.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/API/src/%.o Drivers/API/src/%.su Drivers/API/src/%.cyclo: ../Drivers/API/src/%.c Drivers/API/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Juan Pablo/Downloads/PdM_Practicas/TPFinal2/Drivers/API/inc" -I"C:/Users/Juan Pablo/Downloads/PdM_Practicas/TPFinal2/Drivers/API/src" -I"C:/Users/Juan Pablo/Downloads/PdM_Practicas/TPFinal2/Drivers/API" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-API-2f-src

clean-Drivers-2f-API-2f-src:
	-$(RM) ./Drivers/API/src/API_debounce.cyclo ./Drivers/API/src/API_debounce.d ./Drivers/API/src/API_debounce.o ./Drivers/API/src/API_debounce.su ./Drivers/API/src/API_delay.cyclo ./Drivers/API/src/API_delay.d ./Drivers/API/src/API_delay.o ./Drivers/API/src/API_delay.su ./Drivers/API/src/app.cyclo ./Drivers/API/src/app.d ./Drivers/API/src/app.o ./Drivers/API/src/app.su ./Drivers/API/src/ds3231.cyclo ./Drivers/API/src/ds3231.d ./Drivers/API/src/ds3231.o ./Drivers/API/src/ds3231.su ./Drivers/API/src/lcd_i2c.cyclo ./Drivers/API/src/lcd_i2c.d ./Drivers/API/src/lcd_i2c.o ./Drivers/API/src/lcd_i2c.su ./Drivers/API/src/portButtons.cyclo ./Drivers/API/src/portButtons.d ./Drivers/API/src/portButtons.o ./Drivers/API/src/portButtons.su ./Drivers/API/src/portI2C.cyclo ./Drivers/API/src/portI2C.d ./Drivers/API/src/portI2C.o ./Drivers/API/src/portI2C.su

.PHONY: clean-Drivers-2f-API-2f-src


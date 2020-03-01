################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/hardware-drivers/Serial.c \
../src/hardware-drivers/TouchScreen.c \
../src/hardware-drivers/wifi.c \
../src/hardware-drivers/gps.c 

OBJS += \
./src/hardware-drivers/Serial.o \
./src/hardware-drivers/TouchScreen.o \
./src/hardware-drivers/wifi.o \
./src/hardware-drivers/gps.o 

C_DEPS += \
./src/hardware-drivers/Serial.d \
./src/hardware-drivers/TouchScreen.d \
./src/hardware-drivers/wifi.d \
./src/hardware-drivers/gps.d 


# Each subdirectory must supply rules for building sources it contributes
src/hardware-drivers/%.o: ../src/hardware-drivers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler 4 [arm-linux-gnueabihf]'
	arm-linux-gnueabihf-gcc -std=c99 -Dsoc_cv_av -I"../../include/hardware-drivers/Serial.h" -I"/home/jacob/cpen391/techtrek-soc/techtrek/include/hardware-drivers" -I"/home/jacob/cpen391/techtrek-soc/techtrek/include" -I"/home/jacob/cpen391/techtrek-soc/techtrek/include/graphics" -O0 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



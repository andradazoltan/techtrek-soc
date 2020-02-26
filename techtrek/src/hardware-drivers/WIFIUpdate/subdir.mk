################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/hardware-drivers/WIFIUpdate/WiFi.c 

OBJS += \
./src/hardware-drivers/WIFIUpdate/WiFi.o 

C_DEPS += \
./src/hardware-drivers/WIFIUpdate/WiFi.d 


# Each subdirectory must supply rules for building sources it contributes
src/hardware-drivers/WIFIUpdate/%.o: ../src/hardware-drivers/WIFIUpdate/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler 4 [arm-linux-gnueabihf]'
	arm-linux-gnueabihf-gcc -std=c99 -Dsoc_cv_av -I"C:\intelFPGA\17.1\embedded\ip\altera\hps\altera_hps\hwlib\include" -I"C:\intelFPGA\17.1\embedded\ip\altera\hps\altera_hps\hwlib\include\soc_cv_av" -I"C:\CPEN391\techtrek-soc\techtrek\include\hardware-drivers" -I"C:\CPEN391\techtrek-soc\techtrek\include" -I"C:\CPEN391\techtrek-soc\techtrek\include\graphics" -O0 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



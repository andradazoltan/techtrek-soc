################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += ../src/graphics/ColourPallette.c \
../src/graphics/Fill.c \
../src/graphics/Fonts.c \
../src/graphics/GraphData.c \
../src/graphics/Graphics.c \
../src/graphics/Keyboard.c \
../src/graphics/OutGraphicsCharFont1.c \
../src/graphics/OutGraphicsCharFont2.c \
../src/graphics/OutGraphicsCharFont3.c \
../src/graphics/OutGraphicsCharFont4.c \
../src/graphics/OutGraphicsCharFont5.c \
../src/graphics/Screens.c \
../src/graphics/images.c

OBJS += \
./src/graphics/ColourPallette.o \
./src/graphics/Fill.o \
./src/graphics/Fonts.o \
./src/graphics/GraphData.o \
./src/graphics/Graphics.o \
./src/graphics/Keyboard.o \
./src/graphics/OutGraphicsCharFont1.o \
./src/graphics/OutGraphicsCharFont2.o \
./src/graphics/OutGraphicsCharFont3.o \
./src/graphics/OutGraphicsCharFont4.o \
./src/graphics/OutGraphicsCharFont5.o \
./src/graphics/Screens.o \
./src/graphics/images.o

C_DEPS += \
./src/graphics/ColourPallette.d \
./src/graphics/Fill.d \
./src/graphics/Fonts.d \
./src/graphics/GraphData.d \
./src/graphics/Graphics.d \
./src/graphics/Keyboard.d \
./src/graphics/OutGraphicsCharFont1.d \
./src/graphics/OutGraphicsCharFont2.d \
./src/graphics/OutGraphicsCharFont3.d \
./src/graphics/OutGraphicsCharFont4.d \
./src/graphics/OutGraphicsCharFont5.d \
./src/graphics/Screens.d \
./src/graphics/images.d


# Each subdirectory must supply rules for building sources it contributes
src/graphics/%.o: ../src/graphics/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler 4 [arm-linux-gnueabihf]'
	arm-linux-gnueabihf-gcc -std=c99 -Dsoc_cv_av -I"/home/jacob/cpen391/techtrek-soc/techtrek/include/hardware-drivers" -I"/home/jacob/cpen391/techtrek-soc/techtrek/include" -I"/home/jacob/cpen391/techtrek-soc/techtrek/include/graphics" -O0 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



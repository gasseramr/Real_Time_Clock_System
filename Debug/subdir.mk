################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../alarm.c \
../buttons.c \
../buzzer.c \
../countdown.c \
../lcd.c \
../main.c \
../rtc.c \
../stopwatch.c \
../time_utils.c 

OBJS += \
./alarm.o \
./buttons.o \
./buzzer.o \
./countdown.o \
./lcd.o \
./main.o \
./rtc.o \
./stopwatch.o \
./time_utils.o 

C_DEPS += \
./alarm.d \
./buttons.d \
./buzzer.d \
./countdown.d \
./lcd.d \
./main.d \
./rtc.d \
./stopwatch.d \
./time_utils.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



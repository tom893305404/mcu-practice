################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/SetSysClockTo72.c \
../src/SystemClock_Config.c \
../src/initialize-hardware.c \
../src/main.c \
../src/write.c 

OBJS += \
./src/SetSysClockTo72.o \
./src/SystemClock_Config.o \
./src/initialize-hardware.o \
./src/main.o \
./src/write.o 

C_DEPS += \
./src/SetSysClockTo72.d \
./src/SystemClock_Config.d \
./src/initialize-hardware.d \
./src/main.d \
./src/write.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F103xB -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



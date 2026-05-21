################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Coin.c \
../src/CountNumHeads.c 

C_DEPS += \
./src/Coin.d \
./src/CountNumHeads.d 

OBJS += \
./src/Coin.o \
./src/CountNumHeads.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/Coin.d ./src/Coin.o ./src/CountNumHeads.d ./src/CountNumHeads.o

.PHONY: clean-src


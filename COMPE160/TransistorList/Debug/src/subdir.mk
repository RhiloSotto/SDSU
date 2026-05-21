################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/TransistorList.c \
../src/TransistorListNode.c 

C_DEPS += \
./src/TransistorList.d \
./src/TransistorListNode.d 

OBJS += \
./src/TransistorList.o \
./src/TransistorListNode.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/TransistorList.d ./src/TransistorList.o ./src/TransistorListNode.d ./src/TransistorListNode.o

.PHONY: clean-src


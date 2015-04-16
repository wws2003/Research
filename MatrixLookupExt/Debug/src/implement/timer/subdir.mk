################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/implement/timer/ScopeTimer.cpp 

OBJS += \
./src/implement/timer/ScopeTimer.o 

CPP_DEPS += \
./src/implement/timer/ScopeTimer.d 


# Each subdirectory must supply rules for building sources it contributes
src/implement/timer/%.o: ../src/implement/timer/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/pham/workspace-c/MatrixLookupExt/include" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



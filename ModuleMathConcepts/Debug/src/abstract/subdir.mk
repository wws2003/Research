################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/abstract/ISquareMatrix.cpp 

OBJS += \
./src/abstract/ISquareMatrix.o 

CPP_DEPS += \
./src/abstract/ISquareMatrix.d 


# Each subdirectory must supply rules for building sources it contributes
src/abstract/%.o: ../src/abstract/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../include/interface/abstract -I../include/interface/common -I../include/implement -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



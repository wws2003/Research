################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/abstract/basic/ISquareMatrix.cpp 

OBJS += \
./src/abstract/basic/ISquareMatrix.o 

CPP_DEPS += \
./src/abstract/basic/ISquareMatrix.d 


# Each subdirectory must supply rules for building sources it contributes
src/abstract/basic/%.o: ../src/abstract/basic/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/pham/workspace-c/MatrixLookupExt/include" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/implement/basic/math_concept/SimpleDenseMatrixImpl.cpp 

OBJS += \
./src/implement/basic/math_concept/SimpleDenseMatrixImpl.o 

CPP_DEPS += \
./src/implement/basic/math_concept/SimpleDenseMatrixImpl.d 


# Each subdirectory must supply rules for building sources it contributes
src/implement/basic/math_concept/%.o: ../src/implement/basic/math_concept/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/pham/workspace-c/MatrixLookupExt/include" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/TestMatrixOperator.cpp \
../src/TestSuite.cpp 

OBJS += \
./src/TestMatrixOperator.o \
./src/TestSuite.o 

CPP_DEPS += \
./src/TestMatrixOperator.d \
./src/TestSuite.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../../ModuleMathConcepts/include/interface/abstract -I../../ModuleMathConcepts/include/interface/common -I../../ModuleMathConcepts/include/implement -I../../ModuleOperator/include/interface/abstract -I../../ModuleOperator/include/interface/common -I../../ModuleOperator/include/implement -I../../ModuleOperator/third-parties -O0 -g3 -Wall -c -fmessage-length=0 -std=gnu++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



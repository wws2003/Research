################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/MatrixLookupExtApp.cpp 

OBJS += \
./src/MatrixLookupExtApp.o 

CPP_DEPS += \
./src/MatrixLookupExtApp.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../../ModuleMathConcepts/include/interface/abstract -I../../ModuleMathConcepts/include/interface/common -I../../ModuleAlgorithm/include/interface/abstract -I../../ModuleAlgorithm/include/interface/common -I../../ModuleOperator/include/interface/abstract -I../../ModuleOperator/include/interface/common -I../../ModuleIO/include/interface/abstract -I../../ModuleIO/include/interface/common -I../../ModuleEvaluate/include/interface/abstract -I../../ModuleEvaluate/include/interface/common -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



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
	g++ -I../../ModuleMathConcepts/include/abstract -I../../ModuleMathConcepts/include/common -I../../ModuleAlgorithm/include/abstract -I../../ModuleAlgorithm/include/common -I../../ModuleOperator/include/abstract -I../../ModuleOperator/include/common -I../../ModuleIO/include/abstract -I../../ModuleIO/include/common -I../../ModuleEvaluate/include/abstract -I../../ModuleEvaluate/include/common -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



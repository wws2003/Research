################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/implement/bin/MatrixBin.cpp 

OBJS += \
./src/implement/bin/MatrixBin.o 

CPP_DEPS += \
./src/implement/bin/MatrixBin.d 


# Each subdirectory must supply rules for building sources it contributes
src/implement/bin/%.o: ../src/implement/bin/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../../ModuleMathConcepts/include/interface/abstract -I../../ModuleMathConcepts/include/interface/common -I../../ModuleOperator/include/interface/abstract -I../../ModuleOperator/include/interface/common -I../include/interface/abstract -I../include/interface/common -I../include/implement -I../include/implement/iterator -I../include/implement/collection -I../include/implement/approximator -I../include/implement/bin -I../include/internal-abstract -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



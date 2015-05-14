################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/implement/MatrixTraceDistanceCalculator.cpp \
../src/implement/SampleMatrixOperator.cpp 

OBJS += \
./src/implement/MatrixTraceDistanceCalculator.o \
./src/implement/SampleMatrixOperator.o 

CPP_DEPS += \
./src/implement/MatrixTraceDistanceCalculator.d \
./src/implement/SampleMatrixOperator.d 


# Each subdirectory must supply rules for building sources it contributes
src/implement/%.o: ../src/implement/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../../ModuleMathConcepts/include/interface/abstract -I../../ModuleMathConcepts/include/interface/common -I../include/interface/abstract -I../include/interface/common -I../include/implement -I../third-parties -I../third-parties/eigen3 -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



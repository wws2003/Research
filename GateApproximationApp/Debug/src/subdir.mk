################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Application.cpp \
../src/GateFactory.cpp \
../src/HVBasedResourceContainerImpl.cpp \
../src/SampleAppContainerImpl.cpp \
../src/SampleResourceContainerImpl.cpp 

OBJS += \
./src/Application.o \
./src/GateFactory.o \
./src/HVBasedResourceContainerImpl.o \
./src/SampleAppContainerImpl.o \
./src/SampleResourceContainerImpl.o 

CPP_DEPS += \
./src/Application.d \
./src/GateFactory.d \
./src/HVBasedResourceContainerImpl.d \
./src/SampleAppContainerImpl.d \
./src/SampleResourceContainerImpl.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../../ModuleMathConcepts/include/interface/abstract -I../../ModuleMathConcepts/include/interface/common -I../../ModuleMathConcepts/include/implement -I../../ModuleOperator/include/interface/abstract -I../../ModuleOperator/include/interface/common -I../../ModuleOperator/include/implement -I../../ModuleOperator/third-parties -I../../ModuleIO/include/interface/abstract -I../../ModuleIO/include/interface/common -I../../ModuleIO/include/implement -I../../ModuleAlgorithm/include/interface/abstract -I../../ModuleAlgorithm/include/interface/common -I../../ModuleAlgorithm/include/implement -I../../ModuleAlgorithm/include/implement/iterator -I../../ModuleAlgorithm/include/implement/collection -I../../ModuleAlgorithm/include/implement/persistable -I../../ModuleAlgorithm/include/implement/combiner -I../../ModuleAlgorithm/include/implement/approximator -I../../ModuleAlgorithm/include/implement/bin -I../../ModuleAlgorithm/include/implement/internal-abstract -I../../ModuleAlgorithm/include/internal-abstract -I../../ModuleAlgorithm/include/internal-abstract/collection -I../../ModuleAlgorithm/include/internal-abstract/combiner -I../../ModuleEvaluate/include/interface/abstract -I../../ModuleEvaluate/include/interface/common -I../../ModuleEvaluate/include/implement -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



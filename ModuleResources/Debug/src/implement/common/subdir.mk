################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/implement/common/GateSpecification.cpp 

OBJS += \
./src/implement/common/GateSpecification.o 

CPP_DEPS += \
./src/implement/common/GateSpecification.d 


# Each subdirectory must supply rules for building sources it contributes
src/implement/common/%.o: ../src/implement/common/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__cplusplus=201103L -D__GXX_EXPERIMENTAL_CXX0X__ -I../../ModuleMathConcepts/include/interface/abstract -I../../ModuleMathConcepts/include/interface/common -I../../ModuleOperator/include/interface/abstract -I../../ModuleOperator/include/interface/common -I../include/interface/abstract -I../include/interface/abstract/checker -I../include/interface/abstract/gate_store -I../include/interface/common -I../include/implement -I../include/implement/checker -I../include/implement/gate_store -I../include/implement/internal-common -I../../SharedThirdParties/gmp/include -I../../SharedThirdParties/mpfr/include -I../../SharedThirdParties/mpfrc++/include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



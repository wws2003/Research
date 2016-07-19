################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/implement/internal/SelingerProgramAdapter.cpp 

OBJS += \
./src/implement/internal/SelingerProgramAdapter.o 

CPP_DEPS += \
./src/implement/internal/SelingerProgramAdapter.d 


# Each subdirectory must supply rules for building sources it contributes
src/implement/internal/%.o: ../src/implement/internal/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -D__cplusplus=201103L -I../../ModuleMathConcepts/include/interface/abstract -I../../ModuleMathConcepts/include/interface/common -I../../ModuleAlgorithm/include/interface/abstract -I../../ModuleAlgorithm/include/interface/common -I../../ModuleOperator/include/interface/abstract -I../../ModuleOperator/include/interface/common -I../../ModuleIO/include/interface/abstract -I../../ModuleIO/include/interface/common -I../../SharedThirdParties/gmp/include -I../../SharedThirdParties/mpfr/include -I../../SharedThirdParties/mpfrc++/include -I../include/interface/abstract -I../include/interface/common -I../include/implement -I../include/implement/internal -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -fPIC -pg -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



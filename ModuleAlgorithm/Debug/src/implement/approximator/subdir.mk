################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/implement/approximator/DummyMatrixApproximator.cpp \
../src/implement/approximator/NINAMatrixApproximator.cpp \
../src/implement/approximator/NearIdentityMatrixApproximator.cpp 

OBJS += \
./src/implement/approximator/DummyMatrixApproximator.o \
./src/implement/approximator/NINAMatrixApproximator.o \
./src/implement/approximator/NearIdentityMatrixApproximator.o 

CPP_DEPS += \
./src/implement/approximator/DummyMatrixApproximator.d \
./src/implement/approximator/NINAMatrixApproximator.d \
./src/implement/approximator/NearIdentityMatrixApproximator.d 


# Each subdirectory must supply rules for building sources it contributes
src/implement/approximator/%.o: ../src/implement/approximator/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../../ModuleMathConcepts/include/interface/abstract -I../../ModuleMathConcepts/include/interface/common -I../../ModuleOperator/include/interface/abstract -I../../ModuleOperator/include/interface/common -I../include/interface/abstract -I../include/interface/common -I../include/implement -I../include/implement/combiner -I../include/implement/iterator -I../include/implement/collection -I../include/implement/approximator -I../include/implement/bin -I../include/internal-abstract -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



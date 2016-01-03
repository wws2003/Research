################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/common/Gate.cpp \
../src/common/Mreal.cpp 

OBJS += \
./src/common/Gate.o \
./src/common/Mreal.o 

CPP_DEPS += \
./src/common/Gate.d \
./src/common/Mreal.d 


# Each subdirectory must supply rules for building sources it contributes
src/common/%.o: ../src/common/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../include/interface/abstract -I../include/interface/common -I../include/implement -I../../SharedThirdParties/gmp/include -I../../SharedThirdParties/mpfr/include -I../../SharedThirdParties/mpfrc++/include -O3 -g3 -Wall -c -fmessage-length=0 -std=c++11 -fPIC -pg -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



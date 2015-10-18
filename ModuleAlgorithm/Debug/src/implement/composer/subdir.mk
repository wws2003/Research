################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/implement/composer/NearIdentityElementBinBasedComposer.cpp \
../src/implement/composer/NearIdentityGateBinBasedComposer.cpp \
../src/implement/composer/NearIdentityMatrixBinBasedComposer.cpp 

OBJS += \
./src/implement/composer/NearIdentityElementBinBasedComposer.o \
./src/implement/composer/NearIdentityGateBinBasedComposer.o \
./src/implement/composer/NearIdentityMatrixBinBasedComposer.o 

CPP_DEPS += \
./src/implement/composer/NearIdentityElementBinBasedComposer.d \
./src/implement/composer/NearIdentityGateBinBasedComposer.d \
./src/implement/composer/NearIdentityMatrixBinBasedComposer.d 


# Each subdirectory must supply rules for building sources it contributes
src/implement/composer/%.o: ../src/implement/composer/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../../ModuleMathConcepts/include/interface/abstract -I../../ModuleMathConcepts/include/interface/common -I../../ModuleOperator/include/interface/abstract -I../../ModuleOperator/include/interface/common -I../../ModuleIO/include/interface/abstract -I../../ModuleIO/include/interface/common -I../../SharedThirdParties/gmp/include -I../../SharedThirdParties/mpfr/include -I../../SharedThirdParties/mpfrc++/include -I../include/interface/abstract -I../include/interface/common -I../include/implement -I../include/implement/combiner -I../include/implement/iterator -I../include/implement/collection -I../include/implement/composer -I../include/implement/decomposer -I../include/implement/approximator -I../include/implement/bin -I../include/implement/persistable -I../include/internal-abstract -I../include/internal-abstract/collection -I../include/internal-abstract/combiner -I../src/implement/approximator -I../src/implement/collection -I../src/implement/composer -I../src/implement/persitable -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/implement/persitable/GNATCollectionBlock.cpp \
../src/implement/persitable/PersistableGNATCollectionImpl.cpp \
../src/implement/persitable/PersistableGNATGateCollectionImpl.cpp 

OBJS += \
./src/implement/persitable/GNATCollectionBlock.o \
./src/implement/persitable/PersistableGNATCollectionImpl.o \
./src/implement/persitable/PersistableGNATGateCollectionImpl.o 

CPP_DEPS += \
./src/implement/persitable/GNATCollectionBlock.d \
./src/implement/persitable/PersistableGNATCollectionImpl.d \
./src/implement/persitable/PersistableGNATGateCollectionImpl.d 


# Each subdirectory must supply rules for building sources it contributes
src/implement/persitable/%.o: ../src/implement/persitable/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../../ModuleMathConcepts/include/interface/abstract -I../../ModuleMathConcepts/include/interface/common -I../../ModuleOperator/include/interface/abstract -I../../ModuleOperator/include/interface/common -I../../ModuleIO/include/interface/abstract -I../../ModuleIO/include/interface/common -I../../SharedThirdParties/gmp/include -I../../SharedThirdParties/mpfr/include -I../../SharedThirdParties/mpfrc++/include -I../include/interface/abstract -I../include/interface/common -I../include/implement -I../include/implement/combiner -I../include/implement/iterator -I../include/implement/collection -I../include/implement/approximator -I../include/implement/bin -I../include/implement/persistable -I../include/internal-abstract -I../include/internal-abstract/collection -I../include/internal-abstract/combiner -I../src/implement/approximator -I../src/implement/collection -I../src/implement/persitable -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



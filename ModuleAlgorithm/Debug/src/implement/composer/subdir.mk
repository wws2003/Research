################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/implement/composer/AdaptiveElementComposer.cpp \
../src/implement/composer/AdaptiveGateCoordinateComposer.cpp \
../src/implement/composer/AdaptiveMatrixCoordinateComposer.cpp \
../src/implement/composer/AdditionBasedElementComposer.cpp \
../src/implement/composer/CoordinateAdditionBasedGateComposer.cpp \
../src/implement/composer/CoordinateAdditionBasedMatrixComposer.cpp \
../src/implement/composer/MultipleComparatorAdditionBasedElementComposer.cpp \
../src/implement/composer/MultipleComparatorCoordinateAdditionBasedGateComposer.cpp \
../src/implement/composer/MultipleComparatorCoordinateAdditionBasedMatrixComposer.cpp \
../src/implement/composer/NearIdentityElementBinBasedComposer.cpp \
../src/implement/composer/NearIdentityGateBinBasedComposer.cpp \
../src/implement/composer/NearIdentityMatrixBinBasedComposer.cpp \
../src/implement/composer/SimpleElementComposer.cpp \
../src/implement/composer/SimpleGateComposer.cpp \
../src/implement/composer/SimpleMatrixComposer.cpp 

OBJS += \
./src/implement/composer/AdaptiveElementComposer.o \
./src/implement/composer/AdaptiveGateCoordinateComposer.o \
./src/implement/composer/AdaptiveMatrixCoordinateComposer.o \
./src/implement/composer/AdditionBasedElementComposer.o \
./src/implement/composer/CoordinateAdditionBasedGateComposer.o \
./src/implement/composer/CoordinateAdditionBasedMatrixComposer.o \
./src/implement/composer/MultipleComparatorAdditionBasedElementComposer.o \
./src/implement/composer/MultipleComparatorCoordinateAdditionBasedGateComposer.o \
./src/implement/composer/MultipleComparatorCoordinateAdditionBasedMatrixComposer.o \
./src/implement/composer/NearIdentityElementBinBasedComposer.o \
./src/implement/composer/NearIdentityGateBinBasedComposer.o \
./src/implement/composer/NearIdentityMatrixBinBasedComposer.o \
./src/implement/composer/SimpleElementComposer.o \
./src/implement/composer/SimpleGateComposer.o \
./src/implement/composer/SimpleMatrixComposer.o 

CPP_DEPS += \
./src/implement/composer/AdaptiveElementComposer.d \
./src/implement/composer/AdaptiveGateCoordinateComposer.d \
./src/implement/composer/AdaptiveMatrixCoordinateComposer.d \
./src/implement/composer/AdditionBasedElementComposer.d \
./src/implement/composer/CoordinateAdditionBasedGateComposer.d \
./src/implement/composer/CoordinateAdditionBasedMatrixComposer.d \
./src/implement/composer/MultipleComparatorAdditionBasedElementComposer.d \
./src/implement/composer/MultipleComparatorCoordinateAdditionBasedGateComposer.d \
./src/implement/composer/MultipleComparatorCoordinateAdditionBasedMatrixComposer.d \
./src/implement/composer/NearIdentityElementBinBasedComposer.d \
./src/implement/composer/NearIdentityGateBinBasedComposer.d \
./src/implement/composer/NearIdentityMatrixBinBasedComposer.d \
./src/implement/composer/SimpleElementComposer.d \
./src/implement/composer/SimpleGateComposer.d \
./src/implement/composer/SimpleMatrixComposer.d 


# Each subdirectory must supply rules for building sources it contributes
src/implement/composer/%.o: ../src/implement/composer/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__cplusplus=201103L -I../../ModuleMathConcepts/include/interface/abstract -I../../ModuleMathConcepts/include/interface/common -I../../ModuleOperator/include/interface/abstract -I../../ModuleOperator/include/interface/common -I../../ModuleIO/include/interface/abstract -I../../ModuleIO/include/interface/common -I../../ModuleParallelism/include/interface/common -I../../ModuleParallelism/include/interface/abstract -I../../SharedThirdParties/gmp/include -I../../SharedThirdParties/mpfr/include -I../../SharedThirdParties/mpfrc++/include -I../include/interface/abstract -I../include/interface/common -I../include/implement -I../include/implement/combiner -I../include/implement/comparator -I../include/implement/iterator -I../include/implement/collection -I../include/implement/composer -I../include/implement/decomposer -I../include/implement/approximator -I../include/implement/bin -I../include/implement/persistable -I../include/implement/filter -I../include/implement/task -I../include/internal-abstract -I../include/internal-abstract/collection -I../include/internal-abstract/combiner -I../include/internal-abstract/persistable -I../include/internal-abstract/filter -I../include/internal-abstract/comparator -I../include/internal-abstract/converter -I../include/internal-abstract/bin -I../src/implement/approximator -I../src/implement/collection -I../src/implement/converter -I../src/implement/composer -I../src/implement/persitable -I../src/implement/task -O3 -g3 -Wall -c -fmessage-length=0 -std=c++11 -fPIC -pg -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



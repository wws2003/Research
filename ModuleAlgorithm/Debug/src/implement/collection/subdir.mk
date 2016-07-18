################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/implement/collection/GNATCollectionImpl.cpp \
../src/implement/collection/GNATGateCollectionImpl.cpp \
../src/implement/collection/MapBasedBinCollectionImpl.cpp \
../src/implement/collection/MapBasedGateBinCollectionImpl.cpp \
../src/implement/collection/MapBasedMatrixBinCollectionImpl.cpp \
../src/implement/collection/VectorBasedMatrixCollectionImpl.cpp 

OBJS += \
./src/implement/collection/GNATCollectionImpl.o \
./src/implement/collection/GNATGateCollectionImpl.o \
./src/implement/collection/MapBasedBinCollectionImpl.o \
./src/implement/collection/MapBasedGateBinCollectionImpl.o \
./src/implement/collection/MapBasedMatrixBinCollectionImpl.o \
./src/implement/collection/VectorBasedMatrixCollectionImpl.o 

CPP_DEPS += \
./src/implement/collection/GNATCollectionImpl.d \
./src/implement/collection/GNATGateCollectionImpl.d \
./src/implement/collection/MapBasedBinCollectionImpl.d \
./src/implement/collection/MapBasedGateBinCollectionImpl.d \
./src/implement/collection/MapBasedMatrixBinCollectionImpl.d \
./src/implement/collection/VectorBasedMatrixCollectionImpl.d 


# Each subdirectory must supply rules for building sources it contributes
src/implement/collection/%.o: ../src/implement/collection/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__cplusplus=201103L -I../../ModuleMathConcepts/include/interface/abstract -I../../ModuleMathConcepts/include/interface/common -I../../ModuleOperator/include/interface/abstract -I../../ModuleOperator/include/interface/common -I../../ModuleIO/include/interface/abstract -I../../ModuleIO/include/interface/common -I../../ModuleParallelism/include/interface/common -I../../ModuleParallelism/include/interface/abstract -I../../ModuleResources/include/interface/abstract -I../../ModuleResources/include/interface/abstract/checker -I../../ModuleResources/include/interface/abstract/gate_store -I../../ModuleResources/include/interface/common -I../../SharedThirdParties/gmp/include -I../../SharedThirdParties/mpfr/include -I../../SharedThirdParties/mpfrc++/include -I../include/interface/abstract -I../include/interface/common -I../include/implement -I../include/implement/combiner -I../include/implement/comparator -I../include/implement/iterator -I../include/implement/collection -I../include/implement/composer -I../include/implement/decomposer -I../include/implement/approximator -I../include/implement/bin -I../include/implement/persistable -I../include/implement/filter -I../include/implement/task -I../include/implement/set -I../include/internal-abstract -I../include/internal-abstract/collection -I../include/internal-abstract/combiner -I../include/internal-abstract/persistable -I../include/internal-abstract/filter -I../include/internal-abstract/comparator -I../include/internal-abstract/converter -I../include/internal-abstract/bin -I../include/internal-abstract/tree -I../src/implement/approximator -I../src/implement/collection -I../src/implement/converter -I../src/implement/composer -I../src/implement/persitable -I../src/implement/task -I../src/implement/set -I../src/implement/tree -O3 -g3 -Wall -c -fmessage-length=0 -std=c++11 -fPIC -pg -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



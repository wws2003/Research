################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/implement/task/ElementsCombinationVerifyTask.cpp \
../src/implement/task/GateCoordinateCombiningTask.cpp \
../src/implement/task/GatesCombinationVerifyTask.cpp \
../src/implement/task/GatesCoordinateCombinationVerifyTask.cpp \
../src/implement/task/MatricesCombinationVerifyTask.cpp 

OBJS += \
./src/implement/task/ElementsCombinationVerifyTask.o \
./src/implement/task/GateCoordinateCombiningTask.o \
./src/implement/task/GatesCombinationVerifyTask.o \
./src/implement/task/GatesCoordinateCombinationVerifyTask.o \
./src/implement/task/MatricesCombinationVerifyTask.o 

CPP_DEPS += \
./src/implement/task/ElementsCombinationVerifyTask.d \
./src/implement/task/GateCoordinateCombiningTask.d \
./src/implement/task/GatesCombinationVerifyTask.d \
./src/implement/task/GatesCoordinateCombinationVerifyTask.d \
./src/implement/task/MatricesCombinationVerifyTask.d 


# Each subdirectory must supply rules for building sources it contributes
src/implement/task/%.o: ../src/implement/task/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__cplusplus=201103L -I../../ModuleMathConcepts/include/interface/abstract -I../../ModuleMathConcepts/include/interface/common -I../../ModuleOperator/include/interface/abstract -I../../ModuleOperator/include/interface/common -I../../ModuleIO/include/interface/abstract -I../../ModuleIO/include/interface/common -I../../ModuleParallelism/include/interface/common -I../../ModuleParallelism/include/interface/abstract -I../../ModuleResources/include/interface/abstract -I../../ModuleResources/include/interface/abstract/checker -I../../ModuleResources/include/interface/abstract/gate_store -I../../ModuleResources/include/interface/common -I../../SharedThirdParties/gmp/include -I../../SharedThirdParties/mpfr/include -I../../SharedThirdParties/mpfrc++/include -I../include/interface/abstract -I../include/interface/common -I../include/implement -I../include/implement/combiner -I../include/implement/comparator -I../include/implement/iterator -I../include/implement/collection -I../include/implement/composer -I../include/implement/decomposer -I../include/implement/approximator -I../include/implement/bin -I../include/implement/persistable -I../include/implement/filter -I../include/implement/task -I../include/internal-abstract -I../include/internal-abstract/collection -I../include/internal-abstract/combiner -I../include/internal-abstract/persistable -I../include/internal-abstract/filter -I../include/internal-abstract/comparator -I../include/internal-abstract/converter -I../include/internal-abstract/bin -I../src/implement/approximator -I../src/implement/collection -I../src/implement/converter -I../src/implement/composer -I../src/implement/persitable -I../src/implement/task -O3 -g3 -Wall -c -fmessage-length=0 -std=c++11 -fPIC -pg -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/implement/collection/SampleMatrixBinCollectionImpl.cpp \
../src/implement/collection/SampleMatrixCollectionImpl.cpp \
../src/implement/collection/VectorizationMatrixCollectionImpl.cpp 

OBJS += \
./src/implement/collection/SampleMatrixBinCollectionImpl.o \
./src/implement/collection/SampleMatrixCollectionImpl.o \
./src/implement/collection/VectorizationMatrixCollectionImpl.o 

CPP_DEPS += \
./src/implement/collection/SampleMatrixBinCollectionImpl.d \
./src/implement/collection/SampleMatrixCollectionImpl.d \
./src/implement/collection/VectorizationMatrixCollectionImpl.d 


# Each subdirectory must supply rules for building sources it contributes
src/implement/collection/%.o: ../src/implement/collection/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../../ModuleMathConcepts/include/interface/abstract -I../../ModuleMathConcepts/include/interface/common -I../../ModuleOperator/include/interface/abstract -I../../ModuleOperator/include/interface/common -I../include/interface/abstract -I../include/interface/common -I../include/implement -I../include/implement/combiner -I../include/implement/iterator -I../include/implement/collection -I../include/implement/approximator -I../include/implement/bin -I../include/internal-abstract -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



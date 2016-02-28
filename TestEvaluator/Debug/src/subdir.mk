################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/AssemblerUtil.cpp \
../src/FullTestSuite.cpp \
../src/IOTestSuite.cpp \
../src/IntegratingTester.cpp \
../src/TestEvaluator.cpp 

OBJS += \
./src/AssemblerUtil.o \
./src/FullTestSuite.o \
./src/IOTestSuite.o \
./src/IntegratingTester.o \
./src/TestEvaluator.o 

CPP_DEPS += \
./src/AssemblerUtil.d \
./src/FullTestSuite.d \
./src/IOTestSuite.d \
./src/IntegratingTester.d \
./src/TestEvaluator.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../../SharedThirdParties/gmp/include -I../../SharedThirdParties/mpfr/include -I../../SharedThirdParties/mpfrc++/include -I../../ModuleMathConcepts/include/interface/abstract -I../../ModuleMathConcepts/include/interface/common -I../../ModuleMathConcepts/include/implement -I../../ModuleOperator/include/interface/abstract -I../../ModuleOperator/include/interface/common -I../../ModuleOperator/include/implement -I../../ModuleOperator/third-parties -I../../ModuleIO/include/interface/abstract -I../../ModuleIO/include/interface/common -I../../ModuleIO/include/implement -I../../ModuleAlgorithm/include/interface/abstract -I../../ModuleAlgorithm/include/interface/common -I../../ModuleAlgorithm/include/implement -I../../ModuleAlgorithm/include/implement/iterator -I../../ModuleAlgorithm/include/implement/collection -I../../ModuleAlgorithm/include/implement/persistable -I../../ModuleAlgorithm/include/implement/combiner -I../../ModuleAlgorithm/include/implement/composer -I../../ModuleAlgorithm/include/implement/approximator -I../../ModuleAlgorithm/include/implement/comparator -I../../ModuleAlgorithm/include/implement/filter -I../../ModuleAlgorithm/include/implement/task -I../../ModuleAlgorithm/include/implement/bin -I../../ModuleAlgorithm/include/implement/internal-abstract -I../../ModuleAlgorithm/include/internal-abstract -I../../ModuleAlgorithm/include/internal-abstract/comparator -I../../ModuleAlgorithm/include/internal-abstract/collection -I../../ModuleAlgorithm/include/internal-abstract/combiner -I../../ModuleAlgorithm/include/internal-abstract/filter -I../../ModuleEvaluate/include/interface/abstract -I../../ModuleEvaluate/include/interface/common -I../../ModuleEvaluate/include/implement -I../../ModuleParallelism/include/interface/abstract -I../../ModuleParallelism/include/interface/common -I../../ModuleParallelism/include/interface/internal-abstract -I../../ModuleParallelism/include/implement -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



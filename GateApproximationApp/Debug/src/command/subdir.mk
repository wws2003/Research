################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/command/AbstractChainCommandExecutor.cpp \
../src/command/CommandFactory.cpp \
../src/command/CommandParser.cpp \
../src/command/EvaluateApproximatorCommand.cpp \
../src/command/EvaluateCollectionCommand.cpp \
../src/command/EvaluateComposerCommand.cpp \
../src/command/NotAvailableCommand.cpp 

OBJS += \
./src/command/AbstractChainCommandExecutor.o \
./src/command/CommandFactory.o \
./src/command/CommandParser.o \
./src/command/EvaluateApproximatorCommand.o \
./src/command/EvaluateCollectionCommand.o \
./src/command/EvaluateComposerCommand.o \
./src/command/NotAvailableCommand.o 

CPP_DEPS += \
./src/command/AbstractChainCommandExecutor.d \
./src/command/CommandFactory.d \
./src/command/CommandParser.d \
./src/command/EvaluateApproximatorCommand.d \
./src/command/EvaluateCollectionCommand.d \
./src/command/EvaluateComposerCommand.d \
./src/command/NotAvailableCommand.d 


# Each subdirectory must supply rules for building sources it contributes
src/command/%.o: ../src/command/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../src/command -I../src/configure -I../src/common -I../src/container -I../src/factory -I../src/concrete-from-templates -I../../ModuleMathConcepts/include/interface/abstract -I../../ModuleMathConcepts/include/interface/common -I../../ModuleMathConcepts/include/implement -I../../SharedThirdParties/gmp/include -I../../SharedThirdParties/mpfr/include -I../../SharedThirdParties/mpfrc++/include -I../../ModuleOperator/include/interface/abstract -I../../ModuleOperator/include/interface/common -I../../ModuleOperator/include/implement -I../../ModuleOperator/third-parties -I../../ModuleIO/include/interface/abstract -I../../ModuleIO/include/interface/common -I../../ModuleIO/include/implement -I../../ModuleAlgorithm/include/interface/abstract -I../../ModuleAlgorithm/include/interface/common -I../../ModuleAlgorithm/include/implement -I../../ModuleAlgorithm/include/implement/iterator -I../../ModuleAlgorithm/include/implement/collection -I../../ModuleAlgorithm/include/implement/composer -I../../ModuleAlgorithm/include/implement/decomposer -I../../ModuleAlgorithm/include/implement/persistable -I../../ModuleAlgorithm/include/implement/combiner -I../../ModuleAlgorithm/include/implement/composer -I../../ModuleAlgorithm/include/implement/decomposer -I../../ModuleAlgorithm/include/implement/approximator -I../../ModuleAlgorithm/include/implement/bin -I../../ModuleAlgorithm/include/implement/filter -I../../ModuleAlgorithm/include/implement/comparator -I../../ModuleAlgorithm/include/implement/converter -I../../ModuleAlgorithm/include/implement/task -I../../ModuleAlgorithm/include/implement/set -I../../ModuleAlgorithm/include/implement/internal-abstract -I../../ModuleAlgorithm/include/internal-abstract -I../../ModuleAlgorithm/include/internal-abstract/collection -I../../ModuleAlgorithm/include/internal-abstract/combiner -I../../ModuleAlgorithm/include/internal-abstract/persistable -I../../ModuleAlgorithm/include/internal-abstract/comparator -I../../ModuleAlgorithm/include/internal-abstract/converter -I../../ModuleAlgorithm/include/internal-abstract/filter -I../../ModuleAlgorithm/include/internal-abstract/tree -I../../ModuleEvaluate/include/interface/abstract -I../../ModuleEvaluate/include/interface/common -I../../ModuleEvaluate/include/implement -I../../ModuleEvaluate/include/implement/internal -I../../ModuleParallelism/include/interface/abstract -I../../ModuleParallelism/include/interface/common -I../../ModuleParallelism/include/implement -I../../ModuleParallelism/include/internal-abstract -I../../ModuleParallelism/src/implement -I../../ModuleResources/include/interface/abstract/checker -I../../ModuleResources/include/interface/abstract/gate_store -I../../ModuleResources/include/interface/common -I../../ModuleResources/include/implement/checker -I../../ModuleResources/include/implement/gate_store -O3 -g3 -Wall -c -fmessage-length=0  -pthread -std=c++11 -fPIC -pg -Wl,--no-as-needed -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



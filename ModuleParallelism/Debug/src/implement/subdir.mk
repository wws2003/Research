################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/implement/BufferedQueueBasedTaskExecutorImpl.cpp \
../src/implement/NoLockTaskQueueImpl.cpp \
../src/implement/QueueBasedTaskExecutorImpl.cpp \
../src/implement/RWSemaphore.cpp \
../src/implement/SingleThreadTaskExecutorImpl.cpp \
../src/implement/TaskQueueImpl.cpp \
../src/implement/ThreadSpawnTaskExecutorImpl.cpp 

OBJS += \
./src/implement/BufferedQueueBasedTaskExecutorImpl.o \
./src/implement/NoLockTaskQueueImpl.o \
./src/implement/QueueBasedTaskExecutorImpl.o \
./src/implement/RWSemaphore.o \
./src/implement/SingleThreadTaskExecutorImpl.o \
./src/implement/TaskQueueImpl.o \
./src/implement/ThreadSpawnTaskExecutorImpl.o 

CPP_DEPS += \
./src/implement/BufferedQueueBasedTaskExecutorImpl.d \
./src/implement/NoLockTaskQueueImpl.d \
./src/implement/QueueBasedTaskExecutorImpl.d \
./src/implement/RWSemaphore.d \
./src/implement/SingleThreadTaskExecutorImpl.d \
./src/implement/TaskQueueImpl.d \
./src/implement/ThreadSpawnTaskExecutorImpl.d 


# Each subdirectory must supply rules for building sources it contributes
src/implement/%.o: ../src/implement/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -D__cplusplus=201103L -I"/home/pham/projects/Research/ModuleParallelism/include/interface/abstract" -I"/home/pham/projects/Research/ModuleParallelism/include/interface/common" -I"/home/pham/projects/Research/ModuleParallelism/include/implement" -I"/home/pham/projects/Research/ModuleParallelism/include/internal-abstract" -O0 -g3 -Wall -c -fmessage-length=0 -pthread -std=c++11  -lpthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



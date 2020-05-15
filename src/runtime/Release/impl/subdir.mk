################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../impl/alloc.cpp \
../impl/inception.cpp \
../impl/init_loader.cpp \
../impl/leak.cpp \
../impl/mem_list.cpp \
../impl/pre_loader.cpp \
../impl/spin_lock.cpp 

OBJS += \
./impl/alloc.o \
./impl/inception.o \
./impl/init_loader.o \
./impl/leak.o \
./impl/mem_list.o \
./impl/pre_loader.o \
./impl/spin_lock.o 

CPP_DEPS += \
./impl/alloc.d \
./impl/inception.d \
./impl/init_loader.d \
./impl/leak.d \
./impl/mem_list.d \
./impl/pre_loader.d \
./impl/spin_lock.d 


# Each subdirectory must supply rules for building sources it contributes
impl/%.o: ../impl/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -std=c++11 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../impl/meta.cpp \
../impl/rclass.cpp \
../impl/reflectable.cpp \
../impl/serializer.cpp \
../impl/type_info.cpp \
../impl/type_manager.cpp \
../impl/variant.cpp 

OBJS += \
./impl/meta.o \
./impl/rclass.o \
./impl/reflectable.o \
./impl/serializer.o \
./impl/type_info.o \
./impl/type_manager.o \
./impl/variant.o 

CPP_DEPS += \
./impl/meta.d \
./impl/rclass.d \
./impl/reflectable.d \
./impl/serializer.d \
./impl/type_info.d \
./impl/type_manager.d \
./impl/variant.d 


# Each subdirectory must supply rules for building sources it contributes
impl/%.o: ../impl/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -DDEBUG -Wno-unused-function -std=c++11 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



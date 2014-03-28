################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../aes.o \
../mersenne_twister.o \
../von_neumann.o 

C_SRCS += \
../aes.c \
../main_debut.c \
../mersenne_twister.c \
../von_neumann.c 

OBJS += \
./aes.o \
./main_debut.o \
./mersenne_twister.o \
./von_neumann.o 

C_DEPS += \
./aes.d \
./main_debut.d \
./mersenne_twister.d \
./von_neumann.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



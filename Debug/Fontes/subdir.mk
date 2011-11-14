################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Fontes/AlgRel.c \
../Fontes/Entrada.c \
../Fontes/ErrMsgs.c \
../Fontes/Estruturas.c \
../Fontes/Principal.c \
../Fontes/SQL.c 

OBJS += \
./Fontes/AlgRel.o \
./Fontes/Entrada.o \
./Fontes/ErrMsgs.o \
./Fontes/Estruturas.o \
./Fontes/Principal.o \
./Fontes/SQL.o 

C_DEPS += \
./Fontes/AlgRel.d \
./Fontes/Entrada.d \
./Fontes/ErrMsgs.d \
./Fontes/Estruturas.d \
./Fontes/Principal.d \
./Fontes/SQL.d 


# Each subdirectory must supply rules for building sources it contributes
Fontes/%.o: ../Fontes/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



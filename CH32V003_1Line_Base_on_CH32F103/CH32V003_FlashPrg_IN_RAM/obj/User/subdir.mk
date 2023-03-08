################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/main.c 

OBJS += \
./User/main.o 

C_DEPS += \
./User/main.d 


# Each subdirectory must supply rules for building sources it contributes
User/%.o: ../User/%.c
	@	@	riscv-none-embed-gcc -march=rv32ecxw -mabi=ilp32e -msmall-data-limit=0 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"E:\WCH\RISC-V\V2RAM编程下载\CH32V003_FlashPrg_IN_RAM\Debug" -I"E:\WCH\RISC-V\V2RAM编程下载\CH32V003_FlashPrg_IN_RAM\Core" -I"E:\WCH\RISC-V\V2RAM编程下载\CH32V003_FlashPrg_IN_RAM\User" -I"E:\WCH\RISC-V\V2RAM编程下载\CH32V003_FlashPrg_IN_RAM\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@


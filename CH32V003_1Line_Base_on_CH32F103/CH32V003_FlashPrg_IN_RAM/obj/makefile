################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include User/subdir.mk
-include Startup/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
CH32V003_FlashPrg_IN_RAM.bin \

SECONDARY_LIST += \
CH32V003_FlashPrg_IN_RAM.lst \

SECONDARY_SIZE += \
CH32V003_FlashPrg_IN_RAM.siz \


# All Target
all: CH32V003_FlashPrg_IN_RAM.elf secondary-outputs

# Tool invocations
CH32V003_FlashPrg_IN_RAM.elf: $(OBJS) $(USER_OBJS)
	@	@	riscv-none-embed-gcc -march=rv32ecxw -mabi=ilp32e -msmall-data-limit=0 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -T "E:\WCH\RISC-V\V2RAM±à³ÌÏÂÔØ\CH32V003_FlashPrg_IN_RAM\Ld\Link.ld" -nostartfiles -Wl,-Map,"CH32V003_FlashPrg_IN_RAM.map" --specs=nano.specs --specs=nosys.specs -o "CH32V003_FlashPrg_IN_RAM.elf" $(OBJS) $(USER_OBJS) $(LIBS) -lprintf
	@	@
CH32V003_FlashPrg_IN_RAM.bin: CH32V003_FlashPrg_IN_RAM.elf
	@	riscv-none-embed-objcopy -O binary "CH32V003_FlashPrg_IN_RAM.elf"  "CH32V003_FlashPrg_IN_RAM.bin"
	@	@
CH32V003_FlashPrg_IN_RAM.lst: CH32V003_FlashPrg_IN_RAM.elf
	@	riscv-none-embed-objdump --all-headers --demangle --disassemble -M xw "CH32V003_FlashPrg_IN_RAM.elf" > "CH32V003_FlashPrg_IN_RAM.lst"
	@	@
CH32V003_FlashPrg_IN_RAM.siz: CH32V003_FlashPrg_IN_RAM.elf
	@	riscv-none-embed-size --format=berkeley "CH32V003_FlashPrg_IN_RAM.elf"
	@	@
# Other Targets
clean:
	-$(RM) $(ASM_UPPER_DEPS)$(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS) CH32V003_FlashPrg_IN_RAM.elf
	-@
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets

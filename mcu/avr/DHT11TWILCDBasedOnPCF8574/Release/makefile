################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include uart/subdir.mk
-include tinudht/subdir.mk
-include pcf8574/subdir.mk
-include lcdpcf8574/subdir.mk
-include i2chw/subdir.mk
-include dht11/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
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

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := DHT11TWILCDBasedOnPCF8574
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
DHT11TWILCDBasedOnPCF8574.lss \

FLASH_IMAGE += \
DHT11TWILCDBasedOnPCF8574.hex \

EEPROM_IMAGE += \
DHT11TWILCDBasedOnPCF8574.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: main-build

# Main-build Target
main-build: DHT11TWILCDBasedOnPCF8574.elf secondary-outputs

# Tool invocations
DHT11TWILCDBasedOnPCF8574.elf: $(OBJS) $(USER_OBJS) makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,DHT11TWILCDBasedOnPCF8574.map -mmcu=atmega16 -o "DHT11TWILCDBasedOnPCF8574.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

DHT11TWILCDBasedOnPCF8574.lss: DHT11TWILCDBasedOnPCF8574.elf makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S DHT11TWILCDBasedOnPCF8574.elf  >"DHT11TWILCDBasedOnPCF8574.lss"
	@echo 'Finished building: $@'
	@echo ' '

DHT11TWILCDBasedOnPCF8574.hex: DHT11TWILCDBasedOnPCF8574.elf makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex DHT11TWILCDBasedOnPCF8574.elf  "DHT11TWILCDBasedOnPCF8574.hex"
	@echo 'Finished building: $@'
	@echo ' '

DHT11TWILCDBasedOnPCF8574.eep: DHT11TWILCDBasedOnPCF8574.elf makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex DHT11TWILCDBasedOnPCF8574.elf  "DHT11TWILCDBasedOnPCF8574.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: DHT11TWILCDBasedOnPCF8574.elf makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega16 DHT11TWILCDBasedOnPCF8574.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) DHT11TWILCDBasedOnPCF8574.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents main-build

-include ../makefile.targets

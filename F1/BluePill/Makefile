TARGET = bluepill

# Choosing stlink device
CHOOSE_STLINK = 1
ONBOARD_STLINK_SERIAL = B55B5A1A00000000899EF301
OPENOCD_STLINK = 
FLASH_STLINK = 

ifeq ($(CHOOSE_STLINK), 1)
OPENOCD_STLINK 	= -c "adapter serial $(ONBOARD_STLINK_SERIAL)"
FLASH_STLINK 	= --serial $(ONBOARD_STLINK_SERIAL) 
endif


# debug build?
DEBUG = 1

# GDB config file path
GDB_CONFIG=.gdbinit
SVD_FILE=svd.py

# optimization
OPT = -Og


BUILD_DIR = build

# C sources

C_SOURCES = 	Drivers/GPIO/Example/blinky.c
C_SOURCES +=  	$(wildcard Core/Src/*.c) 
C_SOURCES +=  	$(wildcard Drivers/SPI/*.c) 
C_SOURCES +=  	$(wildcard Drivers/GPIO/*.c) 

# ASM sources
ASM_SOURCES = startup_stm32f103x6.s

# ASM sources
ASMM_SOURCES = 


PREFIX = arm-none-eabi-

ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
# cpu
CPU = -mcpu=cortex-m3

# fpu : NONE


# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS = -DSTM32F103xB


# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES =  \
-ICore/Inc \
-I../CMSIS/Device/ST/STM32F1xx/Include \
-I../CMSIS/Include \
-IDrivers/SPI \
-IDrivers/GPIO \


# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS += $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


# link script
LDSCRIPT = STM32F103C8Tx_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASMM_SOURCES:.S=.o)))
vpath %.S $(sort $(dir $(ASMM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@
$(BUILD_DIR)/%.o: %.S Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		


clean:
	rm -rf $(BUILD_DIR)

rebuild:
	make clean all
	figlet -f Isometric1 done | lolcat

flash:
	st-flash $(FLASH_STLINK) --reset write $(BUILD_DIR)/$(TARGET).bin 0x8000000
	figlet -f Isometric1 done | lolcat


burn: 
	make clean all flash


erase:
	st-flash --connect-under-reset erase
	figlet -f Isometric1 done | lolcat

gdb: 
	gdb-multiarch $(BUILD_DIR)/$(TARGET).elf \
	▎   -ex "tar ext :3333" \
	▎   -ex "source $(SVD_FILE)" \
		-ex "svd stm32f103.svd" \
	▎   -ex "dashboard svd load stm32f103.svd" \
	▎   -ex "dashboard -layout source svd variables" \
	▎   -ex "b main" \


openocd:
	openocd -f interface/stlink-v2.cfg $(OPENOCD_STLINK) -f target/stm32f1x.cfg


.PHONY: all clean flash burn

TARGET = theproject

# Choosing stlink device
CHOOSE_STLINK = 1
ONBOARD_STLINK_SERIAL = 066AFF484872664867231647
OPENOCD_STLINK = 
FLASH_STLINK = 

ifeq ($(CHOOSE_STLINK), 1)
OPENOCD_STLINK 	= -c "adapter serial $(ONBOARD_STLINK_SERIAL)"
FLASH_STLINK 	= --serial $(ONBOARD_STLINK_SERIAL) 
endif

DEBUG = 1
OPT = -Og

# GDB config file path
SVD_FILE=../../svd.py

# Build path
BUILD_DIR = build

# C sources
C_SOURCES = 	Drivers/I2C/Example/helloworld.c	
C_SOURCES += 	$(wildcard Core/Src/*.c)
C_SOURCES += 	$(wildcard Drivers/GPIO/Src/*.c)
C_SOURCES += 	$(wildcard Drivers/SPI/Src/*.c)
C_SOURCES += 	$(wildcard Drivers/I2C/Src/*.c)

# ASM sources
ASM_SOURCES = Startup/startup_stm32f411retx.s
ASMM_SOURCES = 


# binaries
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
 
# CFLAGS
# cpu
CPU = -mcpu=cortex-m4

# fpu
FPU = -mfpu=fpv4-sp-d16

# float-abi
FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  -DSTM32_THREAD_SAFE_STRATEGY=2


# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES =  \
-I../CMSIS/Device/ST/STM32F4xx/Include \
-I../CMSIS/Core/Include \
-ICore/Inc \
-IDrivers/SPI/Inc \
-IDrivers/GPIO/Inc \
-IDrivers/I2C/Inc

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS += $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"
	 
# link script
LDSCRIPT = STM32F411RETX_FLASH.ld

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

  
.PHONY: clean burn rebuild flash erase openocd

# dependencies
-include $(wildcard $(BUILD_DIR)/*.d)

clean:
	-rm -fR $(BUILD_DIR)

rebuild:
	make clean all
	figlet -f Isometric1 Done | lolcat


flash:
	st-flash $(FLASH_STLINK) --reset write $(BUILD_DIR)/$(TARGET).bin 0x8000000
	figlet -f Isometric1 done | lolcat

burn:
	make clean all flash

erase:
	st-flash &(FLASH_STLINK)  --connect-under-reset erase

gdb:
	gdb-multiarch $(BUILD_DIR)/$(TARGET).elf \
		-ex "tar ext :3333" \
		-ex "source $(SVD_FILE)" \
		-ex "dashboard svd load stm32f411.svd" \
		-ex "dashboard -layout source svd variables" \
		-ex "svd stm32f411.svd" \
		-ex "b main" \

openocd:
	openocd -f interface/stlink-v2.cfg $(OPENOCD_STLINK) -f target/stm32f4x.cfg

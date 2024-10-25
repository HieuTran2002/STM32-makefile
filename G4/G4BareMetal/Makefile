TARGET 		= final
CC 			= arm-none-eabi-gcc
CCP 		= arm-none-eabi-objcopy
SZ 			= arm-none-eabi-size
ARCH 		= -mcpu=cortex-m4
C_VERSION 	= gnu11
BUILD_DIR 	= build
LINKER 		= src/linker.ld 

C_INCLUDES 	= -Iinc

SRC			= $(wildcard src/*.c)
OBJ 		= $(patsubst src/%.c, $(BUILD_DIR)/%.o, $(SRC))


CFLAGS = -c -mthumb $(ARCH) -std=$(C_VERSION) $(C_INCLUDES) -Wall -O0 
LDFLAGS = -Os -nostdlib -Wl,--print-memory-usage -Wl,--gc-sections 

all: build/$(TARGET).elf build/$(TARGET).bin

$(BUILD_DIR)/%.o: src/%.c
	$(CC) $(CFLAGS) -o $@ $<

$(BUILD_DIR)/$(TARGET).elf: $(OBJ)
	$(CC) $(LDFLAGS) -T $(LINKER) $^ -o $@ -lc 

$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	$(CCP) -O binary $^ $@
	$(SZ) $(BUILD_DIR)/$(TARGET).elf

build:
	mkdir -p $(BUILD_DIR)

clean:
	rm build/*

ARMGNU ?= arm-none-eabi
BUILD = build/
SOURCE = ./
TARGET = kernel.img
LIST = kernel.list
MAP = kernel.map
LINKER = kernel.ld
CFLAGS = -Wall -fno-zero-initialized-in-bss -DCEU_NOSTDLIB

OBJECTS := $(patsubst $(SOURCE)%.s,$(BUILD)%.o,$(wildcard $(SOURCE)*.s))
OBJECTS += $(patsubst $(SOURCE)%.c,$(BUILD)%.o,$(wildcard $(SOURCE)*.c))

all: $(TARGET) $(LIST)
rebuild: all

$(LIST) : $(BUILD)output.elf
	$(ARMGNU)-objdump -d $(BUILD)output.elf > $(LIST)

$(TARGET) : $(BUILD)output.elf
	$(ARMGNU)-objcopy $(BUILD)output.elf -O binary $(TARGET) 

$(BUILD)output.elf : $(OBJECTS) $(LINKER)
	$(ARMGNU)-ld --no-undefined $(OBJECTS) -Map $(MAP) -o $(BUILD)output.elf -T $(LINKER)

# Rule to make the object files.
#$(BUILD)%.o: $(SOURCE)%.s
	#$(ARMGNU)-as -I $(SOURCE) $< -o $@

$(BUILD)%.o: $(SOURCE)%.* ceu
	mkdir -p $(BUILD)
	$(ARMGNU)-gcc $(CFLAGS) -c -I $(SOURCE) $< -o $@

ceu:
	ceu ex-06.ceu --out-c _ceu_app.src

clean :
	-rm -rf $(BUILD)
	-rm -f $(TARGET)
	-rm -f $(LIST)
	-rm -f $(MAP)

sdcard:
	sudo mount /dev/sdb1 /mnt
	sudo cp kernel.img /mnt/kernel.img
	sudo umount /dev/sdb1

xmodem:
	sx -vv kernel.img < /dev/ttyUSB0 > /dev/ttyUSB0

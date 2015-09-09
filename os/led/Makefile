ARMGNU = arm-none-eabi
SRECCAT = srec_cat
CEUDIR = /data/ceu/ceu-rpi/os

# Ceu and C
FLAGS = -I $(CEUDIR)

CFLAGS = $(FLAGS)
CFLAGS += -Wall #-O2
CFLAGS += -I $(CEUDIR)
CFLAGS += -nostdlib -nostartfiles -ffreestanding
CFLAGS += -nostdinc -isystem ../musl/include
CFLAGS += -fno-zero-initialized-in-bss
#CFLAGS += -fpic -mpic-data-is-text-relative
#CFLAGS += -fwhole-program #-flto
#LDFLAGS += -T $(CEUDIR)/kernel.ld #-flto

KFLAGS = -DCEU_OS_KERNEL $(CEUOSLUAIFC) $(CFLAGS) -include $(CEUDIR)/main.h

AFLAGS  = $(CFLAGS) -DCEU_OS_APP
AFLAGS += -fpic #-mpic-data-is-text-relative
AFLAGS += -fno-zero-initialized-in-bss
LDFLAGS_APP = -Wl,-T../app.ld

all: led
	$(ARMGNU)-objcopy _ceu_led.elf --change-addresses=0x110000 -O ihex _ceu_led.hex
	###
	$(ARMGNU)-as $(CEUDIR)/vectors.s                -o vectors.o
	$(ARMGNU)-gcc $(KFLAGS) -c $(CEUDIR)/stdlib.c   -o stdlib.o
	$(ARMGNU)-gcc $(KFLAGS) -c $(CEUDIR)/ceu_log.c  -o ceu_log.o
	$(ARMGNU)-gcc $(KFLAGS) -c $(CEUDIR)/ceu_pool.c -o ceu_pool.o
	$(ARMGNU)-gcc $(KFLAGS) -c $(CEUDIR)/ceu_os.c   -o ceu_os.o
	$(ARMGNU)-gcc $(KFLAGS) -c tst.c                -o app.o
	$(ARMGNU)-gcc $(KFLAGS) -c $(CEUDIR)/main.c     -o main.o
	$(ARMGNU)-ld vectors.o stdlib.o ceu_log.o ceu_pool.o ceu_os.o app.o main.o \
					-T $(CEUDIR)/kernel.ld -o kernel.elf
	! $(ARMGNU)-objdump -h kernel.elf | grep ".got"
	! $(ARMGNU)-objdump -h kernel.elf | grep ".bss"
	$(ARMGNU)-objdump -S -D kernel.elf > kernel.list
	$(ARMGNU)-size kernel.elf
	$(ARMGNU)-objcopy kernel.elf -O ihex kernel.hex
	#cp all.bin kernel.img
	###
	$(SRECCAT) kernel.hex   -intel -offset -0x8000 \
			   _ceu_led.hex -intel -offset -0x8000 \
					-o all.hex -intel
	$(SRECCAT) all.hex -intel -o kernel.img -Binary

led:
	ceu led.ceu --os $(ceuosluaifc) --verbose \
				--out-c _ceu_led.c \
				--cpp-exe "arm-none-eabi-cpp" \
				--cpp-args "$(CFLAGS)"
	$(ARMGNU)-gcc $(AFLAGS) $(LDFLAGS_APP) _ceu_led.c -o _ceu_led.elf
	! $(ARMGNU)-objdump -h _ceu_led.elf | grep ".got"
	! $(ARMGNU)-objdump -h _ceu_led.elf | grep ".bss"
	$(ARMGNU)-objcopy _ceu_led.elf -O binary led.app

sdcard:
	sudo mount /dev/sdb1 /mnt
	sudo cp kernel.img /mnt/kernel.img
	sudo umount /mnt

clean:
	find . -name "*.exe"  | xargs rm -f
	find . -name "_ceu_*" | xargs rm -f
	find . -name "*.hex"  | xargs rm -f
	find . -name "*.o"    | xargs rm -f
	find . -name "*.elf"  | xargs rm -f
	find . -name "*.app"  | xargs rm -f
	find . -name "*.list" | xargs rm -f

ARMGNU = arm-none-eabi
SRECCAT = srec_cat
CEUDIR = /data/ceu/ceu-rpi/os

CFLAGS = -Wall #-O2
CFLAGS += -DCEU_DEBUG -DCEU_OS -I $(CEUDIR)
CFLAGS += -nostdlib -nostartfiles -ffreestanding
CFLAGS += -fno-zero-initialized-in-bss
#CFLAGS += -fpic -mpic-data-is-text-relative
#CFLAGS += -fwhole-program #-flto
#LDFLAGS += -T $(CEUDIR)/kernel.ld #-flto

CFLAGS_ONE = -fpic #-mpic-data-is-text-relative
CFLAGS_ONE += -fno-zero-initialized-in-bss

LDFLAGS_ONE = -Wl,-T../one.ld

CFLAGS_FAT = -pedantic -pedantic-errors \
		 -nostdlib -nostartfiles -ffreestanding \
		 -Wall -Wextra -Wshadow \
		 -std=gnu99 \
		 -I. -DBUILDING_RPIBOOT \
		 -include config.h \
		$(CFLAGS_ONE)

tst1: lua kernel
	ceu tst1.ceu --os --verbose \
				--out-c _ceu_tst1.c \
				--cpp-exe "arm-none-eabi-cpp" \
				--cpp-args "$(CFLAGS)"
	#cp /tmp/_ceu_tst1.c .
	$(ARMGNU)-gcc -I. $(CFLAGS) $(CFLAGS_ONE) $(LDFLAGS_ONE) \
		../debug.c _ceu_tst1.c -o _ceu_tst1.elf
	! $(ARMGNU)-objdump -h _ceu_tst1.elf | grep ".got"
	! $(ARMGNU)-objdump -h _ceu_tst1.elf | grep ".bss"
	$(ARMGNU)-objcopy _ceu_tst1.elf -O binary tst1.app
	$(ARMGNU)-objcopy _ceu_tst1.elf --change-addresses=0x300000 -O ihex _ceu_tst1.hex
	###
	$(SRECCAT) kernel.hex    -intel -offset -0x8000 \
			   _ceu_lua.hex  -intel -offset -0x8000 \
			   _ceu_tst1.hex -intel -offset -0x8000 \
					-o all.hex -intel
	$(SRECCAT) all.hex -intel -o kernel.img -Binary

lua:
	cd lua-5.1.5/ && make generic && cd -
	ceu lua.ceu --os --verbose \
				--out-c _ceu_lua.c \
				--cpp-exe "arm-none-eabi-cpp" \
				--cpp-args "$(CFLAGS)"
	$(ARMGNU)-gcc $(CFLAGS) $(CFLAGS_ONE) $(LDFLAGS_ONE) \
		-I lua-5.1.5/src/ \
		other.c _ceu_lua.c -o _ceu_lua.elf -L lua-5.1.5/src -llua -lm -lc -lgcc
	###
	#! $(ARMGNU)-objdump -h _ceu_lua.elf | grep ".got"
	#! $(ARMGNU)-objdump -h _ceu_lua.elf | grep ".bss"
	$(ARMGNU)-objcopy _ceu_lua.elf -O binary lua.app
	$(ARMGNU)-objcopy _ceu_lua.elf --change-addresses=0x110000 -O ihex _ceu_lua.hex

kernel:
	$(ARMGNU)-as $(CEUDIR)/vectors.s                -o vectors.o
	$(ARMGNU)-gcc $(CFLAGS) -c $(CEUDIR)/stdlib.c   -o stdlib.o
	$(ARMGNU)-gcc $(CFLAGS) -c $(CEUDIR)/ceu_pool.c -o ceu_pool.o
	$(ARMGNU)-gcc $(CFLAGS) -c $(CEUDIR)/debug.c    -o debug.o
	$(ARMGNU)-gcc $(CFLAGS) -c $(CEUDIR)/ceu_os.c   -o ceu_os.o
	$(ARMGNU)-gcc $(CFLAGS) -c tst1.c               -o app.o
	$(ARMGNU)-gcc $(CFLAGS) -c $(CEUDIR)/main.c     -o main.o
	$(ARMGNU)-ld vectors.o stdlib.o ceu_pool.o debug.o ceu_os.o app.o main.o \
					-T $(CEUDIR)/kernel.ld -o kernel.elf
	###
	! $(ARMGNU)-objdump -h kernel.elf | grep ".got"
	! $(ARMGNU)-objdump -h kernel.elf | grep ".bss"
	$(ARMGNU)-objdump -S -D kernel.elf > kernel.list
	$(ARMGNU)-size kernel.elf
	$(ARMGNU)-objcopy kernel.elf -O ihex kernel.hex

sdcard:
	sudo mount /dev/sdb1 /mnt
	sudo cp kernel.img /mnt/kernel.img
	touch led.app uart.app uart_tst.app
	sudo cp *.app /mnt/
	sudo umount /mnt

xmodem:
	stty -F /dev/ttyUSB0 115200
	sx -vv kernel.img < /dev/ttyUSB0 > /dev/ttyUSB0

clean:
	find . -name "*.exe"  | xargs rm -f
	find . -name "_ceu_*" | xargs rm -f
	find . -name "*.hex"  | xargs rm -f
	find . -name "*.o"    | xargs rm -f
	find . -name "*.elf"  | xargs rm -f
	find . -name "*.app"  | xargs rm -f
	find . -name "*.list" | xargs rm -f
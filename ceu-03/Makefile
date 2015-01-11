
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding -DCEU_NOSTDLIB

gcc : ex-03.bin

all : gcc clang

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img
	rm -f *.bc
	rm -f *.clang.s


vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

ex-03.o : ex-03.c ceu
	$(ARMGNU)-gcc $(COPS) -c ex-03.c -o ex-03.o

ceu:
	ceu ex-03.ceu

ex-03.bin : loader vectors.o ex-03.o
	$(ARMGNU)-ld vectors.o ex-03.o -T loader -o ex-03.elf
	$(ARMGNU)-objdump -D ex-03.elf > ex-03.list
	$(ARMGNU)-objcopy ex-03.elf -O ihex ex-03.hex
	$(ARMGNU)-objcopy ex-03.elf -O binary ex-03.bin

sdcard:
	sudo mount /dev/sdb1 /mnt
	sudo cp ex-03.bin /mnt/kernel.img
	sudo umount /dev/sdb1

xmodem:
	sx -vv ex-03.bin < /dev/ttyUSB0 > /dev/ttyUSB0

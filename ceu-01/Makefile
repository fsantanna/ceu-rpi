
ARMGNU ?= arm-none-eabi

AOPS = --warn --fatal-warnings
COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding -DCEU_NOSTDLIB



gcc : ex-01.hex ex-01.bin

all : gcc clang

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img
	rm -f *.bc
	rm -f *.clang.opt.s

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

ex-01.o : ex-01.c ceu
	$(ARMGNU)-gcc $(COPS) -c ex-01.c -o ex-01.o

ceu:
	ceu ex-01.ceu

ex-01.elf : memmap vectors.o ex-01.o
	$(ARMGNU)-ld vectors.o ex-01.o -T memmap -o ex-01.elf
	$(ARMGNU)-objdump -D ex-01.elf > ex-01.list

ex-01.bin : ex-01.elf
	$(ARMGNU)-objcopy ex-01.elf -O binary ex-01.bin

ex-01.hex : ex-01.elf
	$(ARMGNU)-objcopy ex-01.elf -O ihex ex-01.hex

sdcard:
	sudo mount /dev/sdb1 /mnt
	sudo cp ex-01.bin /mnt/kernel.img
	sudo umount /dev/sdb1

xmodem:
	sx -vv ex-01.bin < /dev/ttyUSB0 > /dev/ttyUSB0

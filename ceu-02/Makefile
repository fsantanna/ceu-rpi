
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

gcc : ex-02.hex ex-02.bin

all : gcc 

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

ex-02.o : ex-02.c ceu
	$(ARMGNU)-gcc $(COPS) -c ex-02.c -o ex-02.o

ceu:
	ceu ex-02.ceu

ex-02.elf : memmap vectors.o ex-02.o
	$(ARMGNU)-ld vectors.o ex-02.o -T memmap -o ex-02.elf
	$(ARMGNU)-objdump -D ex-02.elf > ex-02.list

ex-02.bin : ex-02.elf
	$(ARMGNU)-objcopy ex-02.elf -O binary ex-02.bin

ex-02.hex : ex-02.elf
	$(ARMGNU)-objcopy ex-02.elf -O ihex ex-02.hex

sdcard:
	sudo mount /dev/sdb1 /mnt
	sudo cp ex-02.bin /mnt/kernel.img
	sudo umount /dev/sdb1

xmodem:
	sx -vv ex-02.bin < /dev/ttyUSB0 > /dev/ttyUSB0

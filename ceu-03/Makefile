
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

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






LOPS = -Wall -m32 -emit-llvm
LLCOPS0 = -march=arm 
LLCOPS1 = -march=arm -mcpu=arm1176jzf-s
LLCOPS = $(LLCOPS1)
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

clang : ex-03.bin

ex-03.bc : ex-03.c
	clang $(LOPS) -c ex-03.c -o ex-03.bc

ex-03.clang.elf : loader vectors.o ex-03.bc
	llvm-link ex-03.bc -o ex-03.nopt.bc
	opt $(OOPS) ex-03.nopt.bc -o ex-03.opt.bc
	#llc $(LLCOPS) ex-03.opt.bc -o ex-03.clang.s
	#$(ARMGNU)-as ex-03.clang.s -o ex-03.clang.o
	llc $(LLCOPS) -filetype=obj ex-03.opt.bc -o ex-03.clang.o
	$(ARMGNU)-ld -o ex-03.clang.elf -T loader vectors.o ex-03.clang.o
	$(ARMGNU)-objdump -D ex-03.clang.elf > ex-03.clang.list

#ex-03.bin : ex-03.clang.elf
	#$(ARMGNU)-objcopy ex-03.clang.elf ex-03.clang.bin -O binary

sdcard:
	sudo mount /dev/sdb1 /mnt
	sudo cp ex-03.bin /mnt/kernel.img
	sudo umount /dev/sdb1

xmodem:
	sx -vv ex-03.bin < /dev/ttyUSB0 > /dev/ttyUSB0

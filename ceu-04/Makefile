
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

gcc : ex-04.bin

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

memutils.o : memutils.c
	$(ARMGNU)-gcc $(COPS) -c memutils.c -o memutils.o

keyboard.o : keyboard.c
	$(ARMGNU)-gcc $(COPS) -c keyboard.c -o keyboard.o

ex-04.o : ex-04.c image_smile.h ceu
	$(ARMGNU)-gcc $(COPS) -c ex-04.c -o ex-04.o

ceu:
	ceu ex-04.ceu

ex-04.bin : loader vectors.o ex-04.o keyboard.o memutils.o
	$(ARMGNU)-ld vectors.o ex-04.o memutils.o keyboard.o libcsud.a -T loader -o ex-04.elf
	$(ARMGNU)-objdump -D ex-04.elf > ex-04.list
	$(ARMGNU)-objcopy ex-04.elf -O ihex ex-04.hex
	$(ARMGNU)-objcopy ex-04.elf -O binary ex-04.bin






LOPS = -Wall -m32 -emit-llvm
LLCOPS0 = -march=arm 
LLCOPS1 = -march=arm -mcpu=arm1176jzf-s
LLCOPS = $(LLCOPS1)
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

clang : ex-04.bin

ex-04.bc : ex-04.c image_smile.h
	clang $(LOPS) -c ex-04.c -o ex-04.bc

ex-04.clang.elf : loader vectors.o ex-04.bc
	llvm-link ex-04.bc -o ex-04.nopt.bc
	opt $(OOPS) ex-04.nopt.bc -o ex-04.opt.bc
	#llc $(LLCOPS) ex-04.opt.bc -o ex-04.clang.s
	#$(ARMGNU)-as ex-04.clang.s -o ex-04.clang.o
	llc $(LLCOPS) -filetype=obj ex-04.opt.bc -o ex-04.clang.o
	$(ARMGNU)-ld -o ex-04.clang.elf -T loader vectors.o ex-04.clang.o
	$(ARMGNU)-objdump -D ex-04.clang.elf > ex-04.clang.list

#ex-04.bin : ex-04.clang.elf
	#$(ARMGNU)-objcopy ex-04.clang.elf ex-04.clang.bin -O binary

sdcard:
	sudo mount /dev/sdb1 /mnt
	sudo cp ex-04.bin /mnt/kernel.img
	sudo umount /dev/sdb1

xmodem:
	sx -vv ex-04.bin < /dev/ttyUSB0 > /dev/ttyUSB0

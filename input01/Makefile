
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

gcc : kernel.img

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

input01.o : input01.c
	$(ARMGNU)-gcc $(COPS) -c input01.c -o input01.o

memutils.o : memutils.c
	$(ARMGNU)-gcc $(COPS) -c memutils.c -o memutils.o

keyboard.o : keyboard.c
	$(ARMGNU)-gcc $(COPS) -c keyboard.c -o keyboard.o

kernel.img : loader vectors.o memutils.o keyboard.o input01.o
	$(ARMGNU)-ld vectors.o memutils.o keyboard.o input01.o libcsud.a -T loader -o input01.elf
	$(ARMGNU)-objdump -D input01.elf > input01.list
	$(ARMGNU)-objcopy input01.elf -O ihex input01.hex
	$(ARMGNU)-objcopy input01.elf -O binary kernel.img






LOPS = -Wall -m32 -emit-llvm
LLCOPS0 = -march=arm 
LLCOPS1 = -march=arm -mcpu=arm1176jzf-s
LLCOPS = $(LLCOPS1)
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

clang : input01.bin

input01.bc : input01.c
	clang $(LOPS) -c input01.c -o input01.bc

memutils.bc : memutils.c
	clang $(LOPS) -c memutils.c -o memutils.bc

input01.clang.elf : loader vectors.o input01.bc memutils.bc
	llvm-link memutils.bc input01.bc -o input01.nopt.bc
	opt $(OOPS) input01.nopt.bc -o input01.opt.bc
	#llc $(LLCOPS) input01.opt.bc -o input01.clang.s
	#$(ARMGNU)-as input01.clang.s -o input01.clang.o
	llc $(LLCOPS) -filetype=obj input01.opt.bc -o input01.clang.o
	$(ARMGNU)-ld -o input01.clang.elf -T loader vectors.o input01.clang.o
	$(ARMGNU)-objdump -D input01.clang.elf > input01.clang.list

input01.bin : input01.clang.elf
	$(ARMGNU)-objcopy input01.clang.elf input01.clang.bin -O binary



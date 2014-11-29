
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

video02.o : video02.c
	$(ARMGNU)-gcc $(COPS) -c video02.c -o video02.o

memutils.o : memutils.c
	$(ARMGNU)-gcc $(COPS) -c memutils.c -o memutils.o

textutils.o : textutils.c
	$(ARMGNU)-gcc $(COPS) -c textutils.c -o textutils.o

kernel.img : loader vectors.o memutils.o textutils.o video02.o
	$(ARMGNU)-ld vectors.o memutils.o textutils.o video02.o -T loader -o video02.elf
	$(ARMGNU)-objdump -D video02.elf > video02.list
	$(ARMGNU)-objcopy video02.elf -O ihex video02.hex
	$(ARMGNU)-objcopy video02.elf -O binary kernel.img






LOPS = -Wall -m32 -emit-llvm
LLCOPS0 = -march=arm 
LLCOPS1 = -march=arm -mcpu=arm1176jzf-s
LLCOPS = $(LLCOPS1)
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

clang : video02.bin

video02.bc : video02.c image_data.h
	clang $(LOPS) -c video02.c -o video02.bc

memutils.bc : memutils.c
	clang $(LOPS) -c memutils.c -o memutils.bc

video02.clang.elf : loader vectors.o video02.bc memutils.bc
	llvm-link memutils.bc video02.bc -o video02.nopt.bc
	opt $(OOPS) video02.nopt.bc -o video02.opt.bc
	#llc $(LLCOPS) video02.opt.bc -o video02.clang.s
	#$(ARMGNU)-as video02.clang.s -o video02.clang.o
	llc $(LLCOPS) -filetype=obj video02.opt.bc -o video02.clang.o
	$(ARMGNU)-ld -o video02.clang.elf -T loader vectors.o video02.clang.o
	$(ARMGNU)-objdump -D video02.clang.elf > video02.clang.list

video02.bin : video02.clang.elf
	$(ARMGNU)-objcopy video02.clang.elf video02.clang.bin -O binary



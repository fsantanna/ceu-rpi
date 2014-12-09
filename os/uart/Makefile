ARMGNU = arm-none-eabi
SRECCAT = srec_cat
CEUDIR = /data/ceu/ceu-rpi/os

CFLAGS = -Wall #-O2
CFLAGS += -DCEU_OS -I $(CEUDIR)
CFLAGS += -nostdlib -nostartfiles -ffreestanding
CFLAGS += -fno-zero-initialized-in-bss
#CFLAGS += -fpic -mpic-data-is-text-relative
#CFLAGS += -fwhole-program #-flto
#LDFLAGS += -T $(CEUDIR)/kernel.ld #-flto

CFLAGS_ONE = -fpic -mpic-data-is-text-relative
LDFLAGS_ONE = -Wl,-T../one.ld

all:
	ceu uart.ceu --os --verbose \
				--out-c _ceu_uart.c \
				--cpp-exe "arm-none-eabi-cpp" \
				--cpp-args "$(CFLAGS)"
	$(ARMGNU)-gcc $(CFLAGS) $(CFLAGS_ONE) $(LDFLAGS_ONE) \
		_ceu_uart.c -o _ceu_uart.elf
	! $(ARMGNU)-objdump -h _ceu_uart.elf | grep ".got"
	! $(ARMGNU)-objdump -h _ceu_uart.elf | grep ".bss"
	$(ARMGNU)-objcopy _ceu_uart.elf -O binary _ceu_uart.bin
	$(ARMGNU)-objcopy _ceu_uart.elf --change-addresses=0x94000 -O ihex _ceu_uart.hex
	###
	ceu tst.ceu --os --verbose \
				--out-c _ceu_tst.c \
				--cpp-exe "arm-none-eabi-cpp" \
				--cpp-args "$(CFLAGS)"
	#cp /tmp/_ceu_tst.c .
	$(ARMGNU)-gcc $(CFLAGS) $(CFLAGS_ONE) $(LDFLAGS_ONE) \
		_ceu_tst.c -o _ceu_tst.elf
	! $(ARMGNU)-objdump -h _ceu_tst.elf | grep ".got"
	! $(ARMGNU)-objdump -h _ceu_tst.elf | grep ".bss"
	$(ARMGNU)-objcopy _ceu_tst.elf -O binary _ceu_tst.bin
	$(ARMGNU)-objcopy _ceu_tst.elf --change-addresses=0x96000 -O ihex _ceu_tst.hex
	###
	$(ARMGNU)-as $(CEUDIR)/vectors.s                -o vectors.o
	$(ARMGNU)-gcc $(CFLAGS) -c $(CEUDIR)/stdlib.c   -o stdlib.o
	$(ARMGNU)-gcc $(CFLAGS) -c $(CEUDIR)/ceu_pool.c -o ceu_pool.o
	$(ARMGNU)-gcc $(CFLAGS) -c $(CEUDIR)/ceu_os.c   -o ceu_os.o
	$(ARMGNU)-gcc $(CFLAGS) -c tst.c                -o app.o
	$(ARMGNU)-gcc $(CFLAGS) -c $(CEUDIR)/main.c     -o main.o
	$(ARMGNU)-ld vectors.o stdlib.o ceu_pool.o ceu_os.o app.o main.o \
					-T $(CEUDIR)/kernel.ld -o kernel.elf
	#$(ARMGNU)-gcc -g $(CFLAGS)
				  #$(CEUDIR)/ceu_pool.c $(CEUDIR)/ceu_os.c $(CEUDIR)/vectors.s
				  #$(CEUDIR)/main.c $(CEUDIR)/stdlib.c
				  #tst.c _ceu_tst.c
				  #-o all.o
	#$(ARMGNU)-objdump -S -D kernel.o > /tmp/kernel.list
	#$(ARMGNU)-ld $(LDFLAGS) kernel.o -o kernel.elf
	! $(ARMGNU)-objdump -h kernel.elf | grep ".got"
	! $(ARMGNU)-objdump -h kernel.elf | grep ".bss"
	$(ARMGNU)-objdump -S -D kernel.elf > kernel.list
	$(ARMGNU)-size kernel.elf
	$(ARMGNU)-objcopy kernel.elf -O ihex kernel.hex
	#cp all.bin kernel.img
	###
	$(SRECCAT) kernel.hex    -intel -offset -0x8000 \
			   _ceu_uart.hex -intel -offset -0x8000 \
			   _ceu_tst.hex  -intel -offset -0x8000 \
					-o all.hex -intel
	$(SRECCAT) all.hex -intel -o kernel.img -Binary

clean:
	find . -name "*.exe"  | xargs rm -f
	find . -name "_ceu_*" | xargs rm -f
	find . -name "*.hex"  | xargs rm -f
	find . -name "*.o"    | xargs rm -f
	find . -name "*.elf"  | xargs rm -f
	find . -name "*.bin"  | xargs rm -f
	find . -name "*.list" | xargs rm -f
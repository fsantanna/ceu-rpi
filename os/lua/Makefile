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
CFLAGS += -I $(CEUDIR)/lua/lua-5.1.5/src
#CFLAGS += -fpic -mpic-data-is-text-relative
#CFLAGS += -fwhole-program #-flto
#LDFLAGS += -T $(CEUDIR)/kernel.ld #-flto

KFLAGS = -DCEU_OS_KERNEL $(CEUOSLUAIFC) $(CFLAGS) -include $(CEUDIR)/main.h

AFLAGS  = $(CFLAGS) -DCEU_OS_APP
AFLAGS += -fpic #-mpic-data-is-text-relative
AFLAGS += -fno-zero-initialized-in-bss
LDFLAGS_APP = -Wl,-T../app.ld

tst1: APP=tst1.c
tst1: lua kernel
	ceu tst1.ceu --os --verbose \
				--out-c _ceu_tst1.c \
				--cpp-exe "arm-none-eabi-cpp" \
				--cpp-args "$(CFLAGS)"
	#cp /tmp/_ceu_tst1.c .
	$(ARMGNU)-gcc -I. $(AFLAGS) $(LDFLAGS_APP) \
		-I lua-5.1.5/src/ \
		_ceu_tst1.c -o _ceu_tst1.elf
	! $(ARMGNU)-objdump -h _ceu_tst1.elf | grep ".bss"
	! $(ARMGNU)-objdump -h _ceu_tst1.elf | grep ".rel"
	! $(ARMGNU)-objdump -h _ceu_tst1.elf | grep ".got"
	$(ARMGNU)-objcopy _ceu_tst1.elf -O binary tst1.app
	$(ARMGNU)-objcopy _ceu_tst1.elf --change-addresses=0x150000 -O ihex _ceu_tst1.hex
	###
	$(SRECCAT) kernel.hex    -intel -offset -0x8000 \
			   _ceu_lua.hex  -intel -offset -0x8000 \
			   _ceu_tst1.hex -intel -offset -0x8000 \
					-o all.hex -intel
	$(SRECCAT) all.hex -intel -o kernel.img -Binary

main:
	cd lua-5.1.5/ && make generic && cd -
	$(ARMGNU)-as vectors.s -o vectors.o
	$(ARMGNU)-gcc $(CFLAGS) -c $(CEUDIR)/ceu_log.c  -o ceu_log.o
	$(ARMGNU)-gcc $(CFLAGS) -Ilua-5.1.5/src/ -c main.c -o main.o
	$(ARMGNU)-ld vectors.o ceu_log.o main.o \
		--no-export-dynamic \
		-T $(CEUDIR)/kernel.ld -o kernel.elf \
		-L lua-5.1.5/src -llua \
		-L ../musl/lib -lc \
		-L /usr/lib/gcc/arm-none-eabi/4.8.2 -lgcc
		#-L ../musl/lib -lc
		#-L /usr/lib/arm-none-eabi/newlib.old -lc -lm
	###
	! $(ARMGNU)-objdump -h kernel.elf | grep ".bss"
	! $(ARMGNU)-objdump -h kernel.elf | grep ".rel"
	# TODO: uncomment below
	#! $(ARMGNU)-objdump -h kernel.elf | grep ".got"
	$(ARMGNU)-objdump -S -D kernel.elf > kernel.list
	$(ARMGNU)-size kernel.elf
	$(ARMGNU)-objcopy kernel.elf -O binary kernel.img


lua:
	cd lua-5.1.5/ && make generic && cd -
	ceu lua.ceu --os $(ceuosluaifc) --verbose \
				--out-c _ceu_lua.c \
				--cpp-exe "arm-none-eabi-cpp" \
				--cpp-args "$(CFLAGS)"
	#cp /tmp/_ceu_lua.c .
	$(ARMGNU)-gcc $(AFLAGS) $(LDFLAGS_APP) \
		-I lua-5.1.5/src/ \
		_ceu_lua.c -o _ceu_lua.elf \
		-L lua-5.1.5/src -llua \
		-L ../musl/lib -lc \
		-lgcc
	###
	! $(ARMGNU)-objdump -h _ceu_lua.elf | grep ".bss"
	! $(ARMGNU)-objdump -h _ceu_lua.elf | grep ".rel"
	# TODO: uncomment below
	#! $(ARMGNU)-objdump -h _ceu_lua.elf | grep ".got"
	! strings -a _ceu_lua.elf | grep libc | grep newlib
	$(ARMGNU)-objcopy _ceu_lua.elf -O binary lua.app
	$(ARMGNU)-objcopy _ceu_lua.elf --change-addresses=0x110000 -O ihex _ceu_lua.hex

kernel:
	$(ARMGNU)-as $(CEUDIR)/vectors.s                -o vectors.o
	$(ARMGNU)-gcc $(KFLAGS) -c $(CEUDIR)/stdlib.c   -o stdlib.o
	$(ARMGNU)-gcc $(KFLAGS) -c $(CEUDIR)/ceu_pool.c -o ceu_pool.o
	$(ARMGNU)-gcc $(KFLAGS) -c $(CEUDIR)/ceu_log.c  -o ceu_log.o
	$(ARMGNU)-gcc $(KFLAGS) -c $(CEUDIR)/ceu_os.c   -o ceu_os.o
	$(ARMGNU)-gcc $(KFLAGS) -c $(APP)               -o app.o
	$(ARMGNU)-gcc $(KFLAGS) -c $(CEUDIR)/main.c     -o main.o
	$(ARMGNU)-ld vectors.o stdlib.o ceu_pool.o ceu_log.o ceu_os.o app.o main.o \
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

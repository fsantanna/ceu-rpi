ARMGNU = arm-none-eabi
SRECCAT = srec_cat
CEUDIR = /data/ceu/ceu-rpi/os

# Ceu and C
FLAGS = -I $(CEUDIR)

CFLAGS = $(FLAGS)
CFLAGS += -Wall -O2
CFLAGS += -I $(CEUDIR)
CFLAGS += -nostdlib -nostartfiles -ffreestanding
CFLAGS += -fno-zero-initialized-in-bss
#CFLAGS += -fpic -mpic-data-is-text-relative
#CFLAGS += -fwhole-program #-flto
#LDFLAGS += -T $(CEUDIR)/kernel.ld #-flto

KFLAGS = -DCEU_OS_KERNEL $(CFLAGS) -include $(CEUDIR)/main.h

CFLAGS_ONE = -fpic #-mpic-data-is-text-relative
LDFLAGS_ONE = -Wl,-T../app.ld
#LDFLAGS_ONE = -Wl,--section-start=.export=0x00
# -Wl,--section-start=.text=0x10 -Wl,--section-start=.data=0x200
#LDFLAGS_ONE = -Wl,-T../kernel.ld -Wl,--image-base

all:
	$(ARMGNU)-gcc $(CFLAGS) $(CFLAGS_ONE) $(LDFLAGS_ONE) ceu_tst.c -o ceu_tst.elf
	! $(ARMGNU)-objdump -h ceu_tst.elf | grep ".got"
	! $(ARMGNU)-objdump -h ceu_tst.elf | grep ".bss"
	$(ARMGNU)-objcopy ceu_tst.elf -O binary tst.app
	$(ARMGNU)-objcopy ceu_tst.elf --change-addresses=0x110000 -O ihex ceu_tst.hex
	###
	$(ARMGNU)-as $(CEUDIR)/vectors.s                -o vectors.o
	$(ARMGNU)-gcc $(KFLAGS) -c $(CEUDIR)/stdlib.c   -o stdlib.o
	$(ARMGNU)-gcc $(KFLAGS) -c $(CEUDIR)/ceu_log.c  -o ceu_log.o
	$(ARMGNU)-gcc $(KFLAGS) -c $(CEUDIR)/ceu_pool.c -o ceu_pool.o
	$(ARMGNU)-gcc $(KFLAGS) -c $(CEUDIR)/ceu_os.c   -o ceu_os.o
	$(ARMGNU)-gcc $(KFLAGS) -c tst.c                -o app.o
	$(ARMGNU)-gcc $(KFLAGS) -c $(CEUDIR)/main.c     -o main.o
	$(ARMGNU)-ld vectors.o stdlib.o ceu_log.o ceu_pool.o ceu_os.o app.o main.o \
					-T $(CEUDIR)/kernel.ld -o kernel.elf
	#$(ARMGNU)-gcc -g $(CFLAGS)
				  #$(CEUDIR)/ceu_pool.c $(CEUDIR)/ceu_os.c $(CEUDIR)/vectors.s
				  #$(CEUDIR)/main.c $(CEUDIR)/stdlib.c
				  #tst.c _ceu_tst.c
				  #-o kernel.o
	#$(ARMGNU)-objdump -S -D kernel.o > /tmp/kernel.list
	#$(ARMGNU)-ld $(LDFLAGS) kernel.o -o kernel.elf
	#TODO
	! $(ARMGNU)-objdump -h kernel.elf | grep ".got"
	! $(ARMGNU)-objdump -h kernel.elf | grep ".bss"
	$(ARMGNU)-objdump -S -D kernel.elf > kernel.list
	$(ARMGNU)-size kernel.elf
	$(ARMGNU)-objcopy kernel.elf -O ihex kernel.hex
	#cp all.bin kernel.img
	###
	$(SRECCAT) kernel.hex -intel -offset -0x8000 ceu_tst.hex -intel -offset -0x8000 -o all.hex -intel
	$(SRECCAT) all.hex -intel -o kernel.img -Binary

sdcard:
	sudo mount /dev/sdb1 /mnt
	sudo cp kernel.img /mnt/kernel.img
	sudo umount /mnt

clean:
	find . -name "*.exe"  | xargs rm -f
	find . -name "_ceu_*" | xargs rm -f
	find . -name "*.hex"  | xargs rm -f
	find . -name "*.o"    | xargs rm -f
	find . -name "*.elf"  | xargs rm -f
	find . -name "*.app"  | xargs rm -f
	find . -name "*.list" | xargs rm -f

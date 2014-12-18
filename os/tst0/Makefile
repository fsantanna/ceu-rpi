ARMGNU = arm-none-eabi
CEUDIR = /data/ceu/ceu-rpi/os

CFLAGS = -Wall -O2
CFLAGS += -DCEU_OS -I $(CEUDIR)
CFLAGS += -nostdlib -nostartfiles -ffreestanding
CFLAGS += -fno-zero-initialized-in-bss
#CFLAGS += -fpic -mpic-data-is-text-relative
#CFLAGS += -fwhole-program #-flto
#LDFLAGS += -T $(CEUDIR)/kernel.ld #-flto

all:
	ceu tst.ceu --os --verbose \
				--out-c _ceu_tst.c \
				--cpp-exe "arm-none-eabi-cpp" \
				--cpp-args "$(CFLAGS)"
	#cp /tmp/_ceu_tst.c .
	$(ARMGNU)-as $(CEUDIR)/vectors.s                -o vectors.o
	$(ARMGNU)-gcc $(CFLAGS) -c $(CEUDIR)/stdlib.c   -o stdlib.o
	$(ARMGNU)-gcc $(CFLAGS) -c $(CEUDIR)/ceu_pool.c -o ceu_pool.o
	$(ARMGNU)-gcc $(CFLAGS) -c $(CEUDIR)/ceu_os.c   -o ceu_os.o
	$(ARMGNU)-gcc $(CFLAGS) -c tst.c                -o app.o
	$(ARMGNU)-gcc $(CFLAGS) -c $(CEUDIR)/main.c     -o main.o
	$(ARMGNU)-gcc $(CFLAGS) -c _ceu_tst.c           -o _ceu_tst.o
	$(ARMGNU)-ld vectors.o stdlib.o ceu_pool.o ceu_os.o app.o main.o _ceu_tst.o \
					-T $(CEUDIR)/kernel.ld -o all.elf
	#$(ARMGNU)-gcc -g $(CFLAGS)
				  #$(CEUDIR)/ceu_pool.c $(CEUDIR)/ceu_os.c $(CEUDIR)/vectors.s
				  #$(CEUDIR)/main.c $(CEUDIR)/stdlib.c
				  #tst.c _ceu_tst.c
				  #-o all.o
	#$(ARMGNU)-objdump -S -D all.o > /tmp/all.list
	#$(ARMGNU)-ld $(LDFLAGS) all.o -o all.elf
	! $(ARMGNU)-objdump -h all.elf | grep ".got"
	! $(ARMGNU)-objdump -h all.elf | grep ".bss"
	$(ARMGNU)-objdump -S -D all.elf > all.list
	$(ARMGNU)-size all.elf
	$(ARMGNU)-objcopy all.elf -O ihex all.hex
	$(ARMGNU)-objcopy all.elf -O binary all.bin
	cp all.bin kernel.img
#$(ARMGNU)-objcopy --change-addresses=$(FLASHADDR) -O ihex $@ $(TARGET).hex
#$(SRECCAT) $(TARGET).hex -intel -o $(TARGET).srec

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
	find . -name "*.bin"  | xargs rm -f
	find . -name "*.list" | xargs rm -f

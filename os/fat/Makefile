ARMGNU = arm-none-eabi
SRECCAT = srec_cat
CEUDIR = /data/ceu/ceu-rpi/os

CFLAGS = -Wall #-O2
CFLAGS += -DCEU_DEBUG -DCEU_OS -I $(CEUDIR)
CFLAGS += -nostdlib -nostartfiles -ffreestanding
CFLAGS += -fno-zero-initialized-in-bss
#CFLAGS += -fpic -mpic-data-is-text-relative
#CFLAGS += -fwhole-program #-flto
#LDFLAGS += -T $(CEUDIR)/kernel.ld #-flto

CFLAGS_ONE = -fpic #-mpic-data-is-text-relative
CFLAGS_ONE += -fno-zero-initialized-in-bss

LDFLAGS_ONE = -Wl,-T../one.ld

CFLAGS_FAT = -pedantic -pedantic-errors \
		 -nostdlib -nostartfiles -ffreestanding \
		 -Wall -Wextra -Wshadow \
		 -std=gnu99 \
		 -I. -DBUILDING_RPIBOOT \
		 -include config.h \
		$(CFLAGS_ONE)

tst1: fat kernel
	ceu tst1.ceu --os --verbose \
				--out-c _ceu_tst1.c \
				--cpp-exe "arm-none-eabi-cpp" \
				--cpp-args "$(CFLAGS)"
	#cp /tmp/_ceu_tst1.c .
	$(ARMGNU)-gcc -I. $(CFLAGS) $(CFLAGS_ONE) $(LDFLAGS_ONE) \
		../debug.c _ceu_tst1.c -o _ceu_tst1.elf
	! $(ARMGNU)-objdump -h _ceu_tst1.elf | grep ".got"
	! $(ARMGNU)-objdump -h _ceu_tst1.elf | grep ".bss"
	$(ARMGNU)-objcopy _ceu_tst1.elf -O binary _ceu_tst1.bin
	$(ARMGNU)-objcopy _ceu_tst1.elf --change-addresses=0x300000 -O ihex _ceu_tst1.hex
	###
	$(SRECCAT) kernel.hex    -intel -offset -0x8000 \
			   _ceu_fat.hex  -intel -offset -0x8000 \
			   _ceu_tst1.hex -intel -offset -0x8000 \
					-o all.hex -intel
	$(SRECCAT) all.hex -intel -o kernel.img -Binary

tst2: fat kernel
	cd ../led && make one && cd - && cp ../led/_ceu_led.bin .
	ceu tst2.ceu --os --verbose \
				--out-c _ceu_tst2.c \
				--cpp-exe "arm-none-eabi-cpp" \
				--cpp-args "$(CFLAGS)"
	#cp /tmp/_ceu_tst2.c .
	$(ARMGNU)-gcc -I. $(CFLAGS) $(CFLAGS_ONE) $(LDFLAGS_ONE) \
		../debug.c _ceu_tst2.c -o _ceu_tst2.elf
	! $(ARMGNU)-objdump -h _ceu_tst2.elf | grep ".got"
	! $(ARMGNU)-objdump -h _ceu_tst2.elf | grep ".bss"
	$(ARMGNU)-objcopy _ceu_tst2.elf -O binary _ceu_tst2.bin
	$(ARMGNU)-objcopy _ceu_tst2.elf --change-addresses=0x300000 -O ihex _ceu_tst2.hex
	###
	$(SRECCAT) kernel.hex    -intel -offset -0x8000 \
			   _ceu_fat.hex  -intel -offset -0x8000 \
			   _ceu_tst2.hex -intel -offset -0x8000 \
					-o all.hex -intel
	$(SRECCAT) all.hex -intel -o kernel.img -Binary

fat:
	ceu fat.ceu --os --verbose \
				--out-c _ceu_fat.c \
				--cpp-exe "arm-none-eabi-cpp" \
				--cpp-args "$(CFLAGS)"
	# libfs
	$(ARMGNU)-gcc $(CFLAGS_FAT) -c div.c -o div.o
	$(ARMGNU)-gcc $(CFLAGS_FAT) -c libfs.c -o libfs.o
	$(ARMGNU)-gcc $(CFLAGS_FAT) -c emmc.c -o emmc.o
	$(ARMGNU)-gcc $(CFLAGS_FAT) -c block.c -o block.o
	$(ARMGNU)-gcc $(CFLAGS_FAT) -c mbr.c -o mbr.o
	$(ARMGNU)-gcc $(CFLAGS_FAT) -c fat.c -o fat.o
	$(ARMGNU)-gcc $(CFLAGS_FAT) -c vfs.c -o vfs.o
	$(ARMGNU)-gcc $(CFLAGS_FAT) -c timer.c -o timer.o
	$(ARMGNU)-gcc $(CFLAGS_FAT) -c mmio.c -o mmio.o
	$(ARMGNU)-gcc $(CFLAGS_FAT) -c crc32.c -o crc32.o
	# fat
	#$(ARMGNU)-gcc $(CFLAGS_FAT) -c main.c -o main.o
	$(ARMGNU)-gcc $(SFLAGS_FAT) -Wa,-mcpu=arm1176jzf-s -c boot.s -o boot.o
	$(ARMGNU)-gcc $(CFLAGS_FAT) -c mbox.c -o mbox.o
	$(ARMGNU)-gcc $(CFLAGS_FAT) -c malloc.c -o malloc.o
	$(ARMGNU)-gcc $(CFLAGS_FAT) -c stdlib.c -o stdlib.o
	$(ARMGNU)-gcc $(CFLAGS_FAT) -c heap.c -o heap.o
	$(ARMGNU)-gcc $(CFLAGS_FAT) -c printf.c -o printf.o
	#$(ARMGNU)-gcc $(CFLAGS_FAT) -c uart.c -o uart.o
	# all
	$(ARMGNU)-gcc -I. $(CFLAGS) $(CFLAGS_ONE) $(LDFLAGS_ONE) \
		div.o libfs.o emmc.o block.o mbr.o fat.o vfs.o timer.o mmio.o boot.o \
		mbox.o stdlib.o heap.o malloc.o printf.o \
		_ceu_fat.c -o _ceu_fat.elf -lgcc
		# main.o uart.o
	#$(ARMGNU)-gcc $(CFLAGS) $(CFLAGS_FAT) $(LDFLAGS_ONE)
		#div.c libfs.c emmc.c block.c mbr.c fat.c vfs.c timer.c mmio.c crc32.c
		#boot.o main.c mbox.c stdlib.c malloc.c heap.c printf.c
		#_ceu_fat.c -o _ceu_fat.elf #-lgcc
	###
	! $(ARMGNU)-objdump -h _ceu_fat.elf | grep ".got"
	! $(ARMGNU)-objdump -h _ceu_fat.elf | grep ".bss"
	$(ARMGNU)-objcopy _ceu_fat.elf -O binary _ceu_fat.bin
	$(ARMGNU)-objcopy _ceu_fat.elf --change-addresses=0x110000 -O ihex _ceu_fat.hex

kernel:
	$(ARMGNU)-as $(CEUDIR)/vectors.s                -o vectors.o
	$(ARMGNU)-gcc $(CFLAGS) -c $(CEUDIR)/stdlib.c   -o stdlib.o
	$(ARMGNU)-gcc $(CFLAGS) -c $(CEUDIR)/ceu_pool.c -o ceu_pool.o
	$(ARMGNU)-gcc $(CFLAGS) -c $(CEUDIR)/debug.c    -o debug.o
	$(ARMGNU)-gcc $(CFLAGS) -c $(CEUDIR)/ceu_os.c   -o ceu_os.o
	$(ARMGNU)-gcc $(CFLAGS) -c tst1.c               -o app.o
	$(ARMGNU)-gcc $(CFLAGS) -c $(CEUDIR)/main.c     -o main.o
	$(ARMGNU)-ld vectors.o stdlib.o ceu_pool.o debug.o ceu_os.o app.o main.o \
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
	touch _ceu_led.bin
	sudo cp _ceu_led.bin /mnt/_ceu_led.bin
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
	find . -name "*.bin"  | xargs rm -f
	find . -name "*.list" | xargs rm -f

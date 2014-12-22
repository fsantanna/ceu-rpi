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
		 -Wall -Wextra -Werror -Wshadow \
		 -std=gnu99 \
		 -I. -DBUILDING_RPIBOOT \
		 -O3 \
		 -include config.h \
		$(CFLAGS_ONE)

all:
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
	$(ARMGNU)-gcc $(CFLAGS_FAT) -c main.c -o main.o
	$(ARMGNU)-gcc $(SFLAGS_FAT) -Wa,-mcpu=arm1176jzf-s -c boot.s -o boot.o
	$(ARMGNU)-gcc $(CFLAGS_FAT) -c mbox.c -o mbox.o
	$(ARMGNU)-gcc $(CFLAGS_FAT) -c malloc.c -o malloc.o
	$(ARMGNU)-gcc $(CFLAGS_FAT) -c stdlib.c -o stdlib.o
	$(ARMGNU)-gcc $(CFLAGS_FAT) -c heap.c -o heap.o
	$(ARMGNU)-gcc $(CFLAGS_FAT) -c printf.c -o printf.o
	# all
	$(ARMGNU)-gcc $(CFLAGS) $(CFLAGS_ONE) $(LDFLAGS_ONE) \
		div.o libfs.o emmc.o block.o mbr.o fat.o vfs.o timer.o mmio.o main.o boot.o \
		mbox.o stdlib.o heap.o malloc.o printf.o \
		_ceu_fat.c -o _ceu_fat.elf -lgcc
	#$(ARMGNU)-gcc $(CFLAGS) $(CFLAGS_FAT) $(LDFLAGS_ONE)
		#div.c libfs.c emmc.c block.c mbr.c fat.c vfs.c timer.c mmio.c crc32.c
		#boot.o main.c mbox.c stdlib.c malloc.c heap.c printf.c
		#_ceu_fat.c -o _ceu_fat.elf #-lgcc
	! $(ARMGNU)-objdump -h _ceu_fat.elf | grep ".got"
	! $(ARMGNU)-objdump -h _ceu_fat.elf | grep ".bss"
	$(ARMGNU)-objcopy _ceu_fat.elf -O binary _ceu_fat.bin
	$(ARMGNU)-objcopy _ceu_fat.elf --change-addresses=0x110000 -O ihex _ceu_fat.hex
	###
	ceu tst.ceu --os --verbose \
				--out-c _ceu_tst.c \
				--cpp-exe "arm-none-eabi-cpp" \
				--cpp-args "$(CFLAGS)"
	#cp /tmp/_ceu_tst.c .
	$(ARMGNU)-gcc $(CFLAGS) $(CFLAGS_ONE) $(LDFLAGS_ONE) \
		../debug.c _ceu_tst.c -o _ceu_tst.elf
	! $(ARMGNU)-objdump -h _ceu_tst.elf | grep ".got"
	! $(ARMGNU)-objdump -h _ceu_tst.elf | grep ".bss"
	$(ARMGNU)-objcopy _ceu_tst.elf -O binary _ceu_tst.bin
	$(ARMGNU)-objcopy _ceu_tst.elf --change-addresses=0x113000 -O ihex _ceu_tst.hex
	###
	$(ARMGNU)-as $(CEUDIR)/vectors.s                -o vectors.o
	$(ARMGNU)-gcc $(CFLAGS) -c $(CEUDIR)/stdlib.c   -o stdlib.o
	$(ARMGNU)-gcc $(CFLAGS) -c $(CEUDIR)/ceu_pool.c -o ceu_pool.o
	$(ARMGNU)-gcc $(CFLAGS) -c $(CEUDIR)/debug.c    -o debug.o
	$(ARMGNU)-gcc $(CFLAGS) -c $(CEUDIR)/ceu_os.c   -o ceu_os.o
	$(ARMGNU)-gcc $(CFLAGS) -c tst.c                -o app.o
	$(ARMGNU)-gcc $(CFLAGS) -c $(CEUDIR)/main.c     -o main.o
	$(ARMGNU)-ld vectors.o stdlib.o ceu_pool.o debug.o ceu_os.o app.o main.o \
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
	$(SRECCAT) kernel.hex    		-intel -offset -0x8000 \
			   _ceu_framebuffer.hex -intel -offset -0x8000 \
			   _ceu_tst.hex  		-intel -offset -0x8000 \
					-o all.hex -intel
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
	find . -name "*.bin"  | xargs rm -f
	find . -name "*.list" | xargs rm -f

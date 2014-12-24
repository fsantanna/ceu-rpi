GCC = arm-none-eabi-gcc
AR  = arm-none-eabi-ar
LD  = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy
CFLAGS = -pedantic -pedantic-errors \
		 -nostdlib -nostartfiles -ffreestanding \
		 -Wall -Wextra -Werror -Wshadow \
		 -std=gnu99 \
		 -I. -DBUILDING_RPIBOOT \
		 -O3 \
		 -include config.h

all: libfs.a
	$(GCC) $(CFLAGS) -c uart.c -o uart.o
	$(GCC) $(CFLAGS) -c main.c -o main.o
	$(GCC) -Wa,-mcpu=arm1176jzf-s -include config.h -c boot.s -o boot.o
	$(GCC) $(CFLAGS) -c mbox.c -o mbox.o
	$(GCC) $(CFLAGS) -c malloc.c -o malloc.o
	$(GCC) $(CFLAGS) -c stdlib.c -o stdlib.o
	$(GCC) $(CFLAGS) -c heap.c -o heap.o
	$(GCC) $(CFLAGS) -c printf.c -o printf.o
	$(GCC) $(CFLAGS) uart.o main.o boot.o mbox.o stdlib.o heap.o malloc.o printf.o \
		-Wl,-T,linker.ld -o kernel.elf -lgcc -L. -lfs
	$(OBJCOPY) kernel.elf -O binary kernel.img

libfs.a:
	$(GCC) $(CFLAGS) -c libfs.c -o libfs.o
	$(GCC) $(CFLAGS) -c emmc.c -o emmc.o
	$(GCC) $(CFLAGS) -c block.c -o block.o
	$(GCC) $(CFLAGS) -c mbr.c -o mbr.o
	$(GCC) $(CFLAGS) -c fat.c -o fat.o
	$(GCC) $(CFLAGS) -c vfs.c -o vfs.o
	$(GCC) $(CFLAGS) -c timer.c -o timer.o
	$(GCC) $(CFLAGS) -c mmio.c -o mmio.o
	$(GCC) $(CFLAGS) -c crc32.c -o crc32.o
	$(AR) rcs libfs.a libfs.o emmc.o block.o mbr.o fat.o vfs.o timer.o mmio.o crc32.o

sdcard:
	sudo mount /dev/sdb1 /mnt
	sudo cp kernel.img /mnt/kernel.img
	sudo umount /mnt

clean:
	rm -f *.o
	rm -f *.a

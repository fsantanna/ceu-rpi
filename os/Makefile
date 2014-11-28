ARMGNU  = arm-none-eabi
SRECCAT = srec_cat

CFLAGS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding
CFLAGS += -DCEU_OS

all:
	$(ARMGNU)-as vectors.s -o vectors.o
	$(ARMGNU)-gcc $(CFLAGS) -c stdlib.c -o stdlib.o
	$(ARMGNU)-gcc $(CFLAGS) -c ceu_pool.c -o ceu_pool.o
	$(ARMGNU)-gcc $(CFLAGS) -c ceu_os.c -o ceu_os.o
	$(ARMGNU)-gcc $(CFLAGS) -I. -c $(APP) -o app.o
	$(ARMGNU)-gcc $(CFLAGS) -c main.c -o main.o
	$(ARMGNU)-ld vectors.o stdlib.o ceu_pool.o ceu_os.o app.o main.o \
					-T kernel.ld -o kernel.elf
	$(ARMGNU)-objdump -D kernel.elf > kernel.list
	$(ARMGNU)-objcopy kernel.elf -O ihex kernel.hex
	$(SRECCAT) kernel.hex -intel -o kernel.srec
	$(ARMGNU)-objcopy kernel.elf -O binary kernel.bin

clean:
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.srec
	rm -f *.elf
	rm -f *.list
	rm -f *.img

sdcard:
	sudo mount /dev/sdb1 /mnt
	sudo cp kernel.bin /mnt/kernel.img
	sudo umount /dev/sdb1

xmodem:
	sx -vv kernel.bin < /dev/ttyUSB0 > /dev/ttyUSB0

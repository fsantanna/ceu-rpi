ARMGNU = arm-none-eabi
SRECCAT = srec_cat
CEUDIR = /data/ceu/ceu-rpi/os

CFLAGS = -Wall #-O2
CFLAGS += -DCEU_DEBUG -DCEU_OS -I $(CEUDIR)
CFLAGS += -nostdlib -nostartfiles -ffreestanding
CFLAGS += -nostdinc -isystem ../musl/include
CFLAGS += -I $(CEUDIR)/lua/lua-5.1.5/src
CFLAGS += -fno-zero-initialized-in-bss

CFLAGS_KERNEL = $(CFLAGS) $(CEUOSLUA)

CFLAGS_APP = -fpic -mpic-data-is-text-relative
CFLAGS_APP += -fno-zero-initialized-in-bss
LDFLAGS_APP = -Wl,-T../app.ld

CFLAGS_FAT = -pedantic -pedantic-errors \
		 -nostdlib -nostartfiles -ffreestanding \
		 -Wall -Wextra -Wshadow \
		 -std=gnu99 \
		 -I. -DBUILDING_RPIBOOT \
		 -include config.h \
		$(CFLAGS_APP)

tst1: APP=tst1.c
tst1: fat kernel
	ceu tst1.ceu --os --verbose \
				--out-c _ceu_tst1.c \
				--cpp-exe "arm-none-eabi-cpp" \
				--cpp-args "$(CFLAGS)"
	#cp /tmp/_ceu_tst1.c .
	$(ARMGNU)-gcc -Ilib/ $(CFLAGS) $(CFLAGS_APP) $(LDFLAGS_APP) \
		_ceu_tst1.c -o _ceu_tst1.elf
	! $(ARMGNU)-objdump -h _ceu_tst1.elf | grep ".got"
	! $(ARMGNU)-objdump -h _ceu_tst1.elf | grep ".bss"
	$(ARMGNU)-objcopy _ceu_tst1.elf -O binary tst1.app
	$(ARMGNU)-objcopy _ceu_tst1.elf --change-addresses=0x300000 -O ihex _ceu_tst1.hex
	###
	$(SRECCAT) kernel.hex    -intel -offset -0x8000 \
			   _ceu_fat.hex  -intel -offset -0x8000 \
			   _ceu_tst1.hex -intel -offset -0x8000 \
					-o all.hex -intel
	$(SRECCAT) all.hex -intel -o kernel.img -Binary

tst2: APP=tst2.c
tst2: fat kernel
	cd ../led && make led && cd - && cp ../led/led.app .
	ceu tst2.ceu --os --verbose \
				--out-c _ceu_tst2.c \
				--cpp-exe "arm-none-eabi-cpp" \
				--cpp-args "$(CFLAGS)"
	#cp /tmp/_ceu_tst2.c .
	$(ARMGNU)-gcc -Ilib/ $(CFLAGS) $(CFLAGS_APP) $(LDFLAGS_APP) \
		_ceu_tst2.c -o _ceu_tst2.elf
	! $(ARMGNU)-objdump -h _ceu_tst2.elf | grep ".got"
	! $(ARMGNU)-objdump -h _ceu_tst2.elf | grep ".bss"
	$(ARMGNU)-objcopy _ceu_tst2.elf -O binary tst2.app
	$(ARMGNU)-objcopy _ceu_tst2.elf --change-addresses=0x300000 -O ihex _ceu_tst2.hex
	###
	$(SRECCAT) kernel.hex    -intel -offset -0x8000 \
			   _ceu_fat.hex  -intel -offset -0x8000 \
			   _ceu_tst2.hex -intel -offset -0x8000 \
					-o all.hex -intel
	$(SRECCAT) all.hex -intel -o kernel.img -Binary

tst3: APP=tst3.c
tst3: fat kernel
	cd ../led  && make led  && cd - && cp ../led/led.app .
	cd ../uart && make uart && cd - && cp ../uart/uart.app .
	cd ../uart && make tst  && cd - && cp ../uart/tst.app uart_tst.app
	ceu tst3.ceu --os --verbose \
				--out-c _ceu_tst3.c \
				--cpp-exe "arm-none-eabi-cpp" \
				--cpp-args "$(CFLAGS)"
	#cp /tmp/_ceu_tst3.c .
	$(ARMGNU)-gcc -Ilib/ $(CFLAGS) $(CFLAGS_APP) $(LDFLAGS_APP) \
		_ceu_tst3.c -o _ceu_tst3.elf
	! $(ARMGNU)-objdump -h _ceu_tst3.elf | grep ".got"
	! $(ARMGNU)-objdump -h _ceu_tst3.elf | grep ".bss"
	$(ARMGNU)-objcopy _ceu_tst3.elf -O binary tst3.app
	$(ARMGNU)-objcopy _ceu_tst3.elf --change-addresses=0x300000 -O ihex _ceu_tst3.hex
	###
	$(SRECCAT) kernel.hex    -intel -offset -0x8000 \
			   _ceu_fat.hex  -intel -offset -0x8000 \
			   _ceu_tst3.hex -intel -offset -0x8000 \
					-o all.hex -intel
	$(SRECCAT) all.hex -intel -o kernel.img -Binary

tst4: APP=tst4.c
tst4: fat kernel
	cd ../led  && make led  && cd - && cp ../led/led.app .
	cd ../uart && make uart && cd - && cp ../uart/uart.app .
	cd ../uart && make tst  && cd - && cp ../uart/tst.app uart_tst.app
	cd ../lua  && make lua  && cd - && cp ../lua/lua.app .
	ceu tst4.ceu --os --verbose \
				--out-c _ceu_tst4.c \
				--cpp-exe "arm-none-eabi-cpp" \
				--cpp-args "$(CFLAGS)"
	#cp /tmp/_ceu_tst4.c .
	$(ARMGNU)-gcc -Ilib/ -I../lua/lua-5.1.5/src/ \
		$(CFLAGS) $(CFLAGS_APP) $(LDFLAGS_APP) \
		_ceu_tst4.c -o _ceu_tst4.elf
	! strings -a _ceu_lua.elf | grep libc | grep newlib
	! $(ARMGNU)-objdump -h _ceu_tst4.elf | grep ".got"
	! $(ARMGNU)-objdump -h _ceu_tst4.elf | grep ".bss"
	$(ARMGNU)-objcopy _ceu_tst4.elf -O binary tst4.app
	$(ARMGNU)-objcopy _ceu_tst4.elf --change-addresses=0x300000 -O ihex _ceu_tst4.hex
	###
	$(SRECCAT) kernel.hex    -intel -offset -0x8000 \
			   _ceu_fat.hex  -intel -offset -0x8000 \
			   _ceu_tst4.hex -intel -offset -0x8000 \
					-o all.hex -intel
	$(SRECCAT) all.hex -intel -o kernel.img -Binary

tst5: APP=tst5.c
#tst5: CEUOSLUA=-DCEU_OS_LUA
#tst5: ceuoslua=--os-lua
tst5: fat kernel
	cd ../led  && make led  && cd - && cp ../led/led.app .
	cd ../uart && make uart && cd - && cp ../uart/uart.app .
	cd ../uart && make tst  && cd - && cp ../uart/tst.app uart_tst.app
	cd ../lua  && make lua  && cd - && cp ../lua/_ceu_lua.elf .
	$(ARMGNU)-objcopy _ceu_lua.elf --change-addresses=0x300000 -O ihex _ceu_lua.hex
	###
	ceu tst5.ceu --os --verbose \
				--out-c _ceu_tst5.c \
				--cpp-exe "arm-none-eabi-cpp" \
				--cpp-args "$(CFLAGS)"
	#cp /tmp/_ceu_tst5.c .
	$(ARMGNU)-gcc -Ilib/ -I../lua/lua-5.1.5/src/ \
		$(CFLAGS) $(CFLAGS_APP) $(LDFLAGS_APP) \
		_ceu_tst5.c -o _ceu_tst5.elf -L ../musl/lib -lc
	! strings -a _ceu_lua.elf | grep libc | grep newlib
	! $(ARMGNU)-objdump -h _ceu_tst5.elf | grep ".got"
	! $(ARMGNU)-objdump -h _ceu_tst5.elf | grep ".bss"
	$(ARMGNU)-objcopy _ceu_tst5.elf -O binary tst5.app
	$(ARMGNU)-objcopy _ceu_tst5.elf --change-addresses=0x320000 -O ihex _ceu_tst5.hex
	###
	$(SRECCAT) kernel.hex    -intel -offset -0x8000 \
			   _ceu_fat.hex  -intel -offset -0x8000 \
			   _ceu_lua.hex  -intel -offset -0x8000 \
			   _ceu_tst5.hex -intel -offset -0x8000 \
					-o all.hex -intel
	$(SRECCAT) all.hex -intel -o kernel.img -Binary

init: fat kernel
	cd ../led  && make led  && cd - && cp ../led/led.app .
	cd ../uart && make uart && cd - && cp ../uart/uart.app .
	cd ../uart && make tst  && cd - && cp ../uart/tst.app uart_tst.app
	cd ../lua  && make lua  && cd - && cp ../lua/lua.app .
	ceu init.ceu --os --verbose \
				--out-c _ceu_init.c \
				--cpp-exe "arm-none-eabi-cpp" \
				--cpp-args "$(CFLAGS)"
	#cp /tmp/_ceu_init.c .
	$(ARMGNU)-gcc -Ilib/ -I../lua/lua-5.1.5/src/ \
		$(CFLAGS) $(CFLAGS_APP) $(LDFLAGS_APP) \
		_ceu_init.c -o _ceu_init.elf -L ../musl/lib -lc
	! strings -a _ceu_lua.elf | grep libc | grep newlib
	! $(ARMGNU)-objdump -h _ceu_init.elf | grep ".got"
	! $(ARMGNU)-objdump -h _ceu_init.elf | grep ".bss"
	$(ARMGNU)-objcopy _ceu_init.elf -O binary init.app
	$(ARMGNU)-objcopy _ceu_init.elf --change-addresses=0x300000 -O ihex _ceu_init.hex
	###
	$(SRECCAT) kernel.hex    -intel -offset -0x8000 \
			   _ceu_fat.hex  -intel -offset -0x8000 \
			   _ceu_init.hex -intel -offset -0x8000 \
					-o all.hex -intel
	$(SRECCAT) all.hex -intel -o kernel.img -Binary

fat:
	ceu fat.ceu --os $(ceuoslua) --verbose \
				--out-c _ceu_fat.c \
				--cpp-exe "arm-none-eabi-cpp" \
				--cpp-args "$(CFLAGS)"
	# libfs
	$(ARMGNU)-gcc $(CFLAGS_FAT) -Ilib/ -c lib/libfs.c -o lib/libfs.o
	$(ARMGNU)-gcc $(CFLAGS_FAT) -Ilib/ -c lib/emmc.c -o lib/emmc.o
	$(ARMGNU)-gcc $(CFLAGS_FAT) -Ilib/ -c lib/block.c -o lib/block.o
	$(ARMGNU)-gcc $(CFLAGS_FAT) -Ilib/ -c lib/mbr.c -o lib/mbr.o
	$(ARMGNU)-gcc $(CFLAGS_FAT) -Ilib/ -c lib/fat.c -o lib/fat.o
	$(ARMGNU)-gcc $(CFLAGS_FAT) -Ilib/ -c lib/vfs.c -o lib/vfs.o
	$(ARMGNU)-gcc $(CFLAGS_FAT) -Ilib/ -c lib/timer.c -o lib/timer.o
	$(ARMGNU)-gcc $(CFLAGS_FAT) -Ilib/ -c lib/mmio.c -o lib/mmio.o
	$(ARMGNU)-gcc $(CFLAGS_FAT) -Ilib/ -c lib/crc32.c -o lib/crc32.o
	$(ARMGNU)-gcc $(SFLAGS_FAT) -Wa,-mcpu=arm1176jzf-s -Ilib/ -c lib/boot.s -o lib/boot.o
	$(ARMGNU)-gcc $(CFLAGS_FAT) -Ilib/ -c lib/mbox.c -o lib/mbox.o
	$(ARMGNU)-gcc $(CFLAGS_FAT) -Ilib/ -c lib/malloc.c -o lib/malloc.o
	$(ARMGNU)-gcc $(CFLAGS_FAT) -Ilib/ -c lib/stdlib.c -o lib/stdlib.o
	$(ARMGNU)-gcc $(CFLAGS_FAT) -Ilib/ -c lib/heap.c -o lib/heap.o
	$(ARMGNU)-gcc $(CFLAGS_FAT) -Ilib/ -c lib/printf.c -o lib/printf.o
	#$(ARMGNU)-gcc $(CFLAGS_FAT) -c main.c -o main.o
	$(ARMGNU)-ar rcs libfat.a \
		lib/libfs.o lib/emmc.o lib/block.o lib/mbr.o lib/fat.o lib/vfs.o \
		lib/timer.o lib/mmio.o lib/boot.o lib/mbox.o lib/stdlib.o lib/heap.o \
		lib/malloc.o lib/printf.o
	# all
	$(ARMGNU)-gcc -Ilib/ $(CFLAGS) $(CFLAGS_APP) $(LDFLAGS_APP) \
		_ceu_fat.c -o _ceu_fat.elf -L. -lfat -lgcc
		# main.o
	#$(ARMGNU)-gcc $(CFLAGS) $(CFLAGS_FAT) $(LDFLAGS_APP)
		#div.c libfs.c emmc.c block.c mbr.c fat.c vfs.c timer.c mmio.c crc32.c
		#boot.o main.c mbox.c stdlib.c malloc.c heap.c printf.c
		#_ceu_fat.c -o _ceu_fat.elf #-lgcc
	###
	! $(ARMGNU)-objdump -h _ceu_fat.elf | grep ".got"
	! $(ARMGNU)-objdump -h _ceu_fat.elf | grep ".bss"
	$(ARMGNU)-objcopy _ceu_fat.elf -O binary fat.app
	$(ARMGNU)-objcopy _ceu_fat.elf --change-addresses=0x110000 -O ihex _ceu_fat.hex

kernel:
	$(ARMGNU)-as $(CEUDIR)/vectors.s                -o vectors.o
	$(ARMGNU)-gcc $(CFLAGS_KERNEL) -c $(CEUDIR)/stdlib.c   -o stdlib.o
	$(ARMGNU)-gcc $(CFLAGS_KERNEL) -c $(CEUDIR)/ceu_pool.c -o ceu_pool.o
	$(ARMGNU)-gcc $(CFLAGS_KERNEL) -c $(CEUDIR)/ceu_log.c  -o ceu_log.o
	$(ARMGNU)-gcc $(CFLAGS_KERNEL) -include $(CEUDIR)/ceu_log.h \
		-c $(CEUDIR)/ceu_os.c -o ceu_os.o
	$(ARMGNU)-gcc $(CFLAGS_KERNEL) -c $(APP)               -o app.o
	$(ARMGNU)-gcc $(CFLAGS_KERNEL) -c $(CEUDIR)/main.c     -o main.o
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
	sudo cp *.lua /mnt/
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

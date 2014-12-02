CEUFILE = tst.ceu
include ../one.mk

tst:
	make clean
	make CEUFILE=tst.ceu FLASHADDR=0x40000 all
	cd ../ && make APP=tst0/tst.c && \
		$(SRECCAT) kernel.hex -intel -offset -0x8000 tst0/_ceu_tst.hex -intel -offset -0x8000 -o all.hex -intel && \
		$(SRECCAT) all.hex -intel -o all.bin -Binary

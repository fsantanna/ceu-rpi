CEUFILE = tst.ceu
include ../one.mk

tst:
	make clean
	make CEUFILE=tst.ceu FLASHADDR=0x12000 all
	cd ../ && make APP=tst0/tst.c && \
		$(SRECCAT) kernel.srec tst0/_ceu_tst.srec > all.srec && \
		$(SRECCAT) all.srec -o all.bin -Binary

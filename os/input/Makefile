CEUFILE = tst.ceu
include ../one.mk

tst:
	make clean
	cd ../video && make CEUFILE=video.ceu FLASHADDR=0x12000
	make CEUFILE=tst.ceu   FLASHADDR=0x14000 all
	make CEUFILE=input.ceu LIBS=libcsud.a FLASHADDR=0x16000 DATAADDR=0x18000 all
	cd ../ && make APP=input/tst.c && \
		$(SRECCAT) kernel.srec video/_ceu_video.srec input/_ceu_input.srec input/_ceu_tst.srec > all.srec && \
		$(SRECCAT) all.srec -offset -0x8000 -o all.bin -Binary

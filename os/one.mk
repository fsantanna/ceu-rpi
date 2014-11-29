CEUDIR ?= /data/ceu/ceu-rpi
SRECCAT = srec_cat

ARMGNU = arm-none-eabi
CFLAGS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding
CFLAGS += -DCEU_OS -I $(CEUDIR)/os
CFLAGS += -fpic
#CFLAGS += -mrelax -mshort-calls #-mcall-prologues -fpic

ifndef CEUFILE
#$(error missing CEUFILE application)
endif

OBJECT = $(addprefix _ceu_, $(addsuffix .o, $(basename $(CEUFILE))))
TARGET = $(basename $(OBJECT))
FLASHADDR ?= 0x00

LINKFLAGS += -Wl,--section-start=.export=0x00 \
			 -Wl,--section-start=.text=0x24   \
			 -Wl,-uCEU_EXPORT
# TODO: 0x24 is hardcoded (size of CEU_EXPORT:  `objdump -d _ceu_tst.o´)

.PHONY:	all target flash

target: $(OBJECT)

all: target #upload

upload: _upload

.SECONDARY:		# keeps _ceu_xxx.c
_ceu_%.c: %.ceu
	ceu $< 	--os --verbose \
			--out-c $@ \
			--cpp-args "$(CFLAGS)"
			#--cpp-exe "$(CPP)"
	#cp /tmp/_ceu_tst.c .
#--out-h $(basename $@).h
#--out-s $(basename $<)_SIZE
#--out-f $(basename $<)_init

_ceu_%.o: _ceu_%.c $(LIBS)
	$(ARMGNU)-gcc $(CFLAGS) $(LINKFLAGS) \
		-nostartfiles \
		$^ -o $@
	! $(ARMGNU)-objdump -h $@ | fgrep ".data"
	#$(STRIP) -s $@
	$(ARMGNU)-size $@
	$(ARMGNU)-objcopy --change-addresses=$(FLASHADDR) -O ihex $@ $(TARGET).hex
	$(SRECCAT) $(TARGET).hex -intel -o $(TARGET).srec
	#$(SRECCAT) $(TARGET).srec -o $(TARGET).hex -intel

# WORKAROUND: OBJDUMP generates a difficult HEX to parse.
#             SRECCAT does HEX => SREC => HEX (easy one)
#	(update: I commented the two lines because I'm not parsing it in RPi)

#-nostartfiles -nodefaultlibs -nostdlib
#$(LINKFLAGS)
#$(COMPILE.c) -c -o $(APP).o $(APP).c
#$(CC) -Os -Wl,--gc-sections -mmcu=atmega644p $(APP).o _arduino.a -o $(APP).elf
#$(LD) -Ttext=$(APPTEXT) -o $(APP).elf $(APP).o _arduino.a

clean:
	find . -name "*.exe"  | xargs rm -f
	find . -name "_ceu_*" | xargs rm -f
	find . -name "*.hex"  | xargs rm -f
	find . -name "*.o"    | xargs rm -f

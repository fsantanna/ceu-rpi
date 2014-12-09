#define GPFSEL1         ((unsigned int*)0x20200004)
#define GPSET0          ((unsigned int*)0x2020001C)
#define GPCLR0          ((unsigned int*)0x20200028)
#define GPPUD           ((unsigned int*)0x20200094)
#define GPPUDCLK0       ((unsigned int*)0x20200098)

#define AUX_ENABLES     ((unsigned int*)0x20215004)
#define AUX_MU_IO_REG   ((unsigned int*)0x20215040)
#define AUX_MU_IER_REG  ((unsigned int*)0x20215044)
#define AUX_MU_IIR_REG  ((unsigned int*)0x20215048)
#define AUX_MU_LCR_REG  ((unsigned int*)0x2021504C)
#define AUX_MU_MCR_REG  ((unsigned int*)0x20215050)
#define AUX_MU_LSR_REG  ((unsigned int*)0x20215054)
#define AUX_MU_MSR_REG  ((unsigned int*)0x20215058)
#define AUX_MU_SCRATCH  ((unsigned int*)0x2021505C)
#define AUX_MU_CNTL_REG ((unsigned int*)0x20215060)
#define AUX_MU_STAT_REG ((unsigned int*)0x20215064)
#define AUX_MU_BAUD_REG ((unsigned int*)0x20215068)

void debug (char* s) {
    int i = 0;
    while (s[i] != '\0') {
        while (! (*AUX_MU_LSR_REG & 0x20) );
        *AUX_MU_IO_REG = s[i];
        i++;
    }
}

void debug_init (void) {
    *AUX_ENABLES = 1;
    *AUX_MU_IER_REG = 0;
    *AUX_MU_CNTL_REG = 0;
    *AUX_MU_LCR_REG = 3;
    *AUX_MU_MCR_REG = 0;
    *AUX_MU_IER_REG = 0;
    *AUX_MU_IIR_REG = 0xC6;
    *AUX_MU_BAUD_REG = 270;

    unsigned int v = *GPFSEL1;
    v = v & ~(7<<12);       //gpio14
    v = v | (2<<12);        //alt5
    v = v & ~(7<<15);       //gpio15
    v = v | (2<<15);        //alt5
    *GPFSEL1 = v;

    *GPPUD = 0;
    volatile int i=0;
    while(i++ < 10000);
    *GPPUDCLK0 = (1<<14)|(1<<15);
    i = 0;
    while(i++ < 10000);
    *GPPUDCLK0 = 0;

    *AUX_MU_CNTL_REG = 3;

    while(i++ < 9999999);
    debug("INIT\n");
}

char *tohex(unsigned int value, unsigned int size)
{
    static char buffer[9] = {0};

	unsigned int offset;
	unsigned char ch;

	if(size!=1 && size!=2 && size!=4)
		return "error";

	offset=size*2;

	buffer[offset] = 0;

	while(offset--)
	{
		ch = 48 + (value & 15);
		if(ch>=58)
			ch+=7;

		buffer[offset] = ch;
		value = value >> 4;
	}

	return buffer;
}

char *todec(unsigned int value, int leading)
{
	/* Biggest number is 4294967295 (10 digits) */
    static char buffer[11] = {0};
    static char leadchar = 0;

	unsigned int offset = 10;
	unsigned char ch;

	if(leading <0)
	{
		leading = -leading;
		leadchar = ' ';
	}
	else
	{
		leadchar = '0';
	}

	if(leading>10)
		return "error";

	buffer[offset] = 0;

	while(value || (offset == 10))
	{
		offset--;
		leading--;
		ch = 48 + (value % 10);
		buffer[offset] = ch;

		value = value/10;
	}

	while(leading>0)
	{
		offset--;
		leading--;
		buffer[offset] = leadchar;
	}

	return &buffer[offset];
}

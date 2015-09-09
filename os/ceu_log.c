#define GPFSEL1         ((volatile unsigned int*)0x20200004)
#define GPSET0          ((volatile unsigned int*)0x2020001C)
#define GPCLR0          ((volatile unsigned int*)0x20200028)
#define GPPUD           ((volatile unsigned int*)0x20200094)
#define GPPUDCLK0       ((volatile unsigned int*)0x20200098)

#define AUX_ENABLES     ((volatile unsigned int*)0x20215004)
#define AUX_MU_IO_REG   ((volatile unsigned int*)0x20215040)
#define AUX_MU_IER_REG  ((volatile unsigned int*)0x20215044)
#define AUX_MU_IIR_REG  ((volatile unsigned int*)0x20215048)
#define AUX_MU_LCR_REG  ((volatile unsigned int*)0x2021504C)
#define AUX_MU_MCR_REG  ((volatile unsigned int*)0x20215050)
#define AUX_MU_LSR_REG  ((volatile unsigned int*)0x20215054)
#define AUX_MU_MSR_REG  ((volatile unsigned int*)0x20215058)
#define AUX_MU_SCRATCH  ((volatile unsigned int*)0x2021505C)
#define AUX_MU_CNTL_REG ((volatile unsigned int*)0x20215060)
#define AUX_MU_STAT_REG ((volatile unsigned int*)0x20215064)
#define AUX_MU_BAUD_REG ((volatile unsigned int*)0x20215068)

static void init (void) {
    static int _init = 0;
    if (_init) {
        return;
    } else {
        volatile int i=0;
        while(i++ < 9999999);
        _init = 1;
    }

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
}

void ceu_log_putc (unsigned int c) {
    init();
    while (! (*AUX_MU_LSR_REG & 0x20) );
    *AUX_MU_IO_REG = c;
}

static void hex (unsigned int d) {
    //unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    rb=32;
    while(1) {
        rb-=4;
        rc=(d>>rb)&0xF;
        if(rc>9) rc+=0x37; else rc+=0x30;
        ceu_log_putc(rc);
        if(rb==0) break;
    }
    //ceu_log_putc(0x20);
}

void ceu_log (int mode, long s) {
    int i = 0;
    init();
    if (mode == 1) {
        // HEX
        hex((unsigned int)s);
    } else {
        // STRING
        while (((char*)s)[i] != '\0') {
            ceu_log_putc(((char*)s)[i]);
            i++;
        }
    }
}

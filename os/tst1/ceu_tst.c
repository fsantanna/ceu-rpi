static int XXX = 0x12345678;

__attribute__ ((section (".export")))
void CEU_EXPORT (void) {
    if (XXX == 0x12345678) {
        #define GPFSEL1 ((unsigned int*)0x20200004)
        #define GPSET0  ((unsigned int*)0x2020001C)
        #define GPCLR0  ((unsigned int*)0x20200028)
        unsigned int ra;
        ra = *GPFSEL1;
        ra = ra & ~(7<<18);
        ra = ra | 1<<18;
        *GPFSEL1 = ra;
        *GPCLR0 = 1<<16;   // GPIO16 on
    }
}

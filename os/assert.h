#ifndef GPFSEL1
#define GPFSEL1 ((uint*)0x20200004)
#define GPSET0  ((uint*)0x2020001C)
#define GPCLR0  ((uint*)0x20200028)
#endif

#define assert(x) {             \
    if (! (x)) {                \
        uint ra;                \
        ra = *GPFSEL1;          \
        ra = ra & ~(7<<18);     \
        ra = ra | 1<<18;        \
        *GPFSEL1 = ra;          \
        *GPCLR0 = 1<<16;        \
        /* *GPSET0 = 1<<16; */  \
        while(1);               \
    }                           \
}

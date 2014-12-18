#ifndef GPFSEL1
#define GPFSEL1 ((unsigned int*)0x20200004)
#define GPSET0  ((unsigned int*)0x2020001C)
#define GPCLR0  ((unsigned int*)0x20200028)
#endif

#define assert(x) {             \
    if (! (x)) {                \
        unsigned int ra;        \
        ra = *GPFSEL1;          \
        ra = ra & ~(7<<18);     \
        ra = ra | 1<<18;        \
        *GPFSEL1 = ra;          \
        *GPCLR0 = 1<<16;        \
        /* *GPSET0 = 1<<16; */  \
        while(1);               \
    }                           \
}

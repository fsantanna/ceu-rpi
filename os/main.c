#include "ceu_os.h"

extern u32  _GET32 (u32);
extern void MAIN (void);

/* system timer */
#define TIMER_CLO 0x20003004    /* current value (low 32-bit) */

u32 old;
int dt () {
    u32 now = _GET32(TIMER_CLO);
    int dt = now - old;     // no problems with overflow
    old = now;
    return dt;
}

void ceu_os_main ()
{
#define GPFSEL1 ((u32*)0x20200004)
#define GPSET0  ((u32*)0x2020001C)
#define GPCLR0  ((u32*)0x20200028)

// set GPIO16 as output
uint v = *GPFSEL1;
v = v & ~(7<<18);
v = v | 1<<18;
*GPFSEL1 = v;

if (((char*)0x4000)[0]==0x02) {
#define GPCLR0  ((u32*)0x20200028)
*GPCLR0 = 1<<16;   // GPIO16 on
}

int i;
volatile int _v;

//*GPCLR0 = 1<<16;   // GPIO16 on
//for(i=0; i<999999; i++) _v=i;
    ceu_os_init();
//*GPSET0 = 1<<16;   // GPIO16 off
//for(i=0; i<999999; i++) _v=i;
    MAIN();
//*GPCLR0 = 1<<16;   // GPIO16 on
//for(i=0; i<999999; i++) _v=i;
    old = _GET32(TIMER_CLO);
//*GPSET0 = 1<<16;   // GPIO16 off
//for(i=0; i<999999; i++) _v=i;
    ceu_os_scheduler(dt);
}

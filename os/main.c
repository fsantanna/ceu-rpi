#include "ceu_os.h"

extern void MAIN (void);

/* system timer */
#define TIMER_CLO ((volatile u32*)0x20003004)    /* current value (low 32-bit) */

u32 old = 0;
int dt () {
    u32 now = *TIMER_CLO;
    int dt = now - old;     // no problems with overflow
    old = now;
    return dt;
}

void ceu_sys_assert (int v) {
    #define GPFSEL1 ((uint*)0x20200004)
    #define GPSET0  ((uint*)0x2020001C)
    #define GPCLR0  ((uint*)0x20200028)
    uint ra;
    ra = *GPFSEL1;
    ra = ra & ~(7<<18);
    ra = ra | 1<<18;
    *GPFSEL1 = ra;
    if (v == 0) {
        volatile int i=0;
        while (1) {
            for (i=0; i<99999; i++);
            *GPCLR0 = 1<<16;   // GPIO16 on
            for (i=0; i<99999; i++);
            *GPSET0 = 1<<16;   // GPIO16 off
        }
    }
}

#include "ceu_log.h"
void ceu_sys_log (int mode, const void* s) {
    ceu_log(mode, s);
}

void ceu_os_main ()
{
    ceu_os_init();
    MAIN();
    old = *TIMER_CLO;
    ceu_os_scheduler(dt);
}

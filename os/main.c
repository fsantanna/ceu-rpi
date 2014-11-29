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
    ceu_os_init();
    MAIN();
    old = _GET32(TIMER_CLO);
    ceu_os_scheduler(dt);
}

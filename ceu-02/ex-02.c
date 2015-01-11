#include <stddef.h>
#include "ceu_types.h"

void* memset (void* s, int c, size_t n) {
    int i;
    for (i=0; i<n; i++) {
        ((char*)s)[i] = c;
    }
    return s;
}

#define TIMER_CS  0x20003000      // status
#define TIMER_CLO 0x20003004      // current value
#define TIMER_C0  0x2000300C
#define TIMER_C1  0x20003010      // match 1
#define TIMER_C2  0x20003014
#define TIMER_C3  0x20003018

extern void _PUT32 (unsigned int, unsigned int);
extern unsigned int _GET32 (unsigned int);
extern void _IRQ_ENABLE (void);

int DT;
#define ceu_out_wclock_set(dt)  \
    DT = dt;                    \
    _PUT32(TIMER_C1, _GET32(TIMER_CLO)+dt);

#include "_ceu_app.c"

tceu_app* APP;
void IRQ_HANDLER (void) {
    if (_GET32(TIMER_CS) & 0x02) {
        _PUT32(TIMER_CS,2);
        ceu_out_go(APP, CEU_IN__WCLOCK, &DT);
    }
}

extern void ceu_app_init (tceu_app* app);

int notmain (int argc, char *argv[])
{
    // enable system clock IRQ
    // TODO: generating IRQ for unknown C1 at the beginning?
    DT = 0;
    _PUT32(0x2000B210,0x00000002);
    _IRQ_ENABLE();

    byte CEU_DATA[sizeof(CEU_Main)];
    tceu_app app;
        app.data = (tceu_org*) &CEU_DATA;
        app.init = &ceu_app_init;
    APP = &app;

    int ret = ceu_go_all(&app);

#if 0
    printf("*** END: %d\n", ret);
    #include <unistd.h>
    sleep(1);  /* use when testing threads+valgrind */
#endif
    return ret;
}


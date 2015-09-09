#include <stddef.h>

#define ceu_out_assert(v)
#define ceu_out_log(m,s)

void* memset (void* s, int c, size_t n) {
    int i;
    for (i=0; i<n; i++) {
        ((char*)s)[i] = c;
    }
    return s;
}

#ifdef CEU_OS
    #include "ceu_os.h"
#else
    #include "_ceu_app.c"
#endif

extern void ceu_app_init (tceu_app* app);

int notmain (int argc, char *argv[])
{
    byte CEU_DATA[sizeof(CEU_Main)];
    tceu_app app;
        app.data = (tceu_org*) &CEU_DATA;
        app.init = &ceu_app_init;

    int ret = ceu_go_all(&app);

#if 0
    printf("*** END: %d\n", ret);
    #include <unistd.h>
    sleep(1);  /* use when testing threads+valgrind */
#endif
    return ret;
}

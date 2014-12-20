#include "ceu_os.h"

typedef void(*cb)(void);

void MAIN (void)
{
    //tceu_app* tst = ceu_sys_load(CEU_EXPORT);
    tceu_app* tst = ceu_sys_load((cb)0x110000);
    ceu_sys_start(tst);
    // TODO: check tst
}

#include "ceu_os.h"

void CEU_EXPORT (uint* size, tceu_init** init);

void MAIN (void)
{
    tceu_app* tst = ceu_sys_load(CEU_EXPORT);
    //tceu_app* tst = ceu_sys_load((void*)0x40000);
    ceu_sys_start(tst);
    // TODO: check tst
}

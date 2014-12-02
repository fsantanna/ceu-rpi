#include "ceu_os.h"

void CEU_EXPORT_FRAMEBUFFER (uint* size, tceu_init** init);
void CEU_EXPORT_TST         (uint* size, tceu_init** init);

void MAIN (void)
{
    //tceu_app* tst         = ceu_sys_load((void*)0x40000);
    //tceu_app* framebuffer = ceu_sys_load((void*)0x42000);
    tceu_app* framebuffer = ceu_sys_load(CEU_EXPORT_FRAMEBUFFER);
    tceu_app* tst   = ceu_sys_load(CEU_EXPORT_TST);

    ceu_sys_link( tst,1  ,  framebuffer,243 );

    ceu_sys_start(framebuffer);
    ceu_sys_start(tst);
}

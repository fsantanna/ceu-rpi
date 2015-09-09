#include "ceu_os.h"

void MAIN (void)
{
    tceu_app* framebuffer = ceu_sys_load((void*)0x110000);
    tceu_app* keyboard    = ceu_sys_load((void*)0x113000);
    tceu_app* tst         = ceu_sys_load((void*)0x128000);
    //assert(framebuffer != NULL);
    //assert(keyboard    != NULL);
    //assert(tst         != NULL);

    ceu_sys_link( tst,1       ,  framebuffer,242 );
    ceu_sys_link( tst,2       ,  framebuffer,241 );
    ceu_sys_link( tst,3       ,  framebuffer,240 );
    ceu_sys_link( keyboard,1  ,  tst,243   );

    ceu_sys_start(framebuffer);
    ceu_sys_start(keyboard);
    ceu_sys_start(tst);
}

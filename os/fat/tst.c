#include "ceu_os.h"
#include <assert.h>

void MAIN (void)
{
    tceu_app* fat = ceu_sys_load((void*)0x110000);
    //tceu_app* tst         = ceu_sys_load((void*)0x210000);
    assert(fat != NULL);
    //assert(tst         != NULL);

    //ceu_sys_link( tst,1  ,  framebuffer,243 );
    //ceu_sys_link( tst,2  ,  framebuffer,242 );
    //ceu_sys_link( tst,3  ,  framebuffer,241 );
    //ceu_sys_link( tst,4  ,  framebuffer,240 );

    ceu_sys_start(fat);
    //ceu_sys_start(tst);
}

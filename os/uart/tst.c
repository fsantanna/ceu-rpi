#include "ceu_os.h"

void MAIN (void)
{
    tceu_app* uart = ceu_sys_load((void*)0x110000);
    tceu_app* tst  = ceu_sys_load((void*)0x112000);

    ceu_sys_link( tst,1   ,  uart,243 );
    ceu_sys_link( uart,1  ,  tst,243 );

    ceu_sys_start(uart);
    ceu_sys_start(tst);
}

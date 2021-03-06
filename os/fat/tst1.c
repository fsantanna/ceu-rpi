#include "ceu_os.h"

void MAIN (void)
{
    tceu_app* fat = ceu_sys_load((void*)0x110000);
    tceu_app* tst = ceu_sys_load((void*)0x300000);
    ceu_sys_assert(fat != NULL);
    ceu_sys_assert(tst != NULL);

    ceu_sys_link( tst,1  ,  fat,243 );
    ceu_sys_link( tst,2  ,  fat,242 );
    ceu_sys_link( tst,3  ,  fat,241 );
    ceu_sys_link( tst,4  ,  fat,240 );

    ceu_sys_start(fat);
    ceu_sys_start(tst);
}

#include "ceu_os.h"
#include <assert.h>

void MAIN (void)
{
    tceu_app* lua = ceu_sys_load((void*)0x110000);
    tceu_app* tst = ceu_sys_load((void*)0x150000);
    assert(lua != NULL);
    assert(tst != NULL);

    ceu_sys_link( tst,1  ,  lua,243 );
    ceu_sys_link( tst,2  ,  lua,242 );
    ceu_sys_link( tst,3  ,  lua,241 );

    ceu_sys_start(lua);
    ceu_sys_start(tst);
}

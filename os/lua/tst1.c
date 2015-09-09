#include "ceu_os.h"

void MAIN (void)
{
    tceu_app* lua = ceu_sys_load((void*)0x110000);
    tceu_app* tst = ceu_sys_load((void*)0x150000);
    //ceu_sys_assert(lua != NULL);
    //ceu_sys_assert(tst != NULL);

    ceu_sys_link( tst, 1  ,  lua,243 );
    ceu_sys_link( tst, 2  ,  lua,242 );
    ceu_sys_link( tst, 3  ,  lua,241 );
    ceu_sys_link( tst, 4  ,  lua,240 );
    ceu_sys_link( tst, 5  ,  lua,239 );
    ceu_sys_link( tst, 6  ,  lua,238 );
    ceu_sys_link( tst, 7  ,  lua,237 );
    ceu_sys_link( tst, 8  ,  lua,236 );
    ceu_sys_link( tst, 9  ,  lua,235 );
    ceu_sys_link( tst,10  ,  lua,234 );
    ceu_sys_link( tst,11  ,  lua,233 );
    ceu_sys_link( tst,12  ,  lua,232 );
    ceu_sys_link( tst,13  ,  lua,231 );
    ceu_sys_link( tst,14  ,  lua,230 );
    ceu_sys_link( tst,15  ,  lua,229 );
    ceu_sys_link( tst,16  ,  lua,228 );
    ceu_sys_link( tst,17  ,  lua,227 );
    ceu_sys_link( tst,18  ,  lua,226 );
    ceu_sys_link( tst,19  ,  lua,225 );
    ceu_sys_link( tst,20  ,  lua,224 );
    ceu_sys_link( tst,21  ,  lua,223 );
    ceu_sys_link( tst,22  ,  lua,222 );
    ceu_sys_link( tst,23  ,  lua,221 );
    ceu_sys_link( tst,24  ,  lua,220 );
    ceu_sys_link( tst,25  ,  lua,219 );
    ceu_sys_link( tst,26  ,  lua,218 );
    ceu_sys_link( tst,27  ,  lua,217 );
    ceu_sys_link( tst,28  ,  lua,216 );

    ceu_sys_start(lua);
    ceu_sys_start(tst);
}

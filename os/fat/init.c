#include "ceu_os.h"
#include <assert.h>

void MAIN (void)
{
    tceu_app* fat  = ceu_sys_load((void*)0x110000);
    tceu_app* init = ceu_sys_load((void*)0x300000);
    assert(fat  != NULL);
    assert(init != NULL);

    // FAT
    ceu_sys_link( init,1  ,  fat,243 );  // OPEN
    ceu_sys_link( init,2  ,  fat,242 );  // CLOSE
    ceu_sys_link( init,3  ,  fat,241 );  // SIZE
    ceu_sys_link( init,4  ,  fat,240 );  // READ

    ceu_sys_start(fat);
    ceu_sys_start(init);
}

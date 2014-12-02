#include "ceu_os.h"

void MAIN (void)
{
    tceu_app* video = ceu_sys_load((void*)0x12000);
    tceu_app* tst   = ceu_sys_load((void*)0x14000);
    tceu_app* input = ceu_sys_load((void*)0x16000);

    ceu_sys_link( tst,1    ,  video,243 );
    ceu_sys_link( input,2  ,  video,243 );
    ceu_sys_link( input,1  ,  tst,243   );

    ceu_sys_start(video);
    ceu_sys_start(input);
    ceu_sys_start(tst);
}

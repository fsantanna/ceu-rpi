#include "ceu_os.h"
#include <assert.h>

void MAIN (void)
{
    tceu_app* fat = ceu_sys_load((void*)0x110000);
    tceu_app* lua = ceu_sys_load((void*)0x300000);
    tceu_app* tst = ceu_sys_load((void*)0x320000);
    assert(fat != NULL);
    assert(lua != NULL);
    assert(tst != NULL);

    // FAT
    ceu_sys_link( tst,1  ,  fat,243 );  // OPEN
    ceu_sys_link( tst,2  ,  fat,242 );  // CLOSE
    ceu_sys_link( tst,3  ,  fat,241 );  // SIZE
    ceu_sys_link( tst,4  ,  fat,240 );  // READ

    // LUA
    ceu_sys_link( tst,5   ,  lua,243 );
    ceu_sys_link( tst,6   ,  lua,242 );
    ceu_sys_link( tst,7   ,  lua,241 );
    ceu_sys_link( tst,8   ,  lua,240 );
    ceu_sys_link( tst,9   ,  lua,239 );
    ceu_sys_link( tst,10  ,  lua,238 );
    ceu_sys_link( tst,11  ,  lua,237 );
    ceu_sys_link( tst,12  ,  lua,236 );
    ceu_sys_link( tst,13  ,  lua,235 );
    ceu_sys_link( tst,14  ,  lua,234 );
    ceu_sys_link( tst,15  ,  lua,233 );  // POP
    ceu_sys_link( tst,16  ,  lua,232 );  // PUSHLIGHTUSERDATA
    ceu_sys_link( tst,17  ,  lua,231 );  // SETFIELD
    ceu_sys_link( tst,18  ,  lua,230 );  // OBJLEN
    ceu_sys_link( tst,19  ,  lua,229 );  // RAWGETI
    ceu_sys_link( tst,20  ,  lua,228 );  // GETTOP
    ceu_sys_link( tst,21  ,  lua,227 );  // TOLIGHTUSERDATA
    ceu_sys_link( tst,22  ,  lua,226 );  // L_LOADSTRING
    ceu_sys_link( tst,23  ,  lua,225 );  // ERROR
    ceu_sys_link( tst,24  ,  lua,224 );  // ISNUMBER
    ceu_sys_link( tst,25  ,  lua,223 );  // ISBOOLEAN
    ceu_sys_link( tst,26  ,  lua,222 );  // ISSTRING
    ceu_sys_link( tst,27  ,  lua,221 );  // ISLIGHTUSERDATA
    ceu_sys_link( tst,28  ,  lua,220 );  // TOBOOLEAN
    ceu_sys_link( tst,29  ,  lua,219 );  // TOUSERDATA
    ceu_sys_link( tst,30  ,  lua,218 );  // PCALL
    ceu_sys_link( tst,31  ,  lua,217 );  // PUSHSTRING
    ceu_sys_link( tst,32  ,  lua,216 );  // L_OPENLIBS

    ceu_sys_start(fat);
    ceu_sys_start(lua);
    ceu_sys_start(tst);
}

#include "ceu_os.h"

typedef struct {
    void *fat, *lua;
} apps_t;

void MAIN (void)
{
    tceu_app* fat  = ceu_sys_load((void*)0x110000);
    tceu_app* lua  = ceu_sys_load((void*)0x300000);
    tceu_app* init = ceu_sys_load((void*)0x320000);
    ceu_sys_assert(fat  != NULL);
    ceu_sys_assert(lua  != NULL);
    ceu_sys_assert(init != NULL);

    // FAT
    ceu_sys_link( init,1  ,  fat,243 );  // OPEN
    ceu_sys_link( init,2  ,  fat,242 );  // CLOSE
    ceu_sys_link( init,3  ,  fat,241 );  // SIZE
    ceu_sys_link( init,4  ,  fat,240 );  // READ

    // LUA
    ceu_sys_link( init,5   ,  lua,243 );
    ceu_sys_link( init,6   ,  lua,242 );
    ceu_sys_link( init,7   ,  lua,241 );
    ceu_sys_link( init,8   ,  lua,240 );
    ceu_sys_link( init,9   ,  lua,239 );
    ceu_sys_link( init,10  ,  lua,238 );
    ceu_sys_link( init,11  ,  lua,237 );
    ceu_sys_link( init,12  ,  lua,236 );
    ceu_sys_link( init,13  ,  lua,235 );
    ceu_sys_link( init,14  ,  lua,234 );
    ceu_sys_link( init,15  ,  lua,233 );  // POP
    ceu_sys_link( init,16  ,  lua,232 );  // PUSHLIGHTUSERDATA
    ceu_sys_link( init,17  ,  lua,231 );  // SETFIELD
    ceu_sys_link( init,18  ,  lua,230 );  // OBJLEN
    ceu_sys_link( init,19  ,  lua,229 );  // RAWGETI
    ceu_sys_link( init,20  ,  lua,228 );  // GETTOP
    ceu_sys_link( init,21  ,  lua,227 );  // TOLIGHTUSERDATA
    ceu_sys_link( init,22  ,  lua,226 );  // L_LOADSTRING
    ceu_sys_link( init,23  ,  lua,225 );  // ERROR
    ceu_sys_link( init,24  ,  lua,224 );  // ISNUMBER
    ceu_sys_link( init,25  ,  lua,223 );  // ISBOOLEAN
    ceu_sys_link( init,26  ,  lua,222 );  // ISSTRING
    ceu_sys_link( init,27  ,  lua,221 );  // ISLIGHTUSERDATA
    ceu_sys_link( init,28  ,  lua,220 );  // TOBOOLEAN
    ceu_sys_link( init,29  ,  lua,219 );  // TOUSERDATA
    ceu_sys_link( init,30  ,  lua,218 );  // PCALL
    ceu_sys_link( init,31  ,  lua,217 );  // PUSHSTRING
    ceu_sys_link( init,32  ,  lua,216 );  // L_OPENLIBS

    ceu_sys_start(fat);
    ceu_sys_start(lua);
    ceu_sys_start(init);

    /* emit APPS => (fat, lua); */
    {
        apps_t apps = { fat, lua };
        ceu_sys_go(init, 243, CEU_EVTP((void*)&apps));
    }
}

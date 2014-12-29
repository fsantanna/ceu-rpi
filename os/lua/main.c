#include "lua.h"
#include "lauxlib.h"
#include "ceu_log.h"
#include <stdlib.h>

static void* l_alloc (void *ud, void *ptr, size_t osize, size_t nsize) {
    (void)osize;  /* not used */
    (void)ud;  /* not used */
    void* ret = realloc(ptr, nsize);
    return ret;
}

static int l_writer (lua_State *L, const void* p, size_t sz, void* ud) {
    int i;
    ceu_log(0, "DUMP\n");
    ceu_log(1, sz);
    ceu_log(0, "\n");
    ceu_log(0, "\n");
    for (i=0; i<sz; i++) {
        ceu_log(1, ((char*)p)[i]);
        ceu_log(0, "\n");
    }
}

int notmain (int argc, char *argv[])
{
    volatile int i=0;
    while(i++ < 999999);
    ceu_log(0, "1\n");

    lua_State* l = lua_newstate(l_alloc, NULL);
        ceu_log(0, "2\n");
    int err = luaL_loadstring(l, "a=1\n");
    ceu_log(0, "LOAD\n");
    if (err) {
        ceu_log(0, "\tNO\n");
    } else {
        ceu_log(0, "\tOK\n");
        lua_dump(l, l_writer, NULL);
        err = lua_pcall(l, 0, LUA_MULTRET, 0);
        ceu_log(0, "PCALL\n");
        if (err) {
            ceu_log(0, "\tNO\n");
            ceu_log(0, lua_tostring(l, -1));
        } else {
            ceu_log(0, "OK\n");
        }
    }
    return 0;
}

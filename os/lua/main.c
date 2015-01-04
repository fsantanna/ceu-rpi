#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "ceu_log.h"
#include <stdlib.h>
#include <string.h>

#define MEM_TOTAL 1000000
static char   MEM_buf[MEM_TOTAL] = {0};   /* {0} avoids .bss */
static size_t MEM_i = 0;

void* myrealloc (void* ptr, size_t size) {
    if (size > 0) {
        size_t nxt = MEM_i + size;
        int mod = nxt % 4;  // TODO: 4 hardcoded
        if (mod) {
            nxt = nxt + (4 - mod);
        }
        if (nxt <= MEM_TOTAL) {
            void* ret = &MEM_buf[MEM_i];
            MEM_i = nxt;
            if (ptr != NULL) {
                // realloc
                memcpy(ret, ptr, size);
            }
            return ret;
        } else {
            ceu_log(0, "==> NO MEMORY\n");
            return NULL;
        }
    } else {
        if (ptr != NULL) {
            // TODO: free
        }
        return NULL;
    }
}

static void* l_alloc (void *ud, void *ptr, size_t osize, size_t nsize) {
    (void)osize;  /* not used */
    (void)ud;  /* not used */
    void* ret = myrealloc(ptr, nsize);
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
    ceu_log(0, "Starting...\n");

    lua_State* l = lua_newstate(l_alloc, NULL);
    int err = luaL_loadstring(l, "return 2\n");
    if (err) {
        ceu_log(0, lua_tostring(l, -1));
        goto ERR;
    } else {
        //lua_dump(l, l_writer, NULL);
        err = lua_pcall(l, 0, LUA_MULTRET, 0);
        if (err) {
            ceu_log(0, lua_tostring(l, -1));
            goto ERR;
        } else {
            int v = lua_tonumber(l, -1);
            if (v == 2) {
                ceu_log(0, "OK-1\n");
            } else {
                goto ERR;
            }
        }
    }

    luaL_openlibs(l);
    err = luaL_loadstring(l, "return next { k=10 }\n");
    if (err) {
        ceu_log(0, lua_tostring(l, -1));
        goto ERR;
    } else {
        //lua_dump(l, l_writer, NULL);
        err = lua_pcall(l, 0, LUA_MULTRET, 0);
        if (err) {
            ceu_log(0, lua_tostring(l, -1));
            goto ERR;
        } else {
            int v = lua_tonumber(l, -1);
            if (v == 10) {
                char* str = lua_tostring(l, -2);
                if (strcmp(str, "k") == 0) {
                    ceu_log(0, "OK-2\n");
                } else {
                    goto ERR;
                }
            } else {
                goto ERR;
            }
        }
    }

    return 0;

ERR:
    ceu_log(0, "\nNO\n");
    return 0;
}

#include <stdlib.h>
#include "ceu_log.h"

#define MEM_TOTAL 1000000
static char   MEM_buf[MEM_TOTAL] = {0};   /* {0} avoids .bss */
static size_t MEM_i = 0;

void* ceu_sys_realloc (void* ptr, size_t size) {
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

void* memset (void* s, int c, size_t n) {
    size_t i;
    for (i=0; i<n; i++) {
        ((char*)s)[i] = c;
    }
    return s;
}
void* memcpy (void* dest, const void* src, size_t n) {
    size_t i;
    for (i=0; i<n; i++) {
        ((char*)dest)[i] = ((char*)src)[i];
    }
    return dest;
}

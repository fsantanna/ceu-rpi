#include <stdlib.h>

#define MEM_TOTAL 1000000
static char   MEM_buf[MEM_TOTAL] = {0};   /* {0} avoids .bss */
static size_t MEM_i = 0;

void* malloc (size_t size) {
    size_t nxt = MEM_i + size;
    int mod = nxt % 4;  // TODO: 4 hardcoded
    if (mod) {
        nxt = nxt + (4 - mod);
    }
    if (nxt <= MEM_TOTAL) {
        void* ret = &MEM_buf[MEM_i];
        MEM_i = nxt;
        return ret;
    } else {
        return NULL;
    }
}
void free (void* ptr) {
    (void)ptr;
}

/*
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
*/

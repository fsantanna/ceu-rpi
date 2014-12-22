#include "div.h"

void mydiv (uint32_t n, uint32_t d, uint32_t* q, uint32_t* r) {
    uint32_t i = 0;
    while (n > d) {
        n -= d;
        i++;
    }
    if (q) *q = i;
    if (r) *r = n;
}

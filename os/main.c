#include "ceu_os.h"

extern void MAIN (void);

/* system timer */
#define TIMER_CLO ((volatile u32*)0x20003004)    /* current value (low 32-bit) */

u32 old = 0;
int dt () {
    u32 now = *TIMER_CLO;
    int dt = now - old;     // no problems with overflow
    old = now;
    return dt;
}

/*
    extern uint InitialiseFrameBuffer(u32 width, u32 height, u32 bitDepth);
    extern void SetGraphicsAddress (uint addr);
    extern u32  DrawCharacter (char c, u32 x, u32 y);
    extern void DrawString    (char* str, u32 length, u32 x, u32 y);
    extern void KeyboardUpdate  (void);
    extern char KeyboardGetChar (void);
*/
void ceu_os_main ()
{
/*
debug_init();
#define FB_PITCH 1024*2
#define FB_Bpp   2
uint fb = InitialiseFrameBuffer(1024, 768, 16);
SetGraphicsAddress(fb);
DrawString("Welcome to Ceu/OS!\n", 20, 100, 100);
*/
    ceu_os_init();
    MAIN();
    old = *TIMER_CLO;
    ceu_os_scheduler(dt);
}

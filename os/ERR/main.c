#include "ceu_os.h"

void ceu_os_main ()
{
    //tceu_app* keyboard    = ceu_sys_load(CEU_EXPORT_KEYBOARD);
#define GPFSEL1 ((uint*)0x20200004)
#define GPSET0  ((uint*)0x2020001C)
#define GPCLR0  ((uint*)0x20200028)
uint ra;
ra = *GPFSEL1;
ra = ra & ~(7<<18);
ra = ra | 1<<18;
*GPFSEL1 = ra;
*GPCLR0 = 1<<16;   // GPIO16 on
    //ceu_sys_go(app, CEU_IN__INIT, CEU_EVTP((void*)NULL));
//if (evt == CEU_IN__INIT) {
    void UsbInitialise   (void);
    void KeyboardUpdate  (void);
    char KeyboardGetChar (void);
UsbInitialise();
while(1) {
        KeyboardUpdate();
        char c = KeyboardGetChar();
        //await 1s;
        //var char c = 'x';
        if (c != '\0') {
#define GPFSEL1 ((uint*)0x20200004)
#define GPSET0  ((uint*)0x2020001C)
#define GPCLR0  ((uint*)0x20200028)
uint ra;
ra = *GPFSEL1;
ra = ra & ~(7<<18);
ra = ra | 1<<18;
*GPFSEL1 = ra;
*GPSET0 = 1<<16;   // GPIO16 off
        }
//}
}
while(1);
    //tceu_go go;
}

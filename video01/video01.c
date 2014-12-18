#include "image_smile.h"
#include "image_ceu.h"

extern void PUT8 ( unsigned int, char );
extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );

#define assert(x) x

unsigned int MailboxWrite ( unsigned int fbinfo_addr, unsigned int channel )
{
    unsigned int mailbox;

    mailbox=0x2000B880;
    while(1)
    {
        if((GET32(mailbox+0x18)&0x80000000)==0) break;
    }
    PUT32(mailbox+0x20,fbinfo_addr+channel);
    return(0);
}

unsigned int MailboxRead ( unsigned int channel )
{
    unsigned int ra;
    unsigned int mailbox;

    mailbox=0x2000B880;
    while(1)
    {
        while(1)
        {
            ra=GET32(mailbox+0x18);
            if((ra&0x40000000)==0) break;
        }
        ra=GET32(mailbox+0x00);
        if((ra&0xF)==channel) break;
    }
    return(ra&0xfffffff0); // TODO: ra>>4 ??
}


int notmain ( void )
{
    static volatile unsigned int FB[] __attribute__((aligned (16))) = {
        640, 480,
        640, 480,
        0,
        24,
        0, 0,
        0, 0
    };
    MailboxWrite(((unsigned int)FB)+0x40000000, 1);

    assert(MailboxRead(1) == 0); // TODO: check?

    {
        unsigned int ptr = FB[8];
        int i;
        for(i=0;i<640*480;i++) {
            PUT8(ptr++,0x00);
            PUT8(ptr++,0xFF);
            PUT8(ptr++,0x00);
        }
    }

/*
    {
        unsigned int ptr = FB[8];
        ptr += 640*10*3 + 50;
        int idx = 0;
        int i, j;
        for(i=0;i<10;i++) {
            for(j=0;j<10;j++) {
                PUT8(ptr++,image_smile[idx++]);
                PUT8(ptr++,image_smile[idx++]);
                PUT8(ptr++,image_smile[idx++]);
            }
            for(;j<640;j++) {
                PUT8(ptr++, 0x00);
                PUT8(ptr++, 0x00);
                PUT8(ptr++, 0x00);
            }
        }
    }

    {
        unsigned int ptr = FB[8];
        ptr += 640*30*3 + 50;
        int idx = 0;
        int i, j;
        for(i=0;i<105;i++) {
            for(j=0;j<288;j++) {
                PUT8(ptr++,image_ceu[idx+0]);
                PUT8(ptr++,image_ceu[idx+2]);   // TODO: why?
                PUT8(ptr++,image_ceu[idx+1]);   // TODO: why?
                idx += 3;
            }
            for(;j<640;j++) {
                PUT8(ptr++, 0x00);
                PUT8(ptr++, 0x00);
                PUT8(ptr++, 0x00);
            }
        }
    }
*/

    return(0);
}

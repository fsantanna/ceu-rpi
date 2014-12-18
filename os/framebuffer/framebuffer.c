#include "framebuffer.h"

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
        //hexstrings(ra);
        ra=GET32(mailbox+0x00);
        //hexstring(ra);
        if((ra&0xF)==channel) break;
    }
    return(ra&0xfffffff0); // TODO: ra>>4 ??
}

void DrawPixel(unsigned int x, unsigned int y) {
    if (x<0 || x>FB[0]) return;
    if (y<0 || y>FB[1]) return;
    unsigned int addr = FB[8] + (x + y*FB[0]) * FB_BPP;
    PUT8(addr+0, 0xFF);
    PUT8(addr+1, 0xFF);
    PUT8(addr+2, 0xFF);
}

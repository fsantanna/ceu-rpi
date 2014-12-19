#include <assert.h>
#include "framebuffer.h"
#include "teletext.h"

#define PUT8(x,y)  (*((volatile unsigned char*)(x))) = y
#define GET32(x)   (*((volatile unsigned int*)(x)))
#define PUT32(x,y) (*((volatile unsigned int*)(x))) = y

#define FB_bpp 24
#define FB_BPP (FB_bpp/8)

static volatile unsigned int FB[] __attribute__((aligned (16))) = {
    640, 480,
    640, 480,
    0,
    FB_bpp,
    0, 0,
    0, 0
};

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

void FB_init (void) { // TODO: parameters
    MailboxWrite(((unsigned int)FB)+0x40000000, 1);
    assert(MailboxRead(1) == 0); // TODO: check?
}

void FB_draw_pixel (unsigned int x, unsigned int y,
                    unsigned char r, unsigned char g, unsigned char b) {
    volatile unsigned int addr = FB[8] + (x + y*FB[0]) * FB_BPP;
    PUT8(addr+0, r);
    PUT8(addr+1, g);
    PUT8(addr+2, b);
}

void FB_draw_character (char c, unsigned int x, unsigned int y,
                        unsigned char r, unsigned char g, unsigned char b) {
    unsigned char* CHAR = FB_FONT[c-' '];   // 9=bytes per char
    int row, col;

    assert(c>=0x20 && c<0x7F);   // [SPC,DEL[

    for (row=0; row<9; row++) {        // 9=lines per char
        for (col=0; col<5; col++) {   // 5=row per char
            if (CHAR[row] & (1<<(4-col))) {
                FB_draw_pixel(x+col, y+row, r, g, b);
            }
        }
    }
}

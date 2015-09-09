extern void _PUT16 ( unsigned int, unsigned int );
extern void _PUT32 ( unsigned int, unsigned int );
extern unsigned int _GET32 ( unsigned int );
extern void UsbInitialise (void);
extern void KeyboardUpdate (void);
extern char KeyboardGetChar (void);

#define ceu_out_assert(v)
#define ceu_out_log(m,s)

#define assert(x) x

unsigned int MailboxWrite ( unsigned int fbinfo_addr, unsigned int channel )
{
    unsigned int mailbox;

    mailbox=0x2000B880;
    while(1)
    {
        if((_GET32(mailbox+0x18)&0x80000000)==0) break;
    }
    _PUT32(mailbox+0x20,fbinfo_addr+channel);
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
            ra=_GET32(mailbox+0x18);
            if((ra&0x40000000)==0) break;
        }
        ra=_GET32(mailbox+0x00);
        if((ra&0xF)==channel) break;
    }
    return(ra&0xfffffff0); // TODO: ra>>4 ??
}

///////////////////////////////////////////////////////////////////////////////

#include "memutils.h"
#include "teletext.h"

#define CHARSIZE_X	6
#define CHARSIZE_Y	10

#define FB_W   640//1600
#define FB_H   480//1200
#define FB_bpp 16
#define FB_Bpp (FB_bpp/8)
#if FB_bpp == 16
#define FB_PUT _PUT16
#elif FB_bpp == 32
#define FB_PUT _PUT32
#error TODO: colors
#else
#error invalid depth
#endif

static unsigned int FB_PTR;
static unsigned int FB_PITCH;
static unsigned int FB_MAX_X, FB_MAX_Y;

static int consx = 0;
static int consy = 0;

static unsigned short int fgcolour = 0xffff;
static unsigned short int bgcolour = 0;

static unsigned int colour_stack[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
static unsigned int colour_sp = 8;

/* Control characters for the console */
#define FG_RED "\001"
#define FG_GREEN "\002"
#define FG_BLUE "\003"
#define FG_YELLOW "\004"
#define FG_MAGENTA "\005"
#define FG_CYAN "\006"
#define FG_WHITE "\007"
#define FG_BLACK "\010"
#define FG_HALF "\011"

#define COLOUR_PUSH "\013"
#define COLOUR_POP "\014"

#define BG_RED "\021"
#define BG_GREEN "\022"
#define BG_BLUE "\023"
#define BG_YELLOW "\024"
#define BG_MAGENTA "\025"
#define BG_CYAN "\026"
#define BG_WHITE "\027"
#define BG_BLACK "\030"
#define BG_HALF "\031"

static void newline()
{
	unsigned int source;
	/* Number of bytes in a character row */
    register unsigned int rowbytes = CHARSIZE_Y * FB_PITCH;

	consx = 0;
    if(consy<(FB_H-1))
	{
		consy++;
		return;
	}

	/* Copy a screen's worth of data (minus 1 character row) from the
	 * second row to the first
	 */

	/* Calculate the address to copy the screen data from */
    source = FB_PTR + rowbytes;
    memmove((void *)FB_PTR, (void *)source, (FB_H-1)*rowbytes);

	/* Clear last line on screen */
    memclr((void *)(FB_PTR + (FB_H-1)*rowbytes), rowbytes);
}

void console_write(char *text)
{
    volatile unsigned int ptr;

	unsigned int row, addr;
	int col;
	unsigned char ch;

	/* Double parentheses to silence compiler warnings about
	 * assignments as boolean values
	 */
	while((ch = (unsigned char)*text))
	{
		text++;

		/* Deal with control codes */
		switch(ch)
		{
			case 1: fgcolour = 0b1111100000000000; continue;
			case 2: fgcolour = 0b0000011111100000; continue;
			case 3: fgcolour = 0b0000000000011111; continue;
			case 4: fgcolour = 0b1111111111100000; continue;
			case 5: fgcolour = 0b1111100000011111; continue;
			case 6: fgcolour = 0b0000011111111111; continue;
			case 7: fgcolour = 0b1111111111111111; continue;
			case 8: fgcolour = 0b0000000000000000; continue;
				/* Half brightness */
			case 9: fgcolour = (fgcolour >> 1) & 0b0111101111101111; continue;
			case 10: newline(); continue;
			case 11: /* Colour stack push */
				if(colour_sp)
					colour_sp--;
				colour_stack[colour_sp] =
					fgcolour | (bgcolour<<16);
				continue;
			case 12: /* Colour stack pop */
				fgcolour = colour_stack[colour_sp] & 0xffff;
				bgcolour = colour_stack[colour_sp] >> 16;
				if(colour_sp<8)
					colour_sp++;
				continue;
			case 17: bgcolour = 0b1111100000000000; continue;
			case 18: bgcolour = 0b0000011111100000; continue;
			case 19: bgcolour = 0b0000000000011111; continue;
			case 20: bgcolour = 0b1111111111100000; continue;
			case 21: bgcolour = 0b1111100000011111; continue;
			case 22: bgcolour = 0b0000011111111111; continue;
			case 23: bgcolour = 0b1111111111111111; continue;
			case 24: bgcolour = 0b0000000000000000; continue;
				/* Half brightness */
			case 25: bgcolour = (bgcolour >> 1) & 0b0111101111101111; continue;
		}

		/* Unknown control codes, and anything >127, get turned into
		 * spaces. Anything >=32 <=127 gets 32 subtracted from it to
		 * turn it into a value between 0 and 95, to index into the
		 * character definitions table
		 */
		if(ch<32)
		{
			ch=0;
		}
		else
		{
			if(ch>127)
				ch=0;
			else
				ch-=32;
		}

		/* Plot character onto screen
		 *
		 * CHARSIZE_Y and CHARSIZE_X are the size of the block the
		 * character occupies. The character itself is one pixel
		 * smaller in each direction, and is located in the upper left
		 * of the block
		 */
		for(row=0; row<CHARSIZE_Y; row++)
		{
            addr = (row+consy*CHARSIZE_Y)*FB_PITCH + consx*CHARSIZE_X*FB_Bpp;

			for(col=(CHARSIZE_X-2); col>=0; col--)
			{
                ptr = (FB_PTR+addr);

                addr += FB_Bpp;

                if(row<(CHARSIZE_Y-1) && (teletext[ch][row] & (1<<col))) {
                    FB_PUT(ptr, fgcolour);
                } else {
                    FB_PUT(ptr, bgcolour);
                }
            }

            ptr = (FB_PTR+addr);
            FB_PUT(ptr, bgcolour);
        }

        if(++consx >=FB_W)
		{
			newline();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

#include <stddef.h>

void* memset (void* s, int c, size_t n) {
    int i;
    for (i=0; i<n; i++) {
        ((char*)s)[i] = c;
    }
    return s;
}

#ifdef CEU_OS
    #include "ceu_os.h"
#else
    #include "_ceu_app.c"
#endif

extern void ceu_app_init (tceu_app* app);

int notmain ( void )
{
/*
*/
    static volatile unsigned int FB[] __attribute__((aligned (16))) = {
        FB_W, FB_H,
        FB_W, FB_H,
        0,
        FB_bpp,
        0, 0,
        0, 0
    };
    MailboxWrite(((unsigned int)FB)+0x40000000, 1);

    assert(MailboxRead(1) == 0); // TODO: check?
    FB_PITCH = FB_W*FB_Bpp;//FB[5];
    FB_PTR   = FB[8];
    FB_MAX_X = FB_W / CHARSIZE_X;
    FB_MAX_Y = FB_H / CHARSIZE_Y;

    consx = 0;
    consy = 0;
    fgcolour = 0xffff;
    bgcolour = 0;
    colour_sp = 8;
    colour_stack[0] = 0;
    colour_stack[1] = 0;
    colour_stack[2] = 0;
    colour_stack[3] = 0;
    colour_stack[4] = 0;
    colour_stack[5] = 0;
    colour_stack[6] = 0;
    colour_stack[7] = 0;

    byte CEU_DATA[sizeof(CEU_Main)];
    tceu_app app;
        app.data = (tceu_org*) &CEU_DATA;
        app.init = &ceu_app_init;

    int ret = ceu_go_all(&app);
    return ret;
}

#ifndef MEMUTILS_H
#define MEMUTILS_H

/* Clear length bytes of memory (set to 0) starting at address */
extern void memclr(void *address, unsigned int length);

/* Move length bytes from src to dest. Memory areas may overlap */
extern void *memmove(void *dest, const void *src, unsigned int length);

#endif	/* MEMUTILS_H */

// Character definitions from SAA5050 datasheet
// Each character is a 5x9 bit matrix
// 9 rows of 5-bit numbers
static unsigned char teletext[][9] = {
	{ 0,0,0,0,0,0,0,0,0 },		// space
	{ 4,4,4,4,4,0,4,0,0 },		// !
	{ 10,10,10,0,0,0,0,0,0 },	// "
	{ 6,9,8,28,8,8,31,0,0 },	// £		# in ASCII
	{ 14,21,20,14,5,21,14,0,0 },	// $
	{ 24,25,2,4,8,19,3,0,0 },	// %
	{ 8,20,20,8,21,18,13,0,0 },	// &
	{ 4,4,4,0,0,0,0,0,0 },		// '
	{ 2,4,8,8,8,4,2,0,0 },		// (
	{ 8,4,2,2,2,4,8,0,0 },		// )
	{ 4,21,14,4,14,21,4,0,0 },	// *
	{ 0,4,4,31,4,4,0,0,0 },		// +
	{ 0,0,0,0,0,4,4,8,0 },		// ,
	{ 0,0,0,14,0,0,0,0,0 },		// -
	{ 0,0,0,0,0,0,4,0,0 },		// .
	{ 0,1,2,4,8,16,0,0,0 },		// /
	{ 4,10,17,17,17,10,4,0,0 },	// 0
	{ 4,12,4,4,4,4,14,0,0 },	// 1
	{ 14,17,1,6,8,16,31,0,0 },	// 2
	{ 31,1,2,6,1,17,14,0,0 },	// 3
	{ 2,6,10,18,31,2,2,0,0 },	// 4
	{ 31,16,30,1,1,17,14,0,0 },	// 5
	{ 6,8,16,30,17,17,14,0,0 },	// 6
	{ 31,1,2,4,8,8,8,0,0 },		// 7
	{ 14,17,17,14,17,17,14,0,0 },	// 8
	{ 14,17,17,15,1,2,12,0,0 },	// 9
	{ 0,0,4,0,0,0,4,0,0 },		// :
	{ 0,0,4,0,0,4,4,8,0 },		// ;
	{ 2,4,8,16,8,4,2,0,0 },		// <
	{ 0,0,31,0,31,0,0,0,0 },	// =
	{ 8,4,2,1,2,4,8,0,0 },		// >
	{ 14,17,2,4,4,0,4,0,0 },	// ?
	{ 14,17,23,21,23,16,14,0,0 },	// @
	{ 4,10,17,17,31,17,17,0,0 },	// A
	{ 30,17,17,30,17,17,30,0,0 },	// B
	{ 14,17,16,16,16,17,14,0,0 },	// C
	{ 30,17,17,17,17,17,30,0,0 },	// D
	{ 31,16,16,30,16,16,31,0,0 },	// E
	{ 31,16,16,30,16,16,16,0,0 },	// F
	{ 14,17,16,16,19,17,15,0,0 },	// G
	{ 17,17,17,31,17,17,17,0,0 },	// H
	{ 14,4,4,4,4,4,14,0,0 },	// I
	{ 1,1,1,1,1,17,14,0,0 },	// J
	{ 17,18,20,24,20,18,17,0,0 },	// K
	{ 16,16,16,16,16,16,31,0,0 },	// L
	{ 17,27,21,21,17,17,17,0,0 },	// M
	{ 17,17,25,21,19,17,17,0,0 },	// N
	{ 14,17,17,17,17,17,14,0,0 },	// O
	{ 30,17,17,30,16,16,16,0,0 },	// P
	{ 14,17,17,17,21,18,13,0,0 },	// Q
	{ 30,17,17,30,20,18,17,0,0 },	// R
	{ 14,17,16,14,1,17,14,0,0 },	// S
	{ 31,4,4,4,4,4,4,0,0 },		// T
	{ 17,17,17,17,17,17,14,0,0 },	// U
	{ 17,17,17,10,10,4,4,0,0 },	// V
	{ 17,17,17,21,21,21,10,0,0 },	// W
	{ 17,17,10,4,10,17,17,0,0 },	// X
	{ 17,17,10,4,4,4,4,0,0 },	// Y
	{ 31,1,2,4,8,16,31,0,0 },	// Z
	{ 0,4,8,31,8,4,0,0,0 },		// left arrow	[ in ASCII
	{ 16,16,16,16,22,1,2,4,7 },	// 1/2		\ in ASCII
	{ 0,4,2,31,2,4,0,0,0 },		// right arrow	] in ASCII
	{ 0,4,14,21,4,4,0,0,0 },	// up arrow	^ in ASCII
	{ 10,10,31,10,31,10,10,0,0 },	// #		_ in ASCII
	{ 0,0,0,31,0,0,0,0,0 },		// dash		` in ASCII
	{ 0,0,14,1,15,17,15,0,0 },	// a
	{ 16,16,30,17,17,17,30,0,0 },	// b
	{ 0,0,15,16,16,16,15,0,0 },	// c
	{ 1,1,15,17,17,17,15,0,0 },	// d
	{ 0,0,14,17,31,16,14,0,0 },	// e
	{ 2,4,4,14,4,4,4,0,0 },		// f
	{ 0,0,15,17,17,17,15,1,14 },	// g
	{ 16,16,30,17,17,17,17,0,0 },	// h
	{ 4,0,12,4,4,4,14,0,0 },	// i
	{ 4,0,4,4,4,4,4,4,8 },		// j
	{ 8,8,9,10,12,10,9,0,0 },	// k
	{ 12,4,4,4,4,4,14,0,0 },	// l
	{ 0,0,26,21,21,21,21,0,0 },	// m
	{ 0,0,30,17,17,17,17,0,0 },	// n
	{ 0,0,14,17,17,17,14,0,0 },	// o
	{ 0,0,30,17,17,17,30,16,16 },	// p
	{ 0,0,15,17,17,17,15,1,1 },	// q
	{ 0,0,11,12,8,8,8,0,0 },	// r
	{ 0,0,15,16,14,1,30,0,0 },	// s
	{ 4,4,14,4,4,4,2,0,0 },		// t
	{ 0,0,17,17,17,17,15,0,0 },	// u
	{ 0,0,17,17,10,10,4,0,0 },	// v
	{ 0,0,17,17,21,21,10,0,0 },	// w
	{ 0,0,17,10,4,10,17,0,0 },	// x
	{ 0,0,17,17,17,17,15,1,14 },	// y
	{ 0,0,31,2,4,8,31,0,0 },	// z
	{ 8,8,8,8,9,3,5,7,1 },		// 1/4		{ in ASCII
	{ 10,10,10,10,10,10,10,0,0 },	// ||		| in ASCII
	{ 24,4,24,4,25,3,5,7,1 },	// 3/4		} in ASCII
	{ 0,4,0,31,0,4,0,0,0 },		// divide sign	~ in ASCII
	{ 31,31,31,31,31,31,31,0,0 }	// character-sized block
					//		not defined in ASCII
};

#ifndef TEXTUTILS_H
#define TEXTUTILS_H

extern char *tohex(unsigned int value, unsigned int size);
extern char *todec(unsigned int value, int leading);

#endif	/* TEXTUTILS_H */
/* Various memory utilities */

/* Convert an unsigned value to hex (without the trailing "0x")
 * size = size in bytes (only 1, 2 or 4 permitted)
 */
char *tohex(unsigned int value, unsigned int size)
{
	static char buffer[9];

	unsigned int offset;
	unsigned char ch;

	if(size!=1 && size!=2 && size!=4)
		return "error";

	offset=size*2;

	buffer[offset] = 0;

	while(offset--)
	{
		ch = 48 + (value & 15);
		if(ch>=58)
			ch+=7;

		buffer[offset] = ch;
		value = value >> 4;
	}

	return buffer;
}

/* Convert unsigned value to decimal
 * leading = 0 - no leading spaces/zeroes
 * leading >0 - number of leading zeroes
 * leading <0 - number (when +ve) of leading spaces
 */
char *todec(unsigned int value, int leading)
{
	/* Biggest number is 4294967295 (10 digits) */
	static char buffer[11];
	static char leadchar;

	unsigned int offset = 10;
	unsigned char ch;

	if(leading <0)
	{
		leading = -leading;
		leadchar = ' ';
	}
	else
	{
		leadchar = '0';
	}

	if(leading>10)
		return "error";

	buffer[offset] = 0;

	while(value || (offset == 10))
	{
		offset--;
		leading--;
		ch = 48 + (value % 10);
		buffer[offset] = ch;

		value = value/10;
	}

	while(leading>0)
	{
		offset--;
		leading--;
		buffer[offset] = leadchar;
	}

	return &buffer[offset];
}

/* Clear (set to 0) length bytes of memory starting at address
 */
void memclr(void *address, unsigned int length)
{
	register unsigned int addr = (unsigned int)address;

	/* If the start address is unaligned, fill in the first 1-3 bytes
	 * until it is
	 */
	while((addr & 3) && length)
	{
		*((unsigned char *)addr) = 0;
		addr++;
		length--;
	}

	/* Fill in the remaining 32-bit word-aligned memory locations */
	while(length & 0xfffffffc)
	{
		*((unsigned int *)addr) = 0;
		addr+=4;
		length-=4;
	}

	/* Deal with the remaining 1-3 bytes, if any */
	while(length)
	{
		addr++;
		length--;
		*((unsigned char *)addr) = 0;
	}
}
	
/* Move length bytes from src to dest. Memory areas may overlap
 * Four possibilities:
 * ..[...src...]..[...dest...]..	-- non-overlapping
 * ..[...dest...]..[...src...]..	-- non-overlapping
 * ..[...src..[.]..dest...]..		-- overlapping
 * ..[...dest..[.]..src...]..		-- overlapping
 * The first two can be dealt with by copying from either end of the source
 * block
 * The third (overlapping, source first) by copying from the end of the block
 * back to the start
 * The fourth (overlapping, destination first) by copying from the start of the
 * source block to the end
 *
 * Returns the address of the destination memory area
 */
void *memmove(void *dest, const void *src, unsigned int length)
{
	/* Turn destination and source pointers into integers for easier
	 * calculations
	 */
	register unsigned int d = (unsigned int)dest;
	register unsigned int s = (unsigned int)src;

	if(!length)
		return dest;

	/* Assume the memory blocks are word aligned. Most will be, and the
	 * CPU can deal with unaligned accesses if necessary (as long as it
	 * is configured to do so)
	 */
	if(d>s && d<(s+length))
	{
		/* Destination starts inside source area - work backwards */
		/* If length isn't a multiple of 4 bytes, copy the last 1-3
		 * bytes first
		 */
		while(length & 3)
		{
			length--;
			((unsigned char *)d)[length] = ((unsigned char *)s)[length];
		}

		/* Copy everything else as 32-bit words. If one or both of the
		 * memory areas aren't aligned, this will cause unaligned
		 * reads. Inefficient, but less so than doing everything as
		 * a series of bytes
		 */
		while(length)
		{
			length-=4;
			*((unsigned int *)d+length) = *((unsigned int *)s+length);
		}
	}
	else
	{
		/* Source starts inside destination area - working forwards
		 * is fine - or two areas don't overlap (or they overlap
		 * exactly, but that's an unlikely edge case)
		 *
		 * Copy as much as possible as 32-bit words. See above for
		 * alignment issues
		 */
		while(length & 0xfffffffc)
		{
			*((unsigned int *)d) = *((unsigned int *)s);
			d+=4;
			s+=4;
			length-=4;
		}

		/* Deal with 1-3 remaining bytes, if applicable */
		while(length)
		{
			*((unsigned char *)d) = *((unsigned char *)s);
			d++;
			s++;
			length--;
		}
	}

	return dest;
}

/* Various memory utilities */

/* Clear (set to 0) length bytes of memory starting at address
 */
//extern unsigned int  _GET32 (u32);
//extern unsigned int  _GET16 (u32);
extern void  _PUT32 (u32, u32);
extern void  _PUT16 (u32, u16);

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

void video_init ( void )
{
    static volatile unsigned int FB[] __attribute__((aligned (16))) = {
        FB_W, FB_H,
        FB_W, FB_H,
        0,
        FB_bpp,
        0, 0,
        0, 0
    };
    MailboxWrite(((unsigned int)FB)+0x40000000, 1);

    MailboxRead(1) == 0; // TODO: check?
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

    console_write("1234567890abcdefghijklmnopqrstuvxz\n");
    console_write(BG_GREEN BG_HALF "Reading from tag mailbox\n\n" BG_BLACK);
    console_write(COLOUR_PUSH FG_CYAN "Display resolution: " BG_WHITE BG_HALF BG_HALF);

    char* MEM = (char*)0x8000;
    console_write("Memory: ");
    int i;
    for (i=0; i<16; i++) {
        console_write(tohex(MEM[i],1));
    }
    console_write("\n");

/*
    unsigned int ra,rb;
    rb = FB_PTR;
    for(ra=0;ra<640*480;ra++)
    {
        _PUT32(rb,0x0000FF00);
        rb+=4;
    }
    while(1);
*/
}

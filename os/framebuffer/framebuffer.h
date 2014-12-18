#define PUT8(x,y)  (*((volatile unsigned char*)(x))) = y
#define GET32(x)   (*((volatile unsigned int*)(x)))
#define PUT32(x,y) (*((volatile unsigned int*)(x))) = y

unsigned int MailboxRead (unsigned int channel );
unsigned int MailboxWrite (unsigned int fbinfo_addr, unsigned int channel);
void DrawPixel(unsigned int x, unsigned int y);
void DrawCharacter (char c, unsigned int x, unsigned int y);

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

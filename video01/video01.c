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
    unsigned int ra,rb;

    static volatile unsigned int FB[] __attribute__((aligned (16))) = {
        640, 480,
        640, 480,
        0,
        32,
        0, 0,
        0, 0
    };
    MailboxWrite(((unsigned int)FB)+0x40000000, 1);

    assert(MailboxRead(1) == 0); // TODO: check?

    rb = FB[8];
    for(ra=0;ra<640*480;ra++)
    {
        PUT32(rb,0x000000FF);
        rb+=4;
    }

    return(0);
}

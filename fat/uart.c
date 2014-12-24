#include "uart.h"

#define uint unsigned int
#define u32 volatile unsigned int

#define GPFSEL1         ((u32*)0x20200004)
#define GPSET0          ((u32*)0x2020001C)
#define GPCLR0          ((u32*)0x20200028)
#define GPPUD           ((u32*)0x20200094)
#define GPPUDCLK0       ((u32*)0x20200098)

#define AUX_ENABLES     ((u32*)0x20215004)
#define AUX_MU_IO_REG   ((u32*)0x20215040)
#define AUX_MU_IER_REG  ((u32*)0x20215044)
#define AUX_MU_IIR_REG  ((u32*)0x20215048)
#define AUX_MU_LCR_REG  ((u32*)0x2021504C)
#define AUX_MU_MCR_REG  ((u32*)0x20215050)
#define AUX_MU_LSR_REG  ((u32*)0x20215054)
#define AUX_MU_MSR_REG  ((u32*)0x20215058)
#define AUX_MU_SCRATCH  ((u32*)0x2021505C)
#define AUX_MU_CNTL_REG ((u32*)0x20215060)
#define AUX_MU_STAT_REG ((u32*)0x20215064)
#define AUX_MU_BAUD_REG ((u32*)0x20215068)

void uart_putc (uint c) {
    while (! (*AUX_MU_LSR_REG & 0x20) );
    *AUX_MU_IO_REG = c;
    for (volatile int i=0; i<0x20A; i++);  // TODO: w/o the delay doesnt work
}

void uart_puts (char* str) {
    while (*str != '\0') {
        uart_putc(*str);
        str++;
    }
}

void uart_init (void) {
    *AUX_ENABLES = 1;
    *AUX_MU_IER_REG = 0;
    *AUX_MU_CNTL_REG = 0;
    *AUX_MU_LCR_REG = 3;
    *AUX_MU_MCR_REG = 0;
    *AUX_MU_IER_REG = 0;
    *AUX_MU_IIR_REG = 0xC6;
    *AUX_MU_BAUD_REG = 270;

    uint v = *GPFSEL1;
    v = v & ~(7<<12);       //gpio14
    v = v | (2<<12);        //alt5
    v = v & ~(7<<15);       //gpio15
    v = v | (2<<15);        //alt5
    *GPFSEL1 = v;

    *GPPUD = 0;
    for (volatile int i=0; i<0x10A; i++);
    *GPPUDCLK0 = (1<<14)|(1<<15);
    for (volatile int i=0; i<0x10A; i++);
    *GPPUDCLK0 = 0;

    *AUX_MU_CNTL_REG = 3;

/*
    while (1) {
        while (! (*AUX_MU_LSR_REG & 0x01));
        uint c = *AUX_MU_IO_REG;
        *AUX_MU_IO_REG = c;
        // TODO: putc?
    }
*/
}

void hexstrings ( unsigned int d )
{
    //unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    rb=32;
    while(1)
    {
        rb-=4;
        rc=(d>>rb)&0xF;
        if(rc>9) rc+=0x37; else rc+=0x30;
        uart_putc(rc);
        if(rb==0) break;
    }
    uart_putc(0x20);
}

void hexstring ( unsigned int d )
{
    hexstrings(d);
    uart_putc(0x0D);
    uart_putc(0x0A);
}

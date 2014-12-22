/* Copyright (C) 2013 by John Cronin <jncronin@tysos.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdlib.h>
#include "vfs.h"

void libfs_init();

void kernel_main(void)
{
    // Register the various file systems
	libfs_init();

	// List devices
    vfs_list_devices();

    // Default device
    FILE *f = fopen("/boot/rpi-boot.cfg", "r");
    {
		long flen = fsize(f);
        char *buf = (char *)malloc(flen+1);
		buf[flen] = 0;		// null terminate
		fread(buf, 1, flen, f);
		fclose(f);
        if (buf[0]=='m' &&
            buf[1]=='u' &&
            buf[2]=='l' &&
            buf[3]=='t' &&
            buf[4]=='i') {
#define GPFSEL1 ((unsigned int*)0x20200004)
#define GPSET0  ((unsigned int*)0x2020001C)
#define GPCLR0  ((unsigned int*)0x20200028)
unsigned int ra;
ra = *GPFSEL1;
ra = ra & ~(7<<18);
ra = ra | 1<<18;
*GPFSEL1 = ra;
*GPCLR0 = 1<<16;   // GPIO16 on
while(1);
        }
    }
}

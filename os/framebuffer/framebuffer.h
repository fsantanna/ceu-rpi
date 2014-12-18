#ifndef _FRAMEBUFFER_H
#define _FRAMEBUFFER_H

void FB_init (void);
void FB_draw_pixel (unsigned int x, unsigned int y,
                    unsigned char r, unsigned char g, unsigned char b);

#endif

/*
extern unsigned int InitialiseFrameBuffer(unsigned int width, unsigned int height, unsigned int bitDepth);
extern void SetGraphicsAddress (unsigned int addr);
extern unsigned int  DrawCharacter (char c, unsigned int x, unsigned int y);
extern void DrawString    (char* str, unsigned int length, unsigned int x, unsigned int y);
extern void KeyboardUpdate  (void);
extern char KeyboardGetChar (void);
*/

void notmain (void) {
    unsigned int fb_addr = InitialiseFrameBuffer(1024, 768, 16);
/*
    if (fb_addr == 0) {
        SetGpioFunction(16, 1);
        SetGpio(16, 0);
        while(1);
    }
*/

    SetGraphicsAddress(fb_addr);

    UsbInitialise();

    int x = 100;
    int y = 100;
    while (1) {
        KeyboardUpdate();
        char c = KeyboardGetChar();
        if (c != 0) {
            DrawCharacter(c, x, y);
            x += 8;
        }
    }
}

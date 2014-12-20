int UsbInitialise();
void UsbCheckForChange (void);
int KeyboardCount (void);
unsigned int KeyboardGetAddress (int);
int KeyboardPoll (unsigned int);
unsigned short KeyboardGetKeyDown (unsigned int, int);

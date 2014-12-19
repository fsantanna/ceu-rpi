void UsbCheckForChange (void);
int KeyboardCount (void);
unsigned int KeyboardGetAddress (int);
int KeyboardPoll (unsigned int);
unsigned short KeyboardGetKeyDown (unsigned int, int);

#define KEYS_NBYTES 13

static unsigned int KeyboardAddress = 0;
static char KeyboardOldDown[KEYS_NBYTES] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static char KeyboardNewDown[KEYS_NBYTES] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

static char KeysNormal[] = {
    0x0, 0x0, 0x0, 0x0, 'a', 'b', 'c', 'd',
    'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',
    'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
    'u', 'v', 'w', 'x', 'y', 'z', '1', '2',
    '3', '4', '5', '6', '7', '8', '9', '0',
    '\n', 0x0, '\b', '\t', ' ', '-', '=', '[',
    ']', '\\', '#', ';', '\'', '`', ',', '.',
    '/', 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, '/', '*', '-', '+',
    '\n', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', '0', '.', '\\', 0x0, 0x0, '='
};
	
static char KeysShift[] = {
    0x0, 0x0, 0x0, 0x0, 'A', 'B', 'C', 'D',
    'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',
    'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z', '!', '"',
    '#', '$', '%', '^', '&', '*', '(', ')',
    '\n', 0x0, '\b', '\t', ' ', '_', '+', '{',
    '}', '|', '~', ':', '@', '#', '<', '>',
    '?', 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, '/', '*', '-', '+',
    '\n', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', '0', '.', '|', 0x0, 0x0, '='
};

void KeyboardUpdate () {
    KeyboardAddress = 0;    // TODO
    if (KeyboardAddress == 0) {
        //UsbCheckForChange();
        int n = 0;// KeyboardCount();
        if (n == 0) {
            return;
        }
        //KeyboardAddress = KeyboardGetAddress(0);
        if (KeyboardAddress == 0) {
            return;
        }
    }

    int ret = 0;//KeyboardPoll(KeyboardAddress);
    if (ret != 0) {
        KeyboardAddress = 0;
        return;
    }

    int i;
    for (i=0; i<KEYS_NBYTES; i++) {
        KeyboardNewDown[i] = 0;
    }
    for (i=0; i<6; i++) {
        unsigned short code = 0;//KeyboardGetKeyDown(KeyboardAddress, i);
        if (code != 0) {
            int Byte = code / 8;
            int Bit  = code % 8;
            if (Byte < KEYS_NBYTES) {
                KeyboardNewDown[Byte] |= (1 << Bit);
            }
        }
    }
}

int KeyIsDown (char* keys, unsigned short code) {
    int Byte = code / 8;
    int Bit  = code % 8;
    return keys[Byte] & (1 << Bit);
}

char KeyboardGetChar () {
    char ret = 0;
    int i;
    for (i=0; i<8*KEYS_NBYTES; i++) {
        if (KeyIsDown(KeyboardNewDown,i) && !KeyIsDown(KeyboardOldDown,i)) {
            ret = KeysNormal[i];
            break;
        }
    }

    for (i=0; i<KEYS_NBYTES; i++) {
        KeyboardOldDown[i] = KeyboardNewDown[i];
    }

    return ret;
}

apps = {
    fat = {
        pointer = 0x110000,
    },

    tst4 = {
        pointer = 0x300000,
    },

    lua = {
        pointer = nil,   -- loaded/started from tst4
    },

    led = {
        source = 'led.app',
    },

    uart = {
        source = 'uart.app',
    },

    uart_tst = {
        source = 'uart_tst.app',
    },
}

links = {
    {apps.uart_tst,1  ,  apps.uart,243    },
    {apps.uart,    1  ,  apps.uart_tst,243},
}

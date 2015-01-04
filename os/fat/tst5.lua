apps = {
    fat = {
        pointer = nil,  -- TODO
    },

    lua = {
        pointer = nil,  -- TODO
    },

    tst5 = {
        pointer = nil,  -- TODO
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

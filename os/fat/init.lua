apps = {
    fat = {
        -- preloaded
    },

    lua = {
        -- preloaded
    },

    init = {
        -- preloaded
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
    {apps.uart_tst,'SEND'     ,  apps.uart,    'SEND'   },
    {apps.uart,    'RECEIVE'  ,  apps.uart_tst,'RECEIVE'},
}

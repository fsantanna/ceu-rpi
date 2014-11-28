.globl _start
_start:
    mov sp,#0x8000
    bl ceu_os_main
hang: b hang

.globl _PUT8
_PUT8:
    strb r1,[r0]
    bx lr

.globl _PUT16
_PUT16:
    strh r1,[r0]
    bx lr

.globl _PUT32
_PUT32:
    str r1,[r0]
    bx lr

.globl _GET32
_GET32:
    ldr r0,[r0]
    bx lr

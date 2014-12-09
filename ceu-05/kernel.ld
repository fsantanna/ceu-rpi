SECTIONS {
    .init 0x8000 : {
		*(.init)
	}

    .data : {
		*(.data)
		*.c.o(*)
	}

    .rodata : {
        *(.rodata)
		*.c.o(*)
	}

    .text : {
		*.c.o(.text)
		*(.text)
	}

    .bss : {
        *.c.o(.bss)
        *(.bss)
	}

    /DISCARD/ : {
		*(*)
	}
}

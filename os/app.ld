SECTIONS {
    .export 0x00 : {
        *(.export)
	}

    .rodata : {
        *(.rodata)
		*.c.o(*)
	}

    .text : {
		*.c.o(.text)
		*(.text)
	}

    .data : {
		*(.data)
		*.c.o(*)
	}

/*
    .bss : {
        *.c.o(.bss)
        *(.bss)
	}

    /DISCARD/ : {
		*(*)
	}
*/
}

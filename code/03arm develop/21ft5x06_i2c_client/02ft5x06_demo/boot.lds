SECTIONS {
	. = 0x40000000;

	.text : {
		start.o(.text);
		*(.text);
	}
	. = ALIGN(4);

	.rodata : {
		*(.rodata);
	}
	. = ALIGN(4);

	.data : {
		*(.data);
	}
	. = ALIGN(4);

	_initcall_start = .;
	.initcall : {
		*(.initcall);
	}
	. = ALIGN(4);
	_initcall_end = .;

	_bss_start = .;
	.bss : {
		*(.bss);
	}
	. = ALIGN(4);
	_bss_end = .;
}

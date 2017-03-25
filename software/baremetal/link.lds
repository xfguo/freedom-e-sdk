OUTPUT_ARCH("riscv")
ENTRY(_start)

MEMORY
{
	ram (rwx) : ORIGIN = 0x80000000, LENGTH = 8K
}

SECTIONS
{
	.init : {
		. = ALIGN(8);
		*(.init)
	} > ram

	.text : {
		. = ALIGN(8);
		*(.text)
	} > ram

	.data : {
		. = ALIGN(8);
		*(.data)

		. = ALIGN(8);
		. += 1024;
		_stack = .;
	} > ram
}

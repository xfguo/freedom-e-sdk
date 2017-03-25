.section .text
.global get_hartid
get_hartid:
	csrr a0, mhartid
	ret

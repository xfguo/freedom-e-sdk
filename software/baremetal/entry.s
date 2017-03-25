.section .init
.global _start
_start:
	la t0, trapvec
	csrw mtvec, t0

	csrr t0, mhartid
	bnez t0, wait_a_bit

	la t0, lock
	sw zero, 0(t0)

	la t0, uart_inited
	sw zero, 0(t0)

	fence

	li t0, 1000000
wait_a_bit:
	addi t0, t0, -1
	bne t0, x0, wait_a_bit

c:
	la sp, _stack
	csrr t0, mhartid
	slli t0, t0, 8
	sub sp, sp, t0

	call main
	tail exit

.global exit
	nop
	nop
	nop
exit:
	nop
	nop
	nop
	j exit

.global trapvec
	nop
	nop
	nop
trapvec:
	nop
	nop
	nop
	j trapvec

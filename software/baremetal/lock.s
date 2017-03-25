.section .text
.global get_lock
get_lock:
	li t0, 1
again:
	amoswap.w.aq t0, t0, (a0)
	bnez t0, again
	ret

.global put_lock
put_lock:
	amoswap.w.rl x0, x0, (a0)
	ret

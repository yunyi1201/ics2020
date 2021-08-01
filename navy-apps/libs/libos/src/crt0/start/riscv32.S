.globl  _start
_start:
  move s0, zero
	mv   sp, a0
	mv   s0, sp
	mv   a0, zero
  jal call_main

.globl  _start
_start:
  move s0, zero
	mv   sp, a0
	mv   s0, sp
  jal call_main

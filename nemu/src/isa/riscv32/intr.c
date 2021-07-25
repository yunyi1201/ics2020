#include <cpu/exec.h>
#include "local-include/rtl.h"

void raise_intr(DecodeExecState *s, word_t NO, vaddr_t epc) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
  */
	set_sepc(cpu.pc);
	set_scause(NO);
	s->is_jmp = true;
	s->jmp_pc = epc;
}

void query_intr(DecodeExecState *s) {
}

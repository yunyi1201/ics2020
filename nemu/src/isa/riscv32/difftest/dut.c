#include <isa.h>
#include <monitor/difftest.h>
#include "../local-include/reg.h"
#include "difftest.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
	int i;	
	for(i=0; i<32; i++)
		if(ref_r->gpr[i]._32 != cpu.gpr[i]._32)
			return false;
	
  return ref_r->pc == cpu.pc;
}

void isa_difftest_attach() {
}

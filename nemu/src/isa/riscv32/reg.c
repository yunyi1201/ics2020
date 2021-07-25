#include <isa.h>
#include "local-include/reg.h"

const char *regs[] = {
  "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
  "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
  "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
  "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};

const char *csr_regs[] = {
	"sstatus", "stvec", "sscratch",
	"sepc"  , "scause", "stval",
	"satp"
};

void isa_reg_display() {
	for(int i=0; i<32; i++) {
		printf("%s 0x%x %u\n", reg_name(i), reg_l(i), reg_l(i));
	}
	printf("pc 0x%x %u\n", cpu.pc, cpu.pc);
	for(int j=0; j<7; j++) {
		printf("%s 0x%x %u\n", csr_reg_name(j), reg_csr(j), reg_csr(j));
	}
}

word_t isa_reg_str2val(const char *s, bool *success) {
	if(strcmp(s+1, "pc") == 0){
		return cpu.pc;
	}
	int i;
	for(i=0; i<32; i++){
		if(strcmp(s+1, reg_name(i)) == 0)
			return reg_l(i);
	}
	for(i=0; i<32; i++){
		if(strcmp(s+1, csr_reg_name(i)) == 0)
			return reg_csr(i);
	}
	*success = false;
  return 0;
}

#ifndef __RISCV32_REG_H__
#define __RISCV32_REG_H__

#include <common.h>

enum { sstatus, stvec, sscratch, sepc, scause, stval, stap };

static inline int check_reg_index(int index) {
  assert(index >= 0 && index < 32);
  return index;
}

static inline int check_csr_reg_index(int index) {
	assert(index >= 0 && index < 7);
	return index;
}

#define reg_l(index)   (cpu.gpr[check_reg_index(index)]._32)
#define reg_csr(index) (cpu.csr[check_csr_reg_index(index)]._32)

static inline word_t map_csr_addr(word_t addr) {
	assert(addr >= 0x100 && addr <= 0x180);
	switch(addr) {
		case 0x100: return 0;	
		case 0x105: return 1;
		case 0x140: return 2;
		case 0x141: return 3;
		case 0x142: return 4;
		case 0x143: return 5;
		case 0x180: return 6;
		default: Assert(0, "error:0x%x", addr);
	}
	return 0;
}


static inline const char* reg_name(int index) {
  extern const char* regs[];
  assert(index >= 0 && index < 32);
  return regs[index];
}

static inline const char* csr_reg_name(int index) {
	extern const char* csr_regs[];
	assert(index >=0 && index < 7);
	return csr_regs[index];
}

static inline void set_sepc(word_t val) {
	cpu.csr[sepc]._32 = val;
}

static inline void set_scause(word_t val) {
	cpu.csr[scause]._32 = val;
}


#endif

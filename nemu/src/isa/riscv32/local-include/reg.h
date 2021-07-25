#ifndef __RISCV32_REG_H__
#define __RISCV32_REG_H__

#include <common.h>

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

#endif

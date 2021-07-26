#ifndef ARCH_H__
#define ARCH_H__

struct Context {
	//void *pdir;
  uintptr_t gpr[32], cause, status, epc;
	void *pdir;
};

#define GPR1 gpr[17] // a7
#define GPR2 gpr[10] // argument 
#define GPR3 gpr[11] // argument
#define GPR4 gpr[12]
#define GPRx gpr[10] //return 

#endif

#include <common.h>
#include "syscall.h"

int sys_yield() {
	yield();
	return -1;
}

int sys_exit(int status) {
	halt(0);
	return status;
}

void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;

  switch (a[0]) {
		case SYS_exit: 	c->GPRx = sys_exit(c->GPR2);  break; 
		case SYS_yield: c->GPRx = sys_yield(); break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}

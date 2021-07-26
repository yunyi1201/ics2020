#include <common.h>
#include "syscall.h"

int sys_yield() {
	yield();
	return -1;
}

int sys_exit() {
	halt(0);
}
void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;

  switch (a[0]) {
		case 0: 	c->GPRx = sys_exit();  break; 
		case 1: c->GPRx = sys_yield(); break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}

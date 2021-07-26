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

int sys_write(int fd, void* buf, size_t len) {

	//Log("sys_write");
	//Log("time: %s, %s", __TIME__, __DATE__);
	for(int i=0; i<len; i++) 
		putch(((char*)buf)[i]);
	return len;
}

void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;

  switch (a[0]) {
			case SYS_exit: 	c->GPRx = sys_exit(c->GPR2);  break; 
			case SYS_yield: c->GPRx = sys_yield(); break;
			case SYS_write: assert(c->GPR2 == 1 || c->GPR2 == 2); c->GPRx = sys_write(c->GPR2, (void*)c->GPR3, c->GPR4); break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}

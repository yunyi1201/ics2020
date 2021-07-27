#include <common.h>
#include "syscall.h"
#include <fs.h>

int sys_yield() {
	yield();
	return -1;
}

int sys_exit(int status) {
	halt(0);
	return status;
}

size_t sys_write(int fd, void* buf, size_t len) {

	//Log("sys_write");
	//Log("time: %s, %s", __TIME__, __DATE__);
	return fs_write(fd, buf, len);
}

size_t sys_read(int fd, void *buf, size_t len) {
	return fs_read(fd, buf, len);
}

size_t sys_lseek(int fd, int offset, int whence) {
	return fs_lseek(fd, offset, whence);
}

int sys_open(const char* filename, int flag, int mode) {
	return fs_open(filename, flag, mode);
}

int sys_close(int fd) {
	return fs_close(fd);
}

int sys_brk(void *addr) {
	return 0;
}

void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;

  switch (a[0]) {
			case SYS_exit: 	c->GPRx = sys_exit(c->GPR2);  break; 
			case SYS_yield: c->GPRx = sys_yield(); break;
			case SYS_write: c->GPRx = sys_write(c->GPR2, (void*)c->GPR3, c->GPR4); break;
			case SYS_read:  c->GPRx = sys_read(c->GPR2, (void*)c->GPR3, c->GPR4); break;
			case SYS_open:  c->GPRx = sys_open((const char*)c->GPR2, c->GPR3, c->GPR4); break;
			case SYS_lseek: c->GPRx = sys_lseek(c->GPR2, c->GPR3, c->GPR4); break;
			case SYS_close: c->GPRx = sys_close(c->GPR2); break;
			case SYS_brk:   c->GPRx = sys_brk((void*)c->GPR2); break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}

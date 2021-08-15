#include <common.h>
#include "syscall.h"
#include <fs.h>
#include <proc.h>
#include <sys/time.h>

#define RTC_ADDR 0xa1000048

uint32_t inl(uintptr_t addr) { return *(volatile uint32_t *)addr; }

extern void naive_uload(PCB * pcb, const char *filename);
extern void context_uload(PCB *pcb, const char *fname, char *const argv[], char *const envp[]);
extern void switch_boot_pcb();


int sys_yield() {
	yield();
	return -1;
}

int sys_exit(int status) {
	//halt(0);
	PCB* pcb = find_free_pcb();
	char *argv[] = {"/bin/nterm", NULL};
	char *environ[] = {NULL };

	context_uload(pcb, "/bin/nterm", argv, environ);
	//naive_uload(NULL, "/bin/nterm");
	return status;
}

size_t sys_write(int fd, void* buf, size_t len) {
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
	// BUG 
	return 0;
}

int sys_gettimeofday(struct timeval *tv, struct timezone *tz) {
	tv->tv_sec  =  inl(RTC_ADDR + 4);
	tv->tv_usec =  inl(RTC_ADDR);
	return 0;
}

int sys_execve(const char *fname, char * const argv[], char *const envp[]) {
	if(fs_open(fname, 0, 0) < 0)
		return -2;
	PCB* pcb = find_free_pcb();
	context_uload(pcb, fname, argv, envp);
	switch_boot_pcb();
	yield();
	return -1;
}

void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
	a[1] = c->GPR2;
	a[2] = c->GPR3;
	a[3] = c->GPR4;
  switch (a[0]) {
			case SYS_exit: 	c->GPRx = sys_exit(a[1]);  break; 
			case SYS_yield: c->GPRx = sys_yield(); break;
			case SYS_write: c->GPRx = sys_write(c->GPR2, (void*)c->GPR3, c->GPR4); break;
			case SYS_read:  c->GPRx = sys_read(c->GPR2, (void*)c->GPR3, c->GPR4); break;
			case SYS_open:  c->GPRx = sys_open((const char*)c->GPR2, c->GPR3, c->GPR4); break;
			case SYS_lseek: c->GPRx = sys_lseek(c->GPR2, c->GPR3, c->GPR4); break;
			case SYS_close: c->GPRx = sys_close(c->GPR2); break;
			case SYS_brk:   c->GPRx = sys_brk((void*)c->GPR2); break;
			case SYS_gettimeofday: c->GPRx = sys_gettimeofday((struct timeval*)(a[1]), (struct timezone *)(a[2])); break;
			case SYS_execve:  c->GPRx = sys_execve((const char *)(a[1]), (void *)(a[2]), (void *)a[3]); break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}

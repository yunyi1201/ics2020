#include <proc.h>
#include <am.h>

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;

void switch_boot_pcb() {
  current = &pcb_boot;
}

void hello_fun(void *arg) {
  int j = 1;
  while (1) {
    Log("Hello World from Nanos-lite with arg '%s' for the %dth time!", (char *)arg, j);
    j ++;
    yield();
  }
}

void init_proc() {
	
	extern void context_kload(PCB *, void *, void *);
	extern void context_uload(PCB *, const char *);
	context_uload(&pcb[0], "/bin/menu");

	context_uload(&pcb[1], "/bin/bird");

  switch_boot_pcb();
	//extern void naive_uload(PCB*, const char*);
  Log("Initializing processes...");
  // load program here
}


Context* schedule(Context *prev) {
	current->cp = prev;
	current = (current == &pcb[0] ? &pcb[1] : &pcb[0]);
  return current->cp;
}

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
    Log("Hello World from Nanos-lite with arg '%p' for the %dth time!", (uintptr_t)arg, j);
    j ++;
    yield();
  }
}

void init_proc() {
	
	extern void context_kload(PCB *, void *, void *);
	context_kload(&pcb[0], hello_fun, NULL);
  switch_boot_pcb();
	//extern void naive_uload(PCB*, const char*);
  Log("Initializing processes...");
	//naive_uload(NULL, "/bin/nterm");
  // load program here
}


Context* schedule(Context *prev) {
	current->cp = prev;
	current = &pcb[0];
  return current->cp;
}

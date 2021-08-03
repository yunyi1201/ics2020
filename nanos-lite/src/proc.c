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
		if(j % 10000 == 0) {
    	Log("Hello World from Nanos-lite with arg '%s' for the %dth time!", (char *)arg, j);
			j = 0;
		}
		j++;
    yield();
  }
}

void init_proc() {
	
	Log("Start proc init");
	extern void context_kload(PCB *, void *, void *);
	extern void context_uload(PCB *, const char *, char *const argv[], char *const envp[]);
	context_kload(&pcb[0], hello_fun, "A");

	context_uload(&pcb[1], "/bin/nterm", NULL, NULL);
	

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

PCB* find_free_pcb() {
	for(int i=0; i<MAX_NR_PROC; i++) {
		if(pcb[i].cp != 0) 
			return &pcb[i];
	}
	panic("Should not reach there");
}

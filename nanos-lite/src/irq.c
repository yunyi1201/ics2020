#include <common.h>
#include <proc.h>

static Context* do_event(Event e, Context* c) {
  //switch (e.event) {
  //  default: panic("Unhandled event ID = %d", e.event);
 // }
	if(e.event == EVENT_YIELD) {
		//c = schedule(c);
		printf("Tirger yield event ID = %d\n", e.event);
	}
	else if(e.event == EVENT_SYSCALL) {
		extern void do_syscall(Context*);
		do_syscall(c);
	}
	else 
		panic("Unhandled event ID = %d", e.event);
  return c;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  cte_init(do_event);
}

#include <am.h>
#include <riscv32.h>
#include <klib.h>

static Context* (*user_handler)(Event, Context*) = NULL;

Context* __am_irq_handle(Context *c) {
  if (user_handler) {
    Event ev = {0};
		if(c->cause == -1) {
			ev.event = EVENT_YIELD; 
			c->epc += 4;
		} else if(c->cause >=0 && c->cause < 20) {
			ev.event = EVENT_SYSCALL;
			c->epc += 4;
		}
		else {
			ev.event = EVENT_ERROR;
		}

    c = user_handler(ev, c);      // 切换进程
    assert(c != NULL);
  }

  return c;
}

extern void __am_asm_trap(void);


bool cte_init(Context*(*handler)(Event, Context*)) {
  // initialize exception entry
  asm volatile("csrw stvec, %0" : : "r"(__am_asm_trap));

  // register event handler
  user_handler = handler;

  return true;
}

Context *kcontext(Area kstack, void (*entry)(void *), void *arg) {
	Context* cp = (Context *)kstack.end - 1;
	memset((void *)cp, 0, sizeof(Context));
	cp->epc = (uintptr_t)entry;
	cp->status = 0xc0100;
	cp->gpr[8]  = (uintptr_t)kstack.end;
	cp->gpr[2]  = (uintptr_t)kstack.end;
	// TODO: pass arg
  return cp;
}

void yield() {
  asm volatile("li a7, -1; ecall");
}

bool ienabled() {
  return false;
}

void iset(bool enable) {
}

#include <cpu/exec.h>
#include "../local-include/decode.h"
#include "all-instr.h"

static inline void set_width(DecodeExecState *s, int width) {
  if (width != 0) s->width = width;
}

static inline def_EHelper(load) {
  switch (s->isa.instr.i.funct3) {
    EXW  (2, ld, 4)
    default: exec_inv(s);
  }
}

static inline def_EHelper(store) {
  switch (s->isa.instr.s.funct3) {
    EXW  (2, st, 4)
    default: exec_inv(s);
  }
}

static inline def_EHelper(compute) {
	switch(s->isa.instr.s.funct3){
		EX(0, addi) 
		EX(1, slli)
		EX(2, slti)
		EX(3, sltiu)
		EX(4, xori)
		EX(5, srli)
		EX(6, ori)
		EX(7, andi)
		default: exec_inv(s);
	}
}

static inline def_EHelper(rrcompute) {
	switch(s->isa.instr.r.funct3){
		EX(0, add)
		EX(1, sll)
		EX(2, slt)
		EX(3, sltu)
		EX(4, xor)
		EX(5, srl)
		EX(6, or)
		EX(7, and)
		default: exec_inv(s);
	}
}

static inline void fetch_decode_exec(DecodeExecState *s) {
  s->isa.instr.val = instr_fetch(&s->seq_pc, 4);
  Assert(s->isa.instr.i.opcode1_0 == 0x3, "Invalid instruction");
  switch (s->isa.instr.i.opcode6_2) {
    IDEX (0b00000, I, load)
    IDEX (0b01000, S, store)
    IDEX (0b01101, U, lui)
		IDEX (0b00101, U, auipc)
		IDEX (0b00100, I, compute)	
		IDEX (0b11011, J, jal)
		IDEX (0b11001, I, jarl)
		IDEX (0b01100, R, rrcompute)
		EX   (0b11010, nemu_trap)
    default: exec_inv(s);
  }
}

static inline void reset_zero() {
  reg_l(0) = 0;
}

vaddr_t isa_exec_once() {
  DecodeExecState s;
  s.is_jmp = 0;
  s.seq_pc = cpu.pc;

  fetch_decode_exec(&s);
  update_pc(&s);

  reset_zero();

  return s.seq_pc;
}

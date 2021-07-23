static inline def_EHelper(jal) {
	rtl_addi(s, ddest, rz, s->seq_pc);
	rtl_mv(s, s0, &(cpu.pc));
	rtl_addi(s, s0, s0, id_src1->imm);
	rtl_jr(s, s0);
	print_asm_template2(jal);
}

static inline def_EHelper(jarl) {
	rtl_addi(s, dsrc1, dsrc1, id_src2->imm);
	vaddr_t vaddr = (*dsrc1) & (~1);
	*ddest = s->seq_pc;
	rtl_j(s, vaddr);
	print_asm_template2(jarl);
}

static inline def_EHelper(beq) {
	if(*dsrc1 == *dsrc2){
		vaddr_t vaddr = cpu.pc + id_dest->imm;
		rtl_j(s, vaddr);
	}
}

static inline def_EHelper(bne) {
	if(*dsrc1 != *dsrc2){
		vaddr_t vaddr = cpu.pc + id_dest->imm;
		rtl_j(s, vaddr);
	}
}

static inline def_EHelper(blt) {
	if((int32_t)*dsrc1 < (int32_t)*dsrc2){
		vaddr_t vaddr = cpu.pc + id_dest->imm;
		rtl_j(s, vaddr);
	}
}

static inline def_EHelper(bge) {
	if((int32_t)*dsrc1 >= (int32_t)*dsrc2){
		vaddr_t vaddr = cpu.pc + id_dest->imm;
		rtl_j(s, vaddr);
	}
}

static inline def_EHelper(bltu) {
	if(*dsrc1 < *dsrc2){
		vaddr_t vaddr = cpu.pc + id_dest->imm;
		rtl_j(s, vaddr);
	}
}

static inline def_EHelper(bgeu) {
	if(*dsrc1 >= *dsrc2){
		vaddr_t vaddr = cpu.pc + id_dest->imm;
		rtl_j(s, vaddr);
	}
}

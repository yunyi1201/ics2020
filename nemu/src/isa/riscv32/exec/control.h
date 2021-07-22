static inline def_EHelper(jal) {
	*ddest = s->seq_pc;
	vaddr_t vaddr = cpu.pc + id_src1->imm; 
	rtl_j(s, vaddr);
	print_asm_template2(jal);
}

static inline def_EHelper(jarl) {
	vaddr_t vaddr = (*dsrc1 + id_src2->imm) & (~1);
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

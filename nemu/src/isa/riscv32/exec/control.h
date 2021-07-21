static inline def_EHelper(jal) {
	*ddest = s->seq_pc;
	vaddr_t vaddr = cpu.pc + id_src1->imm; 
	rtl_j(s, vaddr);
}

static inline def_EHelper(jarl) {
	vaddr_t vaddr = (*dsrc1 + id_src2->imm) & (~1);
	*ddest = s->seq_pc;
	rtl_j(s, vaddr);
}

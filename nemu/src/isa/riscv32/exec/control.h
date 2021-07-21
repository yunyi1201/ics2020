static inline def_EHelper(jal) {
	*ddest = s->seq_pc + 4;
	vaddr_t vaddr = s->seq_pc + id_src1->imm; 
	rtl_j(s, vaddr);
}

static inline def_EHelper(jarl) {
	vaddr_t vaddr = (*dsrc1 + id_src2->imm) & (~1);
	*ddest = s->seq_pc + 4;
	rtl_j(s, vaddr);
}

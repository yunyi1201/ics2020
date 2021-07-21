static inline def_EHelper(lui) {
  rtl_li(s, ddest, id_src1->imm);
  print_asm_template2(lui);
}

static inline def_EHelper(addi) {
	rtl_addi(s, ddest, dsrc1, id_src2->imm);
	print_asm_template2(addi);
}

static inline def_EHelper(auipc) {	
	(*ddest) = s->seq_pc + id_src1->imm;
	print_asm_template2(auipc);
}

static inline def_EHelper(jal) {
	*ddest = s->seq_pc + 4;
	vaddr_t vaddr = s->seq_pc + id_src1->imm; 
	rtl_j(s, vaddr);
}

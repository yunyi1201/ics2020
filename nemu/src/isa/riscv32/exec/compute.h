static inline def_EHelper(lui) {
  rtl_li(s, ddest, id_src1->imm);
  print_asm_template2(lui);
}

static inline def_EHelper(li){
	rtl_addi(s, ddest, rz, id_src2->imm);   	 
}

static inline def_EHelper(auipc){	
	(*ddest) = s->seq_pc + id_src1->imm;
	print_asm_template2(auipc);
}

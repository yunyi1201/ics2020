static inline def_EHelper(lui) {
  rtl_li(s, ddest, id_src1->imm);
  print_asm_template2(lui);
}

static inline def_EHelper(addi) {
	rtl_addi(s, ddest, dsrc1, id_src2->imm);
	print_asm_template2(addi);
}

static inline def_EHelper(slti) {
	if((int32_t)(*dsrc1) < (int32_t)id_src2->imm)
		*ddest = 1;
	else 
		*ddest = 0;
	print_asm_template3(slti);
}

static inline def_EHelper(xori) {
	rtl_xori(s, ddest, dsrc1, id_src2->imm);
	print_asm_template3(xori);
}

static inline def_EHelper(ori) {
	rtl_ori(s, ddest, dsrc1, id_src2->imm);
	print_asm_template3(ori);
}

static inline def_EHelper(andi) {
	rtl_andi(s, ddest, dsrc1, id_src2->imm);
	print_asm_template3(andi);
}

static inline def_EHelper(auipc) {	
	(*ddest) = cpu.pc + id_src1->imm;
	print_asm_template2(auipc);
}

static inline def_EHelper(slli) {
	rtl_shli(s, ddest, dsrc1, id_src2->imm);
	print_asm_template3(slli);
}

static inline def_EHelper(srli) {
	switch(id_src2->imm & 0x400) {
		case 0: rtl_shri(s, ddest, dsrc1, id_src2->imm);
		case 1: rtl_sari(s, ddest, dsrc1, id_src2->imm);	
	}
	print_asm_template3(srli);
}



static inline def_EHelper(lui) {
  rtl_li(s, ddest, id_src1->imm);
  print_asm_template2(lui);
}

static inline def_EHelper(addi) {
	rtl_addi(s, ddest, dsrc1, id_src2->imm);
	print_asm_template3(addi);
}

static inline def_EHelper(add) {
	switch(s->isa.instr.r.funct7 & 0x20){
		case 0: rtl_add(s, ddest, dsrc1, dsrc2); print_asm_template3(add); break;
		default: rtl_sub(s, ddest, dsrc1, dsrc2); print_asm_template3(sub); break;
	}
}

static inline def_EHelper(slti) {
	rtl_setrelopi(s, RELOP_LT, ddest, dsrc1, id_src2->imm);
	print_asm_template3(slti);
}

static inline def_EHelper(slt) {
	rtl_setrelop(s, RELOP_LT, ddest, dsrc1, dsrc2);
	print_asm_template3(slt);
}

static inline def_EHelper(xori) {
	rtl_xori(s, ddest, dsrc1, id_src2->imm);
	print_asm_template3(xori);
}

static inline def_EHelper(xor) {
	rtl_xor(s, ddest, dsrc1, dsrc2);
	print_asm_template3(xor);
}

static inline def_EHelper(ori) {
	rtl_ori(s, ddest, dsrc1, id_src2->imm);
	print_asm_template3(ori);
}

static inline def_EHelper(or) {
	rtl_or(s, ddest, dsrc1, dsrc2);
	print_asm_template3(or);
}

static inline def_EHelper(andi) {
	rtl_andi(s, ddest, dsrc1, id_src2->imm);
	print_asm_template3(andi);
}

static inline def_EHelper(and) {
	rtl_and(s, ddest, dsrc1, dsrc2);
	print_asm_template3(and);
}

static inline def_EHelper(auipc) {	
	(*ddest) = cpu.pc + id_src1->imm;
	print_asm_template2(auipc);
}

static inline def_EHelper(slli) {
	rtl_shli(s, ddest, dsrc1, id_src2->imm);
	print_asm_template3(slli);
}

static inline def_EHelper(sll) {
	rtl_shl(s, ddest, dsrc1, dsrc2);	
	print_asm_template3(sll);
}

static inline def_EHelper(srli) {
	switch(id_src2->imm & 0x400) {
		case 0: rtl_shri(s, ddest, dsrc1, id_src2->imm); break;
		default: rtl_sari(s, ddest, dsrc1, id_src2->imm); break;	
	}
	print_asm_template3(srli);
}

static inline def_EHelper(srl) {
	switch(s->isa.instr.r.funct7 & 0x20){
		case 0: rtl_shr(s, ddest, dsrc1, dsrc2); break;
		default: rtl_sar(s, ddest, dsrc1, dsrc2); break; 
	}
	print_asm_template3(srl);
}

static inline def_EHelper(sltiu) {
	rtl_setrelopi(s, RELOP_LTU, ddest, dsrc1, id_src2->imm);
	print_asm_template3(sltiu);
}

static inline def_EHelper(sltu) {
	rtl_setrelop(s, RELOP_LTU, ddest, dsrc1, dsrc2);
	print_asm_template3(sltu);
}

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
	rtl_addi(s, ddest, rz, s->seq_pc);
	rtl_j(s, vaddr);
	print_asm_template2(jarl);
}

static inline def_EHelper(beq) {
	rtl_mv(s, s0, &(cpu.pc));
	rtl_addi(s, s0, s0, id_dest->imm);
	rtl_jrelop(s, RELOP_EQ, dsrc1, dsrc2, s0);
	print_asm_template3(beq);
}

static inline def_EHelper(bne) {
	rtl_mv(s, s0, &(cpu.pc));
	rtl_addi(s, s0, s0, id_dest->imm);
	rtl_jrelop(s, RELOP_NE, dsrc1, dsrc2, s0);
	print_asm_template3(bne);
}

static inline def_EHelper(blt) {
	rtl_mv(s, s0, &(cpu.pc));
	rtl_addi(s, s0, s0, id_dest->imm);
	rtl_jrelop(s, RELOP_LT, dsrc1, dsrc2, s0);
	print_asm_template3(blt);
}

static inline def_EHelper(bge) {
	rtl_mv(s, s0, &(cpu.pc));
	rtl_addi(s, s0, s0, id_dest->imm);
	rtl_jrelop(s, RELOP_GE, dsrc1, dsrc2, s0);
	print_asm_template3(bge);
}

static inline def_EHelper(bltu) {
	rtl_mv(s, s0, &(cpu.pc));
	rtl_addi(s, s0, s0, id_dest->imm);
	rtl_jrelop(s, RELOP_LTU, dsrc1, dsrc2, s0);
	print_asm_template3(bltu);
}

static inline def_EHelper(bgeu) {
	rtl_mv(s, s0, &(cpu.pc));
	rtl_addi(s, s0, s0, id_dest->imm);
	rtl_jrelop(s, RELOP_GEU, dsrc1, dsrc2, s0);
	print_asm_template3(bgeu);
}

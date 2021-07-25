static inline def_EHelper(csrrw) {
	rtl_mv(s, s0, dsrc2);
	rtl_mv(s, dsrc2, dsrc1);
	rtl_mv(s, ddest, s0);
	print_asm_template3(csrrw);
}

static inline def_EHelper(csrrs) {
	rtl_mv(s, s0, dsrc2);
	rtl_or(s, dsrc2, s0, dsrc1);
	rtl_mv(s, ddest, s0);
	print_asm_template3(csrrs);
}

static inline def_EHelper(csrrc) {
	rtl_mv(s, s0, dsrc2);
	//rtl_mv(s, dsrc2, dsrc1);
	*dsrc2 = *s0 & (~*dsrc1);
	rtl_mv(s, ddest, s0);
	print_asm_template3(csrrc);
}

static inline def_EHelper(csrrwi) {
	TODO();
}

static inline def_EHelper(cssrrsi) {
	TODO();
}

static inline def_EHelper(csrrci) {
	TODO();
}

static inline def_EHelper(ecall) {			
	bool success = true;
	word_t stvec  = isa_reg_str2val("stvec", &success);
	word_t scause = isa_reg_str2val("a7", &success);
	extern void raise_intr(DecodeExecState*, word_t, vaddr_t);
	print_asm_template1(ecall);
	raise_intr(s, scause, stvec);
}

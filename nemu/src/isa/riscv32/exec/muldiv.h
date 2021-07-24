static inline def_EHelper(mul) {
	rtl_mul_lo(s, ddest, dsrc1, dsrc2);
	print_asm_template3(mul);
}

static inline def_EHelper(mulh) {
	rtl_imul_hi(s, ddest, dsrc1, dsrc2);
	print_asm_template3(mulh);
}

static inline def_EHelper(mulhsu) {
	rtl_iumul_hi(s, ddest, dsrc1, dsrc2);
	print_asm_template3(mulhsu);
}

static inline def_EHelper(mulhu) {
	rtl_mul_hi(s, ddest, dsrc1, dsrc2);
	print_asm_template3(mulhu);
}

static inline def_EHelper(div) {
	rtl_idiv_q(s, ddest, dsrc1, dsrc2);
	print_asm_template3(div);
}

static inline def_EHelper(divu) {
	rtl_div_q(s, ddest, dsrc1, dsrc2);
	print_asm_template3(divu);
}

static inline def_EHelper(rem) {
	rtl_idiv_r(s, ddest, dsrc1, dsrc2);
	print_asm_template3(rem);
}

static inline def_EHelper(remu) {
	rtl_div_r(s, ddest, dsrc1, dsrc2);
	print_asm_template3(remu);
}

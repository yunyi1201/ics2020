#include <isa.h>
#include "local-include/reg.h"
#include "string.h"
const char *regsl[] = {
  "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
  "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
  "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
  "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};

void isa_reg_display() {
	int i = 0;
	for( i=0; i<32; i++ ) {
		
		printf("%s: %08x\n",reg_name(i,32),reg_l(i));
		
	}
	printf("pc: %08x\n",cpu.pc);
}

word_t isa_reg_str2val(const char *s, bool *success) { 

  int index = -1;
  for( int i=0; i<32; i++ ) {
	if( strcmp( s, regsl[i]) == 0 )
		index = i;	
 } 
  if( index == -1 )
  	*success =  false;
  else 
	return reg_l(index);	
  return 0;
}

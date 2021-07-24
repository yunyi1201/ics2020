#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
	uint32_t key = inl(KBD_ADDR);
	if(key >= KEYDOWN_MASK) {
		kbd->keydown = 1;
		kbd->keycode = key ^ KEYDOWN_MASK;
	} else if(key != 0){
		kbd->keydown = 0;
		kbd->keycode = key;
	} else {
		kbd->keycode = 0;
		kbd->keydown = 0;
	}
}

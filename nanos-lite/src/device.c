#include <common.h>

#if defined(MULTIPROGRAM) && !defined(TIME_SHARING)
# define MULTIPROGRAM_YIELD() yield()
#else
# define MULTIPROGRAM_YIELD()
#endif

#define NAME(key) \
  [AM_KEY_##key] = #key,

#define KEYDOWN_MASK 0x8000
#define KBD_ADDR     0xa1000060
static const char *keyname[256] __attribute__((used)) = {
  [AM_KEY_NONE] = "NONE",
  AM_KEYS(NAME)
};

static uint32_t inl(uintptr_t addr) { return *(volatile uint32_t *)addr; }

size_t serial_write(const void *buf, size_t offset, size_t len) {
	for(int i=0; i<len; i++)
		putch(((const char* )buf)[i]);
  return len;
}

size_t events_read(void *buf, size_t offset, size_t len) {
	AM_INPUT_KEYBRD_T kbd;
	uint32_t key = inl(KBD_ADDR);
	size_t ret;
	if(key >= KEYDOWN_MASK) {
		kbd->keydown = 1;
		kbd->keycode = key ^ KEYDOWN_MASK;
		ret = sprintf(buf, "%s %s\n", "kd", keyname[kbd.keycode]);
	} else if(key != 0) {
		kbd->keydown = 0;
		kbd->keycode = key;
		ret = sprintf(buf, "%s %s\n", "ku", keyname[kbd.keycode]);	
	} else
		return 0;
	return 0;
}

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  return 0;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  return 0;
}

void init_device() {
  Log("Initializing devices...");
  ioe_init();
}

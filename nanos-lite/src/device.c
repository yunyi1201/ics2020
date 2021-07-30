#include <common.h>

#if defined(MULTIPROGRAM) && !defined(TIME_SHARING)
# define MULTIPROGRAM_YIELD() yield()
#else
# define MULTIPROGRAM_YIELD()
#endif

#define NAME(key) \
  [AM_KEY_##key] = #key,

#define min(a, b) \
	((a) > (b) ? (b) : (a))

static const char *keyname[256] __attribute__((used)) = {
  [AM_KEY_NONE] = "NONE",
  AM_KEYS(NAME)
};


size_t serial_write(const void *buf, size_t offset, size_t len) {
	for(int i=0; i<len; i++)
		putch(((const char* )buf)[i]);
  return len;
}

size_t events_read(void *buf, size_t offset, size_t len) {
	AM_INPUT_KEYBRD_T kbd = io_read(AM_INPUT_KEYBRD);
	size_t ret;
	if(kbd.keydown == 1) {
		ret = sprintf(buf, "%s %s\n", "kd", keyname[kbd.keycode]);
	} else if(kbd.keycode != AM_KEY_NONE) {
		ret = sprintf(buf, "%s %s\n", "ku", keyname[kbd.keycode]);	
	} else
		return 0;
	return ret;
}

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
	AM_GPU_CONFIG_T gpu = io_read(AM_GPU_CONFIG);
	int ret = sprintf(buf, "WIDTH: %d\nHEIGHT: %d\n", gpu.width, gpu.height);
  return ret;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
	AM_GPU_CONFIG_T gpu = io_read(AM_GPU_CONFIG);
	int x = offset % gpu.width, y = offset / gpu.width;
	int width = len / sizeof(uint32_t);
	io_write(AM_GPU_FBDRAW, x, y, (void *)buf, width, 1, 1);
	return len;
}

void init_device() {
  Log("Initializing devices...");
  ioe_init();
}

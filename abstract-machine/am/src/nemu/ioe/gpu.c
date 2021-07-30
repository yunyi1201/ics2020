#include <am.h>
#include <nemu.h>
#include <klib.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)
#define min(a, b) ((a) > (b) ? (b) : (a))

static uint32_t GPU_H, GPU_W, VMEMSZ;
void __am_gpu_init() {
	int i;
	GPU_H = inw(VGACTL_ADDR);
	GPU_W = inw(VGACTL_ADDR+2);
	VMEMSZ = GPU_W * GPU_H * sizeof(uint32_t);
	uint32_t *fb = (uint32_t*)(uintptr_t)FB_ADDR;
	for(i=0; i<GPU_H*GPU_W; i++) fb[i] = 0;
	outl(SYNC_ADDR, 1);
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = GPU_W, .height = GPU_H,
    .vmemsz = VMEMSZ
  };
}


void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {

	int w = ctl->w, h = ctl->h;
	int x = ctl->x, y = ctl->y;
	uint32_t *pixels = ctl->pixels;
	uint32_t *fb = (uint32_t *)(uintptr_t) FB_ADDR;	
	int cp_bytes = sizeof(uint32_t)*min(w, GPU_W - x);
	for(int i = 0; i < h && y+i < GPU_H; ++ i) {
		memcpy(&fb[(y + i)*GPU_W + x], pixels, cp_bytes);
		pixels += w;
	}
  if (ctl->sync) {
    outl(SYNC_ADDR, 1);
  }

}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}

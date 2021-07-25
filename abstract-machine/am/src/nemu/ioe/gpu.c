#include <am.h>
#include <nemu.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)

static uint32_t GPU_H, GPU_W, VMEMSZ;
void __am_gpu_init() {
	int i;
	GPU_H = inw(VGACTL_ADDR);
	GPU_W = inw(VGACTL_ADDR+2);
	VMEMSZ = GPU_W * GPU_H * sizeof(uint32_t);
	uint32_t *fb = (uint32_t*)(uintptr_t)FB_ADDR;
	for(i=0; i<GPU_H*GPU_W; i++) fb[i] = i;
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
  if (ctl->sync) {
    outl(SYNC_ADDR, 1);
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}

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

static uint32_t __get_pixel_pos(int x, int y, int i, int j) {
  /* Draw a graph from (x, y) -> (i, j)
   * for __am_gpu_fbdraw */
  x += i;
  y += j;
  return y * GPU_W + x;
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {

	int w = ctl->w, h = ctl->h;
  int i, j;

  // Map the [pixels] to [fb]
  uint32_t *fb = (uint32_t *)(uintptr_t) FB_ADDR;
  for (i = 0; i < w; ++i) {
    for (j = 0; j < h; ++j) {
      uint32_t col = *((uint32_t* ) (ctl->pixels) + (j * w + i));
      *(fb + __get_pixel_pos(ctl->x, ctl->y, i, j)) = col;
    }
  }
  if (ctl->sync) {
    outl(SYNC_ADDR, 1);
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}

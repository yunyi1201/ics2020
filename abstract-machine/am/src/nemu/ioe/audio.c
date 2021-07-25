#include <am.h>
#include <nemu.h>

#define AUDIO_FREQ_ADDR      (AUDIO_ADDR + 0x00)
#define AUDIO_CHANNELS_ADDR  (AUDIO_ADDR + 0x04)
#define AUDIO_SAMPLES_ADDR   (AUDIO_ADDR + 0x08)
#define AUDIO_SBUF_SIZE_ADDR (AUDIO_ADDR + 0x0c)
#define AUDIO_INIT_ADDR      (AUDIO_ADDR + 0x10)
#define AUDIO_COUNT_ADDR     (AUDIO_ADDR + 0x14)

#define AUDIO_QUEUE_HEAD_ADDR (AUDIO_SBUF_ADDR + 0x00)
#define AUDIO_QUEUE_END_ADDR  (AUDIO_SBUF_ADDR + 0x04)

static inline int __push(uint8_t val) {
  /* More likely a rule to maintain a
   * certain memory space
   * --------RULE--------
   * st  : AUDIO_SBUF_ADDR + 0x00, index;
   * ed  : AUDIO_SBUF_ADDR + 0x04, index;
   * size: AUDIO_COUNT_ADDR, uint32_t value;
   * buffer_data: [*st, *ed)
   * */

  /* Failed to implement */

  // static uint32_t st = 0;
  static uint32_t ed = 65536; // For tmp usage

  /* Implement with index 555 */
  // uint32_t head   = (uint32_t) inl(AUDIO_QUEUE_HEAD_ADDR);
  uint32_t end    = (uint32_t) inl(AUDIO_QUEUE_END_ADDR);
  uint32_t count  = (uint32_t) inl(AUDIO_COUNT_ADDR);

  if (count >= (uint32_t) 65536 - 8)
    return -1;

  outb(AUDIO_SBUF_ADDR + end, val);
  if (end == ed - 1) {
    outl(AUDIO_QUEUE_END_ADDR, 8);
  } else {
    outl(AUDIO_QUEUE_END_ADDR, end + 1);
  }
  outl(AUDIO_COUNT_ADDR, count + 1);
  return 0;
}

static inline int __am_get_count() {
  return inl(AUDIO_COUNT_ADDR);
}

void __am_audio_init() {
}

void __am_audio_config(AM_AUDIO_CONFIG_T *cfg) {
  cfg->present = true;
	cfg->bufsize = inl(AUDIO_SBUF_SIZE_ADDR);
}


void __am_audio_ctrl(AM_AUDIO_CTRL_T *ctrl) {
	outl(AUDIO_INIT_ADDR, 1);
	outl(AUDIO_FREQ_ADDR, ctrl->freq);
	outl(AUDIO_CHANNELS_ADDR, ctrl->channels);
	outl(AUDIO_SAMPLES_ADDR, ctrl->samples);
}

void __am_audio_status(AM_AUDIO_STATUS_T *stat) {
  stat->count = __am_get_count();
}

void __am_audio_play(AM_AUDIO_PLAY_T *ctl) {
	uint8_t *st, *ed;
  st = ctl->buf.start;
  ed = ctl->buf.end;
  // uint32_t count  = (uint32_t) inl(AUDIO_COUNT_ADDR);

  /* Wait until there's space to write
   * Atomic problems... */

  for (; st != ed; ++st) {
    /* Writeback count */
    while (__push(*st) == -1);
  }
}

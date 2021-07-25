#include <common.h>

#ifdef HAS_IOE

#include <device/map.h>
#include <SDL2/SDL.h>

#define AUDIO_PORT 0x200 // Note that this is not the standard
#define AUDIO_MMIO 0xa1000200
#define STREAM_BUF 0xa0800000
#define STREAM_BUF_MAX_SIZE 65536

enum {
  reg_freq,
  reg_channels,
  reg_samples,
  reg_sbuf_size,
  reg_init,
  reg_count,
  nr_reg
};

static uint8_t *sbuf = NULL;
static uint32_t *audio_base = NULL;
IOMap *queue, *audio_map;
extern IOMap* fetch_mmio_map(paddr_t addr);

static inline int __pop() {
  /* Return -1 if there's no element to pop */
  // static uint32_t st = 0;
  static uint32_t ed = 65536; // STREAM_MAX_BUF_SIZE

  queue = fetch_mmio_map(STREAM_BUF);
  audio_map = fetch_mmio_map(AUDIO_MMIO);

  // Use 'index' for better programming experience
  uint32_t head  = (uint32_t) map_read(STREAM_BUF + 0x00, sizeof(uint32_t), queue);
  uint32_t count = (uint32_t) map_read(AUDIO_MMIO + 0x14, sizeof(uint32_t), audio_map);

  int ret;
  if (count == 0)
    return -1;

  // GET corresponding val from buf pointed by head
  ret = (int) map_read(STREAM_BUF + head, sizeof(uint8_t), queue);
  if (head == ed - 1) {
    map_write(STREAM_BUF + 0x00, 8, sizeof(uint32_t), queue);
  } else {
    map_write(STREAM_BUF + 0x00, head + 1, sizeof(uint32_t), queue);
  }
  map_write(AUDIO_MMIO + 0x14, count - 1, sizeof(uint32_t), audio_map);
  return ret;
}

static inline void audio_play(void *userdata, uint8_t *stream, int len) {
	int nread = len;
  uint32_t count  = map_read(AUDIO_MMIO + 0x14, sizeof(uint32_t), audio_map);
  if (count < len) nread = count;

  // -> size chaned
  int tmp = nread;
  while (tmp--) {
    *stream++ = (uint8_t) __pop();
    // assert(*(stream - 1) != -1);
  }

  if (len > nread)
    memset(stream + nread, 0, len - nread);		
}

void update_audio() {
  audio_map = fetch_mmio_map(AUDIO_MMIO);
  uint32_t init = map_read(AUDIO_MMIO + 0x10, sizeof(uint32_t), audio_map);
  if (init != 1)
    return;

  // Set INIT to zero
  map_write(AUDIO_MMIO + 0x10, 0, sizeof(uint32_t), audio_map);

  // Init the queue IOMap for other functions
  queue = fetch_mmio_map(STREAM_BUF);

  // Set offset
  uint32_t freq = map_read(AUDIO_MMIO + 0x00, sizeof(uint32_t), audio_map);
  uint32_t chan = map_read(AUDIO_MMIO + 0x04, sizeof(uint32_t), audio_map);
  uint32_t samp = map_read(AUDIO_MMIO + 0x08, sizeof(uint32_t), audio_map);

  // Init bufsize
  map_write(AUDIO_MMIO + 0x0c, STREAM_BUF_MAX_SIZE, sizeof(uint32_t), audio_map);

  // Init buf queue, head -> 8 and end -> 8
  map_write(STREAM_BUF + 0x00, 8, sizeof(uint32_t), queue);
  map_write(STREAM_BUF + 0x04, 8, sizeof(uint32_t), queue);

  SDL_AudioSpec s = {};

  s.format   = AUDIO_S16SYS;
  s.freq     = freq;
  s.channels = chan;
  s.samples  = samp;
  // SDL2 will invoke the callback function
  s.callback = audio_play;
  s.userdata = NULL;

  int ret = SDL_InitSubSystem(SDL_INIT_AUDIO);
  if (ret == 0) {
    SDL_OpenAudio(&s, NULL);
    SDL_PauseAudio(0);
  }
}

static void audio_io_handler(uint32_t offset, int len, bool is_write) {
	return ;
}

void init_audio() {
  uint32_t space_size = sizeof(uint32_t) * nr_reg;
  audio_base = (void *)new_space(space_size);
  add_pio_map("audio", AUDIO_PORT, (void *)audio_base, space_size, audio_io_handler);
  add_mmio_map("audio", AUDIO_MMIO, (void *)audio_base, space_size, audio_io_handler);

  sbuf = (void *)new_space(STREAM_BUF_MAX_SIZE);
  add_mmio_map("audio-sbuf", STREAM_BUF, (void *)sbuf, STREAM_BUF_MAX_SIZE, NULL);
}
#endif	/* HAS_IOE */

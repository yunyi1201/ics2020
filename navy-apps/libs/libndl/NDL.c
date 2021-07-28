#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

static int evtdev = -1;
static int fbdev = -1;
static int screen_w = 0, screen_h = 0;

static struct timeval boottime;

uint32_t NDL_GetTicks() {
	struct timeval now;
	gettimeofday(&now, NULL);
	uint32_t ticks = (now.tv_sec - boottime.tv_sec)*1000 + (now.tv_usec - boottime.tv_usec)/1000;
  return ticks;
}

int NDL_PollEvent(char *buf, int len) {
	FILE *f = fopen("/dev/events", "r");

	for(int i=0; buf[i]; i++)        //BUG
		buf[i] = '\0';
  unsigned char c;
  size_t size = 0;
  c = fgetc(f);
  while (c != '\n' && c != 0 && c != 255) {
    *buf++ = c;
    c = fgetc(f);
    ++size;
  }

  return size;
}

void NDL_OpenCanvas(int *w, int *h) {
	FILE *f = fopen("/proc/dispinfo", "r");
  fscanf(f, "WIDTH: %d\nHEIGHT: %d", &screen_w, &screen_h);
  //printf("%d %d\n", screen_w, screen_h);

  if (*w == 0 && *h == 0) {
    *w = screen_w;
    *h = screen_h;
  }
  if (getenv("NWM_APP")) {
    int fbctl = 4;
    fbdev = 5;
    screen_w = *w; screen_h = *h;
    char buf[64];
    int len = sprintf(buf, "%d %d", screen_w, screen_h);
    // let NWM resize the window and create the frame buffer
    write(fbctl, buf, len);
    while (1) {
      // 3 = evtdev
      int nread = read(3, buf, sizeof(buf) - 1);
      if (nread <= 0) continue;
      buf[nread] = '\0';
      if (strcmp(buf, "mmap ok") == 0) break;
    }
    close(fbctl);
  }
}

void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {
	int fd = open("/dev/fb", O_WRONLY);
  int j;
  for (j = 0; j < h; ++j) {
    lseek(fd, ((y + j) * screen_w + x) * sizeof(uint32_t), SEEK_SET);
    write(fd, pixels + j * w, sizeof(uint32_t) * w);
  }
  close(fd);
}

void NDL_OpenAudio(int freq, int channels, int samples) {
}

void NDL_CloseAudio() {
}

int NDL_PlayAudio(void *buf, int len) {
  return 0;
}

int NDL_QueryAudio() {
  return 0;
}

int NDL_Init(uint32_t flags) {
  if (getenv("NWM_APP")) {
    evtdev = 3;
  }
	gettimeofday(&boottime, NULL);
  return 0;
}

void NDL_Quit() {
}

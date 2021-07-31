#include <NDL.h>
#include <SDL.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define keyname(k) #k,

static const char *keyname[] = {
  "NONE",
  _KEYS(keyname)
};

static const int length = (sizeof(keyname)) / (sizeof(const char *));
static uint8_t keystate[83] = {0};


int SDL_PushEvent(SDL_Event *ev) {
  return 0;
}

int SDL_PollEvent(SDL_Event *ev) {
	char buf[20] = "", name[20] = "", ch = '\0';
  int res = 0;
  res = NDL_PollEvent(buf, 19);
  if (res == 0)
  {
    ev->key.keysym.sym = SDLK_NONE;
    return 0;
  }
  // printf("%s\n", buf);
  sscanf(buf, "k%c %s\n", &ch, name);
  // printf("%c %s\n", ch, name);
  ev->key.keysym.sym = 0;
  for (int i = 0; i < ((sizeof(keyname)) / (sizeof(keyname[0]))); i++)
  {
    if (strcmp(name, keyname[i]) == 0)
    {
      ev->key.keysym.sym = i;
    }
  }
  assert(ev->key.keysym.sym != 0);
  if (ch == 'u')
    ev->key.type = SDL_KEYUP;
  else if (ch == 'd')
    ev->key.type = SDL_KEYDOWN;
  else
    assert(0);
  if (ev->key.type == SDL_KEYDOWN)
    keystate[ev->key.keysym.sym] = 1;
  else if (ev->key.type == SDL_KEYUP)
    keystate[ev->key.keysym.sym] = 0;
  ev->type = ev->key.type;
  return 1;
}

int SDL_WaitEvent(SDL_Event *event) {
	char buf[20] = "", name[20] = "", ch = '\0';
  int res = 0;
  while (res == 0)
    res = NDL_PollEvent(buf, 19);
  // printf("%s\n", buf);
  sscanf(buf, "k%c %s\n", &ch, name);
  // printf("%c %s\n", ch, name);
  event->key.keysym.sym = 0;
  for (int i = 0; i < ((sizeof(keyname)) / (sizeof(keyname[0]))); i++)
  {
    if (strcmp(name, keyname[i]) == 0)
    {
      event->key.keysym.sym = i;
    }
  }
  assert(event->key.keysym.sym != 0);
  if (ch == 'u')
    event->key.type = SDL_KEYUP;
  else if (ch == 'd')
    event->key.type = SDL_KEYDOWN;
  else
    assert(0);
  if (event->key.type == SDL_KEYDOWN)
    keystate[event->key.keysym.sym] = 1;
  else if (event->key.type == SDL_KEYUP)
    keystate[event->key.keysym.sym] = 0;
  event->type = event->key.type;
  return 1;
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  return 0;
}

uint8_t* SDL_GetKeyState(int *numkeys) {
  return NULL;
}

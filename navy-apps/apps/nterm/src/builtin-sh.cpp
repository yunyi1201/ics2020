#include <nterm.h>
#include <stdarg.h>
#include <unistd.h>
#include <SDL.h>

char handle_key(SDL_Event *ev);

char cmd_list[30][30];

static void sh_printf(const char *format, ...) {
  static char buf[256] = {};
  va_list ap;
  va_start(ap, format);
  int len = vsnprintf(buf, 256, format, ap);
  va_end(ap);
  term->write(buf, len);
}

static void sh_banner() {
  sh_printf("Built-in Shell in NTerm (NJU Terminal)\n\n");
}

static void sh_prompt() {
  sh_printf("sh> ");
}

static void sh_handle_cmd(const char *cmd) {
	char *argv_list[30];
  setenv("PATH", "/bin:/usr/bin", 0);
  int cmd_cnt = 0;
  while (*cmd && *cmd != '\n')
  {
    while(*cmd && *cmd == ' ')
      cmd++;
    sscanf(cmd, "%s", cmd_list[cmd_cnt]);
    argv_list[cmd_cnt] = cmd_list[cmd_cnt];
    cmd_cnt++;
    while(*cmd && *cmd != ' ' && *cmd != '\n')
      cmd++;
  }
  argv_list[cmd_cnt] = NULL;
  int res = execvp(argv_list[0], argv_list);
  printf("execvp result: %d", res);
}

void builtin_sh_run() {
  sh_banner();
  sh_prompt();

  while (1) {
    SDL_Event ev;
    if (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_KEYUP || ev.type == SDL_KEYDOWN) {
        const char *res = term->keypress(handle_key(&ev));
        if (res) {
          sh_handle_cmd(res);
          sh_prompt();
        }
      }
    }
    refresh_terminal();
  }
}

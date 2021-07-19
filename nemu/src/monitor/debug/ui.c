#include <isa.h>
#include "expr.h"
#include "watchpoint.h"
#include "../../../include/memory/vaddr.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);
int is_batch_mode();

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}


static int cmd_q(char *args) {
  return -1;
}

static int cmd_si(char *args) {
	if(args == NULL){
		isa_exec_once();
		return 0;
	}
	int n = atoi(args);
	if(n == 0){
		printf("string invalid\n");
		return 0;
	}
	for( ; n > 0; --n) 
		isa_exec_once();
	return 0;
}

static int cmd_info(char *args) {
	if(strcmp(args, "r") == 0) {
		isa_reg_display();			
		return 0;
	}else if(strcmp(args, "w") == 0) {
		display_wp();	
		return 0;
	}else {
		printf("Unknown arg\n");	
		return 0;
	}
}

static int cmd_x(char *args) {
	int n, addr;
	if(sscanf(args, "%d %x", &n, &addr) != 2) {
		printf("err too few arguments\n");
		return 0;
	}
	for(int i=0; i<n; i++){
		printf("0x%x: 0x%08x\n", addr, vaddr_read(addr, 4));
		addr += 4;
	}
	return 0;
}

static int cmd_p(char *args){
	if(args == NULL){
		printf("cmd_p need expression\n");
		return -1;
	}
	bool success = true;
	word_t ans = expr(args, &success);
	if(success){
		printf("DEC: %u, HEX: 0x%08x\n", ans, ans);
	}else{
		printf("expr: faliure\n");
	}
	return 0;
}

static int cmd_w(char *args){
	if(args == NULL){
		printf("cmd_w need expression\n");
		return 0;
	}
	bool success = true;
	word_t ans = expr(args, &success);
	if(success){
		WP* wp = new_wp();
		char *expression = (char*)malloc(sizeof(char)*32);
		strcpy(expression, args);
		wp->expr = expression;
		wp->old_val = ans;
		printf("watchpoint %d: %s\n", wp->NO, wp->expr);
	}else  {
		printf("expression invalid\n");
	}
	return 0;
}

static int cmd_d(char *args){
	if(args != NULL){
		printf("error: arguments\n");
	}else 
		delete_wp();
	return 0;
}

static int cmd_help(char *args);

static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
	{ "si", "step the execution of the program", cmd_si },
	{ "info", "printf registers or watchpoint informations", cmd_info },
	{ "x"  ,  "printf memory informations", cmd_x },
	{ "p",   "calculate the value of the expression", cmd_p },
	{ "w",  "set watchpoint", cmd_w },
	{ "d",  "delete watchpoint", cmd_d },

  /* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void ui_mainloop() {
  if (is_batch_mode()) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}

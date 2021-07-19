#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include <common.h>

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
	char *expr;
	bool used;
	word_t old_val;
	word_t new_val;
  /* TODO: Add more members if necessary */

} WP;

WP* new_wp();
void free_wp();
void display_wp();
void delete_wp();

#endif

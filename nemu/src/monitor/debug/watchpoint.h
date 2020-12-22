#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include <common.h>

typedef struct watchpoint {
  int NO;
  char *expr;
  struct {

	int old_value;
	int new_value;

  };
  struct watchpoint *next;

  /* TODO: Add more members if necessary */

} WP;
int set_wp( char* expr);

bool de_wp( int NO );

void display_wp( void );

bool de_all( void );

WP* scan_wp();	
#endif

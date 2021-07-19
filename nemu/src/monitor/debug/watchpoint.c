#include "watchpoint.h"
#include "expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
		wp_pool[i].used = false;
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

WP* new_wp() {
	if(free_ == NULL)
		return NULL;
	WP* new = free_;
	assert(new->used == false);
	new->used = true;
	free_   = free_->next;
	new->next = head;
	head = new;
	return new;
}


void free_wp(WP *wp) {
	assert(wp != NULL);
	assert(wp->used != false);
	int NO = wp->NO;
	WP* cur, *prev;
	cur = prev = head;
	int flag = 1;
	for( ; cur; cur = cur->next) {
		if(cur->NO == NO)
			break;
		if(flag){
			flag = 0;
		}
		else 
			prev = prev->next;
	}
	if(cur == head)
		head = head->next;
	else {
		prev->next = cur->next;
	}
	wp->used = false;
	wp->next = free_;
	free_ = wp;
}

void display_wp(){
	WP* new = head;
	while(new) {
		printf("watchpoint %d: %s\n", new->NO, new->expr);	
		new = new->next;
	}
}

void delete_wp(){
	WP* cur = head;
	do{
		free_wp(cur);
		cur = head;
	} while(cur);
}
/* TODO: Implement the functionality of watchpoint */


#include "watchpoint.h"
#include "expr.h"
#include <assert.h>
#include <stdlib.h>
#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].used = false;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */
static WP* new_wp() {
	assert( free_ != NULL );
	WP *p = free_;
	assert( p->used == false );
	free_ = p->next;
	p->next = head;
	p->used = true;
	head = p;
	return p;

}

static void free_wp( WP* wp ) {
	
	assert( head != NULL );
	wp->used = false;	
	wp->next = free_;
	free_ = wp;
}

int set_wp( char *args ) {
	
	bool success = true;
	word_t res = expr( args, &success );
	WP *p;	
	if( success ) {	
		p = new_wp();		
		p->used = true;
		p->old_value = res;	
		p->new_value = res;
		p->expr = (char *)malloc((strlen(args)+1)*sizeof(char));
	 	strcpy( p->expr, args );	
	} else {
		
		return -1;
	}
	return p->NO;
}
bool de_wp( int NO ) {
	
	WP* dummy = (WP *)malloc(sizeof(struct watchpoint));
	dummy->next = head;
	WP *pre;
	WP *tmp;
	for( pre=dummy, tmp=head; tmp && tmp->NO != NO; tmp=tmp->next, pre = pre->next );
	if( tmp ) {
		pre->next = tmp->next;
		free(tmp->expr);
		free_wp( tmp );
		head = dummy->next;
	}
	else { 
		free(dummy);
		return false;
	}

	free(dummy);
	return true;
}
void display_wp( ) {

	if( !head ) {
		printf("NO watchpoint\n");
		return ;
	}
	for( WP *tmp=head; tmp != NULL; tmp=tmp->next ) {
		printf("watchpoint%d %s\n",tmp->NO, tmp->expr);		
	}
	
}
void de_all( void ) {

	if( !head || head->used == false ) {
		return ;
	}
	for( WP* tmp=head; tmp && tmp->used; tmp=tmp->next ) {
		free_wp( tmp );


	}
	head = NULL;
	return ;	
				
}

WP* scan_wp( ) {
	
	if( !head ) 
		return NULL;

	for( WP *tmp=head; tmp != NULL; tmp=tmp->next ) {
		bool success = true;
	        int res = expr( tmp->expr, &success );
		if( success == false ) 
			continue;
		tmp->new_value = res;
		if( tmp->new_value != tmp->old_value ) {
		  	tmp->old_value = res;	
			return tmp; 
		}
		
	}
	return NULL;	
}

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

int main(int argc, char *argv[], char *envp[]);
extern char **environ;
void call_main(uintptr_t *args) {
  //char *empty[] =  {NULL };
	int argc; 
	char **argv, **envp;
	argc = *args;
	
	printf("argc: %d\n", argc);

	argv = args + 1;

	envp = args + 1 + argc + 1;

  //environ = empty;
  exit(main(argc, argv, envp));
  assert(0);
}

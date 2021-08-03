#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

int main(int argc, char *argv[], char *envp[]);
extern char **environ;
void call_main(uintptr_t *args) {
  char *empty[] =  {NULL };
	uint32_t argc = *(uint32_t *)args;
	printf("argc :%d\n", argc);
  environ = empty;
	char ** argv = (char **)((uint32_t *)args + 1);
  exit(main(argc, argv, empty));
  assert(0);
}

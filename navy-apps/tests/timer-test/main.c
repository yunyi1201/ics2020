#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <NDL.h>

//extern int NDL_Init(uint32_t);
//extern uint32_t NDL_GetTicks();

int main(){
	printf("timer-test\n");
	NDL_Init(0);
	int i =0;
	uint32_t s = NDL_GetTicks();
 uint32_t t = s+1000;
 while(1){
    s = NDL_GetTicks();
     if(s >= t)
     {
        t = s + 1000 ;
        i++;
        printf("%d tick \n",i);

     }
 }
 return 0;
}

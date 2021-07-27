#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>

//typedef __uint8_t uint8_t;
//typedef __uint16_t uint16_t;
//typedef __uint32_t uint32_t;
//typedef __uint64_t uint64_t;


//#include "../../libs/libndl/include/NDL.h"
int main(){
printf("timer-test\n");
//NDL_Init(0);
//int i =0;
 //uint32_t s = NDL_GetTicks();
 struct timeval time;
 //uint32_t t = s+1000;
 uint32_t sec = 0;
 while(1){
		gettimeofday(&time, NULL);
		/*
    s = NDL_GetTicks();
     if(s >= t)
     {
        t = s + 1000 ;
        i++;
        printf("%d tick \n",i);

     }
		 */
		if(time.tv_sec > sec) {
			printf("%d sec\n", sec);
			sec ++;
		}
 }
 return 0;
}

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/times.h>

void busy_wait_delay(int seconds)
{
	int i, dummy;
	int tps = sysconf(_SC_CLK_TCK);
	clock_t start;struct tms exec_time;
	times(&exec_time);
	start = exec_time.tms_utime;
	while( (exec_time.tms_utime - start) < (seconds * tps)){
		for(i=0; i<1000; i++){
			dummy = i;
		}
		times(&exec_time);
	}
}

void* sleepy1(){
	printf("Thread 1 says hello \n");
	busy_wait_delay(5);
	printf("Thread 1 says goodbye \n");
}

void* sleepy2(){
	printf("Thread 2 says hello \n");
	busy_wait_delay(5);
	printf("Thread 2 says goodbye \n");
}

int main(){
	pthread_t sleepyT1;
	pthread_t sleepyT2;
	pthread_create(&sleepyT1, NULL, sleepy1, NULL);
	pthread_create(&sleepyT2, NULL, sleepy2, NULL);
	pthread_join(sleepyT1, NULL);
	pthread_join(sleepyT2, NULL);
	return 0;
}





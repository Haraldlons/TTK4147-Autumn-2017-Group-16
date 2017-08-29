#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



void* sleepy1(){
	printf("Thread 1 says hello \n");
	sleep(5);
	printf("Thread 1 says goodbye \n");
}

void* sleepy2(){
	printf("Thread 2 says hello \n");
	sleep(5);
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

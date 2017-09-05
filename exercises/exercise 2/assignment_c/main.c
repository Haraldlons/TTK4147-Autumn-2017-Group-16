#include <unistd.h>
#include <stdio.h>
// #include <stdlib.h>
#include <sys/times.h>
#include <pthread.h>
// #define N_FLOORS 4
#include <semaphore.h>

extern int global_variable = 0;
sem_t semaphore;


void* increment_and_print(){
	sem_wait(&semaphore);
	for(int i = 0; i <= 4; i++){
		printf("Thread number %i\n",i);
		usleep(100000);
	}
	sem_post(&semaphore);
}


int main(int argc, char **argv)
{
	
	sem_init(&semaphore, 0, 3);

	pthread_t thread1;
	pthread_t thread2;
	pthread_t thread3;
	pthread_t thread4;
	pthread_t thread5;
	pthread_create(&thread1, NULL, increment_and_print, NULL);
	pthread_create(&thread2, NULL, increment_and_print, NULL);
	pthread_create(&thread3, NULL, increment_and_print, NULL);
	pthread_create(&thread4, NULL, increment_and_print, NULL);
	pthread_create(&thread5, NULL, increment_and_print, NULL);
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);
	pthread_join(thread4, NULL);
	pthread_join(thread5, NULL);



	return 0;
}

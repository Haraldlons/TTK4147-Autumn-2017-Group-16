#include <unistd.h>
#include <stdio.h>
// #include <stdlib.h>
#include <sys/times.h>
#include <pthread.h>
// #define N_FLOORS 4
#include <semaphore.h>

extern int global_variable = 0;

extern int running = 1;
extern int var1 = 0;
extern int var2 = 0;

pthread_mutex_t lock;
void* foo1(){
	while(running){
		pthread_mutex_lock(&lock);
		var1++;
		var2 = var1;
		pthread_mutex_unlock(&lock);
		// printf("%s", ".");
	}

}

void* foo2(){
	for(int i = 0; i <= 20; i++){
		pthread_mutex_lock(&lock);
		printf("Number 1 is %i, number 2 is %i. Differanse: %i \n",var1, var2, var2-var1 );
		pthread_mutex_unlock(&lock);
		usleep(1);
	}
	running = 0;
}


int main(int argc, char **argv)
{
	pthread_mutex_init(&lock, NULL);

	pthread_t thread1;
	pthread_t thread2;
	pthread_create(&thread1, NULL, foo1, NULL);
	pthread_create(&thread2, NULL, foo2, NULL);
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	pthread_mutex_destroy(&lock);


	return 0;
}

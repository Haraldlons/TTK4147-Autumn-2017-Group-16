#include <unistd.h>
#include <stdio.h>
// #include <stdlib.h>
#include <sys/times.h>
#include <pthread.h>
// #define N_FLOORS 4
#include <semaphore.h>

extern int global_variable = 0;

extern int flag = 1;
extern int var1 = 0;
extern int var2 = 0;

pthread_mutex_t fork1;
pthread_mutex_t fork2;
pthread_mutex_t fork3;
pthread_mutex_t fork4;
pthread_mutex_t fork5;

void* phil1(){
	int antall = 0;
	int r;
	while(flag){

		pthread_mutex_lock(&fork1);
		pthread_mutex_lock(&fork2);
		antall++;
		printf("Phil 1 for the %i time \n", antall);
		sleep(0.5);
		pthread_mutex_unlock(&fork1);
		pthread_mutex_unlock(&fork2);
		r = rand(1,10)%10; 
		usleep(r);
	}
}
void* phil2(){
	int antall = 0;
	int r;
	while(flag){
		pthread_mutex_lock(&fork2);
		pthread_mutex_lock(&fork3);
		antall++;
		printf("Phil 2 for the %i time \n", antall);
		sleep(0.5);
		pthread_mutex_unlock(&fork2);
		pthread_mutex_unlock(&fork3);
		r = rand(1,10)%10; 
		usleep(r);
	}
}
void* phil3(){
	int antall = 0;
	int r;
	while(flag){
		pthread_mutex_lock(&fork3);
		pthread_mutex_lock(&fork4);
		antall++;
		printf("Phil 3 for the %i time \n", antall);
		sleep(0.5);
		pthread_mutex_unlock(&fork3);
		pthread_mutex_unlock(&fork4);
		r = rand(1,10)%10; 
		usleep(r);
	}
}
void* phil4(){
	int antall = 0;
	int r;
	while(flag){
		pthread_mutex_lock(&fork4);
		pthread_mutex_lock(&fork5);
		antall++;
		printf("Phil 4 for the %i time \n", antall);
		sleep(0.5);
		pthread_mutex_unlock(&fork4);
		pthread_mutex_unlock(&fork5);
		r = rand(1,10)%10; 
		usleep(r);
	}
}
void* phil5(){
	int antall = 0;
	int r;
	while(flag){
		pthread_mutex_lock(&fork1);
		pthread_mutex_lock(&fork5);
		antall++;
		printf("Phil 5 for the %i time \n", antall);
		sleep(0.5);
		pthread_mutex_unlock(&fork5);
		pthread_mutex_unlock(&fork1);
		r = rand(1,10)%10; 
		usleep(r);
	}
}




int main(int argc, char **argv)
{
	srand(time(NULL));   // should only be called once
	// int r;
	// for(int i = 0; i < 10; i++){
	// 	r = rand(1,10)%10000;  
	// 	printf("%i\n", r);
	// }
	// sleep(10);
	pthread_mutex_init(&fork1, NULL);
	pthread_mutex_init(&fork2, NULL);
	pthread_mutex_init(&fork3, NULL);
	pthread_mutex_init(&fork4, NULL);
	pthread_mutex_init(&fork5, NULL);

	pthread_t thread1, thread2, thread3, thread4, thread5;
	pthread_create(&thread1, NULL, phil1, NULL);
	pthread_create(&thread2, NULL, phil2, NULL);
	pthread_create(&thread3, NULL, phil3, NULL);
	pthread_create(&thread4, NULL, phil4, NULL);
	pthread_create(&thread5, NULL, phil5, NULL);
	sleep(10);
	printf("Sovet ferdig\n");
	flag = 0;

	pthread_mutex_destroy(&fork4);



	return 0;
}

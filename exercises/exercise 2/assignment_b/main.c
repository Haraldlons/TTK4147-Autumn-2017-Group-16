#include <unistd.h>
#include <stdio.h>
// #include <stdlib.h>
#include <sys/times.h>
#include <pthread.h>
// #define N_FLOORS 4

extern int global_variable = 0;


void* increment_and_print(){
	int local_variable = 0;
	local_variable++;
	global_variable++;

	printf("Local Variable: %i\n", local_variable );
	printf("Global Variable: %i\n", global_variable );

}


int main(int argc, char **argv)
{
	pthread_t thread1;
	pthread_t thread2;
	for(int i = 0; i < 10; i++){
		pthread_create(&thread1, NULL, increment_and_print, NULL);
		pthread_create(&thread2, NULL, increment_and_print, NULL);
	}
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);



	return 0;
}

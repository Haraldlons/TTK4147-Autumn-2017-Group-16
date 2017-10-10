#include "io.h"
#include <unistd.h>
#include <pthread.h>
#include "sched.h"

pthread_t test1;
pthread_t test2;
pthread_t test3;

#define DISTURBANCE 10
#define PERIOD 10 // in us

int set_cpu(int cpu_number)
{
	// setting cpu set to the selected cpu
	cpu_set_t cpu;
	CPU_ZERO(&cpu);
	CPU_SET(cpu_number, &cpu);
	// set cpu set to current thread and return
	return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}

struct timespec aTime;
struct timespec bTime;
struct timespec cTime;


void timespec_add_us(struct timespec *t, long us)
{
	// add microseconds to timespecs nanosecond counter
	t->tv_nsec += us*1000;
	// if wrapping nanosecond counter, increment second counter
	if (t->tv_nsec > 1000000000)
	{
		t->tv_nsec = t->tv_nsec-1000000000;
		t->tv_sec += 1;
	}
}

void* testA(){
	set_cpu(1);
	clock_gettime(CLOCK_REALTIME, &aTime);
	while(1){
		timespec_add_us(&aTime, PERIOD);
		clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &aTime, NULL);
		if(!io_read(1)){
			io_write(1, 0);
			usleep(1);
			io_write(1, 1);
		}
	}
}

void* testB(){
	set_cpu(1);
	clock_gettime(CLOCK_REALTIME, &bTime);
	while(1){
		timespec_add_us(&bTime, PERIOD);
		clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &bTime, NULL);
		if(!io_read(2)){
			io_write(2, 0);
			usleep(1);
			io_write(2, 1);
		}
	}
}

void* testC(){
	set_cpu(1);
	clock_gettime(CLOCK_REALTIME, &cTime);
	while(1){
		timespec_add_us(&cTime, PERIOD);
		clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &cTime, NULL);
		if(!io_read(3)){
			io_write(3, 0);
			usleep(1);
			io_write(3, 1);
		}
	}
}

void* disturbance(){
	set_cpu(1);
	int i = 0;
	while(1){
		i++;
	}
}

int main(){
	io_init();
	io_write(1, 1);
	io_write(2, 1);
	io_write(3, 1);

	pthread_create(&test1, NULL, testA, NULL);
	pthread_create(&test2, NULL, testB, NULL);
	pthread_create(&test3, NULL, testC, NULL);

	pthread_t disturbances[DISTURBANCE];
	int i;
	for(i = 0; i < DISTURBANCE; i++){
		pthread_create(&disturbances[i], NULL, disturbance, NULL);
	}


	while(1){
		continue;
	}

	return 0;
}

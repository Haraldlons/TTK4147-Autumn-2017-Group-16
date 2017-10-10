#define DISTURBANCE 10
#define PERIOD 100000 //In ns
#include <pthread.h>
#include <unistd.h>
#include "io.h"
#include <rtdk.h>
#include <stdio.h>
#include <sys/mman.h>
#include <native/task.h>  
#include <native/timer.h>
 #include <signal.h>

void* testAFunc(){
	rt_task_set_periodic(NULL, TM_NOW, PERIOD);
	while(1){
		if(!io_read(1)){
			io_write(1, 0);
			rt_task_sleep(1);
			io_write(1, 1);
		}
		rt_task_wait_period(NULL);
	}
}

void* testBFunc(){	
	rt_task_set_periodic(NULL, TM_NOW, PERIOD);
	while(1){
		if(!io_read(2)){
			io_write(2, 0);
			rt_task_sleep(1);
			io_write(2, 1);
		}
		rt_task_wait_period(NULL);
	}
}

void* testCFunc(){
	rt_task_set_periodic(NULL, TM_NOW, PERIOD);
	while(1){
		if(!io_read(3)){
			io_write(3, 0);
			rt_task_sleep(1);
			io_write(3, 1);
		}
		rt_task_wait_period(NULL);
	}
}

int set_cpu(int cpu_number)
{
	// setting cpu set to the selected cpu
	cpu_set_t cpu;
	CPU_ZERO(&cpu);
	CPU_SET(cpu_number, &cpu);
	// set cpu set to current thread and return
	return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}
void* disturbance(){
	set_cpu(0);
	int i = 0;
	while(1){
		i++;
	}
}

int main(){
	mlockall(MCL_CURRENT|MCL_FUTURE);

	io_init();
	io_write(1, 1);
	io_write(2, 1);
	io_write(3, 1);

	rt_print_auto_init(1);
	rt_printf("stuff");


	pthread_t disturbances[DISTURBANCE];
	int i;
	for(i = 0; i < DISTURBANCE; i++){
		pthread_create(&disturbances[i], NULL, disturbance, NULL);
	}

	RT_TASK* testA;
	RT_TASK* testB;
	RT_TASK* testC;
	rt_task_create(testA, "Reaction test A", 0, 50, T_CPU(0));
	rt_task_create(testB, "Reaction test B", 0, 50, T_CPU(0));
	rt_task_create(testC, "Reaction test C", 0, 50, T_CPU(0));

	rt_task_start(testA, testAFunc, NULL);
	rt_task_start(testB, testBFunc, NULL);
	rt_task_start(testC, testCFunc, NULL);

	while(1){
		continue;
	}
	return 0;
}
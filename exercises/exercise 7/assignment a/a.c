#include <native/sem.h>
#include <pthread.h>
#include <unistd.h>
#include <rtdk.h>
#include <stdio.h>
#include <sys/mman.h>
#include <native/task.h>  
#include <native/timer.h>
#include <signal.h>


RT_SEM sem;

void taskFunc(int taskNum){
	usleep(1000000);
	rt_printf("%i waiting\n", taskNum);
	rt_sem_p(&sem, TM_INFINITE);
	rt_printf("%i Running!\n", taskNum);

}

void broadcast(){
	usleep(100);
	rt_printf("GOGO!\n");
	rt_sem_broadcast(&sem);
	usleep(100);
}

int main(){
	mlockall(MCL_CURRENT|MCL_FUTURE);
	rt_print_auto_init(1);
	rt_sem_create(&sem, "Barrier semaphore", 1, S_PRIO);
	RT_TASK task1;
	RT_TASK task2;
	RT_TASK task3;
	rt_task_create(&task1, "Barrier task 1", 0, 15, T_CPU(0));
	rt_task_create(&task2, "Barrier task 2", 0 , 10, T_CPU(0));
	rt_task_create(&task3, "Broadcaster", 0, 20, T_CPU(0));
	rt_task_start(&task1, &taskFunc, 1);
	rt_task_start(&task2, &taskFunc, 2);
	rt_task_start(&task3, &broadcast, NULL);

	usleep(1000000);
	rt_printf("deleting sem\n");
	rt_sem_delete(&sem);
	return 0;
}


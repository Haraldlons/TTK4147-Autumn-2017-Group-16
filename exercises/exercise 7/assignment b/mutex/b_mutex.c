#include <native/sem.h>
#include <pthread.h>
#include <unistd.h>
#include <rtdk.h>
#include <stdio.h>
#include <sys/mman.h>
#include <native/task.h>  
#include <native/timer.h>
#include <signal.h>
#include <native/mutex.h>


RT_SEM barrier;
RT_MUTEX resource;

void busy_wait_ms(unsigned long delay){
	unsigned long count = 0;
	while (count <= delay*10){
		rt_timer_spin(1000*100);
		count++;
	}
}

int rt_task_sleep_ms(unsigned long delay){
	return rt_task_sleep(1000*1000*delay);
}


void taskLFunc(){
	rt_sem_p(&barrier, TM_INFINITE);
	rt_printf("L aquiring resource\n");
	rt_mutex_acquire(&resource, TM_INFINITE);
	rt_printf("L aquired resource\n");
	busy_wait_ms(3);
	rt_printf("L releasing resource\n");
	rt_mutex_release(&resource);
	rt_printf("L finished\n");

}

void taskMFunc(){
	rt_sem_p(&barrier, TM_INFINITE);
	rt_task_sleep_ms(1);
	busy_wait_ms(5);
	rt_printf("M finished\n");
}

void taskHFunc(){
	rt_sem_p(&barrier, TM_INFINITE);
	rt_task_sleep_ms(2);
	rt_printf("H aquiring resource\n");
	rt_mutex_acquire(&resource, TM_INFINITE);
	rt_printf("H aquired resource\n");
	busy_wait_ms(2);
	rt_printf("H releasing resource\n");
	rt_mutex_release(&resource);
	rt_printf("H finished\n");

}

void broadcast(){
	usleep(100);
	rt_sem_broadcast(&barrier);
	usleep(100);
}

int main(){
	mlockall(MCL_CURRENT|MCL_FUTURE);
	rt_print_auto_init(1);
	rt_sem_create(&barrier, "Barrier semaphore", 0, S_PRIO);
	rt_mutex_create(&resource, "resource");
	RT_TASK L;
	RT_TASK M;
	RT_TASK H;
	RT_TASK barrierTask;
	rt_task_create(&L, "Low priority", 0, 5, T_CPU(0));
	rt_task_create(&M, "Medium priority", 0 , 10, T_CPU(0));
	rt_task_create(&H, "High priority", 0, 20, T_CPU(0));
	rt_task_create(&barrierTask, "barrier", 0, 30, T_CPU(0));

	rt_task_start(&L, &taskLFunc, NULL);
	rt_task_start(&M, &taskMFunc, NULL);
	rt_task_start(&H, &taskHFunc, NULL);
	rt_task_start(&barrierTask, &broadcast, NULL);

	usleep(1000000);
	rt_sem_delete(&barrier);
	rt_mutex_delete(&resource);
	return 0;
}


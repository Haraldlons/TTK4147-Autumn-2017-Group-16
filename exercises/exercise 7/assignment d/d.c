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
RT_MUTEX mutA;
RT_MUTEX mutB;

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
	rt_mutex_acquire(&mutA, TM_INFINITE);
	busy_wait_ms(3);
	rt_mutex_acquire(&mutB, TM_INFINITE);
	busy_wait_ms(3);
	rt_mutex_release(&mutB);
	rt_mutex_release(&mutA);
	busy_wait_ms(1);
	rt_printf("L finished\n");

}

void taskHFunc(){
	rt_sem_p(&barrier, TM_INFINITE);
	rt_task_sleep_ms(1);
	rt_mutex_acquire(&mutB, TM_INFINITE);
	busy_wait_ms(1);
	rt_mutex_acquire(&mutA, TM_INFINITE);
	busy_wait_ms(2);
	rt_mutex_release(&mutA);
	rt_mutex_release(&mutB);
	busy_wait_ms(1);
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
	rt_mutex_create(&mutA, "Mutex A");
	rt_mutex_create(&mutB, "Mutex B");
	RT_TASK L;
	RT_TASK H;
	RT_TASK barrierTask;
	rt_task_create(&L, "Low priority", 0, 5, T_CPU(0));
	rt_task_create(&H, "High priority", 0, 20, T_CPU(0));
	rt_task_create(&barrierTask, "barrier", 0, 30, T_CPU(0));

	rt_task_start(&L, &taskLFunc, NULL);
	rt_task_start(&H, &taskHFunc, NULL);
	rt_task_start(&barrierTask, &broadcast, NULL);

	usleep(10000000);
	rt_printf("Exit\n");
	rt_sem_delete(&barrier);
	rt_mutex_delete(&mutA);
	rt_mutex_delete(&mutB);
	return 0;
}


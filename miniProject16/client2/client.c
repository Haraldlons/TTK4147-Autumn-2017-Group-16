#include "miniproject.h"
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <semaphore.h>

//Statics:
//Comm:
#define PORT 9999
#define IP "192.168.0.1"
//PID:
#define KP 10
#define KI 800
#define REFERENCE 1
//Timing: (all in us)
#define PERIOD 15000
#define RUNTIME 500000

//Globals:
struct udp_conn connection;
pthread_mutex_t udp;		//lock on the udp channel for sending
pthread_mutex_t y_mutex;	//lock on system output
sem_t ack_sem; 				//Keeps count on how many acks we need to send
int running = 0; 			//True while sim is running
float y = 0; 				//system output

//Functions:
void init(){
	udp_init_client(&connection, PORT, IP);
	sem_init(&ack_sem, 0 ,0);
	pthread_mutex_init(&udp, NULL);
	pthread_mutex_init(&y_mutex, NULL);
	char send[30];
	sprintf(send,"%s","START");
	pthread_mutex_lock(&udp);
	udp_send(&connection,send, strlen(send));
	pthread_mutex_unlock(&udp);
	running = 1;
}

void stop(){
	char* send = "STOP";
	pthread_mutex_lock(&udp);
	udp_send(&connection, send, 5);
	pthread_mutex_unlock(&udp);
	running = 0;
	udp_close(&connection);
}

void send_ack(){
	char* send = "SIGNAL_ACK";
	pthread_mutex_lock(&udp);
	udp_send(&connection,send,11);
	pthread_mutex_unlock(&udp);
}

void *ack_sender(){
	while(running){
		sem_wait(&ack_sem);
		send_ack();
	}
}

void *reciever(){
	char recieved[30];
	char *value;
	while(running){
		udp_receive(&connection, recieved, 30);
		value = strstr(recieved,"GET_ACK:");
		if(value){
			value = value+8;
			pthread_mutex_lock(&y_mutex);
			y = atof(value);
			pthread_mutex_unlock(&y_mutex);
		}
		value = strstr(recieved, "SIGNAL");
		if(value){
			sem_post(&ack_sem);
		}
		value = strstr(recieved, "STOP");
		if(value){
			printf("Server stopped\n");
		}
	}
}

void send_get(){
	char* send = "GET";
	pthread_mutex_lock(&udp);
	udp_send(&connection,send,4);
	pthread_mutex_unlock(&udp);
}

void update_input(float value){
	char send[13];
	snprintf(send,13,"SET:%f",value);
	send[12] = '\0';
	pthread_mutex_lock(&udp);
	udp_send(&connection,send, 13);
	pthread_mutex_unlock(&udp);
}

void *pid(){
	float integral = 0;
	float error;
	float u;
	struct timespec time_pid;
	clock_gettime(CLOCK_REALTIME, &time_pid);
	int i = 0;
	for(i = 0 ; i < RUNTIME/PERIOD ; i++){
		timespec_add_us(&time_pid, PERIOD);
		send_get();
		pthread_mutex_lock(&y_mutex);
		error = REFERENCE-y;
		pthread_mutex_unlock(&y_mutex);
		integral = integral + (error*PERIOD/1000000);
	    u = KP*error + KI*integral;
		update_input(u);
		clock_nanosleep(&time_pid);
	}
}

int main(){
	init();
	pthread_t pid_thread;
	pthread_t reciever_thread;
	pthread_t ack_thread;
	pthread_create(&pid_thread, NULL, pid, NULL);
	pthread_create(&reciever_thread, NULL, reciever, NULL);
	pthread_create(&ack_thread, NULL, ack_sender, NULL);
	pthread_join(pid_thread, NULL);
	stop();
	return 1;
}

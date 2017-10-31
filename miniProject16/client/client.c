#include "miniproject.h"
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

//Statics:
//Comm:
#define PORT 9999
#define IP "192.168.0.1"
//PID:
#define KP 10
#define KI 800
#define REFERENCE 1
//Timing: (all in us)
#define PERIOD 2000
#define RUNTIME 500000

//Globals:
struct udp_conn connection;
pthread_mutex_t udp_mutex; //Lock on sending over udp (Not really necessary for part 1)

//Functions:
void init(){
	udp_init_client(&connection, PORT, IP);
	pthread_mutex_init(&udp_mutex, NULL);
	char* send = "START";
	pthread_mutex_lock(&udp_mutex);
	udp_send(&connection,send, strlen(send));
	pthread_mutex_unlock(&udp_mutex);
}

void stop(){
	char* send = "STOP"
	pthread_mutex_lock(&udp_mutex);
	udp_send(&connection, send, 5);
	pthread_mutex_unlock(&udp_mutex);
	udp_close(&connection);
}

float get_output(){
	char recieved[30];
	char* value;
	udp_receive(&connection, recieved, 30);
	value = strstr(recieved,"GET_ACK:");
	if(value){
		value = value+8; 
		return(atof(value));
	}
	value = strstr(recieved, "STOP");
	if(value){
		printf("Server stopped\n");
	}
	return 0;
}

void send_get(){ 
	char* send = "GET"; 
	pthread_mutex_lock(&udp_mutex);
	udp_send(&connection,send,4);
	pthread_mutex_unlock(&udp_mutex);
}

void update_input(float value){
	char send[13];
	snprintf(send,13,"SET:%f",value);
	send[12] = '\0'; 
	pthread_mutex_lock(&udp_mutex);
	udp_send(&connection,send, 13);
	pthread_mutex_unlock(&udp_mutex);

}

void *pid(){
	float integral = 0;
	float error;
	float u;
	float y;
	struct timespec time_pid;
	clock_gettime(CLOCK_REALTIME, &time_pid);
	int i = 0;
	for(i = 0 ; i < RUNTIME/PERIOD ; i++){ 
		timespec_add_us(&time_pid, PERIOD);
		send_get();
		y = get_output();
		error = REFERENCE-y;
		integral = integral + (error*PERIOD/1000000); 
	    u = KP*error + KI*integral;
		update_input(u);
		clock_nanosleep(&time_pid); 
	}
}

int main(){
	init();
	pthread_t pid_thread;
	pthread_create(&pid_thread, NULL, pid, NULL);
	pthread_join(pid_thread, NULL); 
	stop();
	return 1;
}

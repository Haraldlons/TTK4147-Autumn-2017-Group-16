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
pthread_mutex_t udp; //lock on the udp channel for sending: Not really necessary (yet)

//Functions:
void init(){
	udp_init_client(&connection, PORT, IP);
	char send[30];
	sprintf(send,"%s","START");
	pthread_mutex_lock(&udp);
	udp_send(&connection,send, strlen(send));
	pthread_mutex_unlock(&udp);
}

void stop(){
	char send[5];
	snprintf(send,5,"STOP"); //Hvorfor bruke snprintf?
	send[4] = '\0'; //Dette er kun ét tegn? (ikke to)

	pthread_mutex_lock(&udp);
	udp_send(&connection, send, 5);
	pthread_mutex_unlock(&udp);
	udp_close(&connection);
}

float get_output(){
	char recieved[30];
	char* value;
	udp_receive(&connection, recieved, 30);
	value = strstr(recieved,"GET_ACK:"); //Denne gir alt ETTER 'GET_ACK:'?
	if(value){
		value = value+8; //What is this stuff?
		return(atof(value));
	}
	printf("Bad things happened!\n"); //Maybe change this line?
	return 0;
}

void send_get(){ 
	char* send = "GET"; 

	pthread_mutex_lock(&udp);
	udp_send(&connection,send,4); //Hvorfor må vi noen steder sende '\0' mens andre steder ikke?
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
	float y;
	struct timespec time_pid;
	clock_gettime(CLOCK_REALTIME, &time_pid);
	int i = 0;
	for(i = 0 ; i < RUNTIME/PERIOD ; i++){ //Kan man ha en while og heller sjekke om en klokke har nådd 0.5 sek?
		timespec_add_us(&time_pid, PERIOD);
		send_get();
		y = get_output();
		error = REFERENCE-y;
		integral = integral + (error*PERIOD);
	    u = KP*error + KI*integral;
		update_input(u);
		clock_nanosleep(&time_pid); //sove intill?
	}
}

int main(){
	init();
	pthread_t pid_thread;
	pthread_create(&pid_thread, NULL, pid, NULL);
	pthread_join(pid_thread, NULL); //Join som venter? 
	stop(); //Hva er det som hindrer funksjonen til å avslutte? 
	return 1;
}

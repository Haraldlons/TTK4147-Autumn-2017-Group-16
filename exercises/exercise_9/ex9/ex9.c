#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/dispatch.h>
#include <sys/mman.h>
#include <pthread.h>

struct pid_data{
	pthread_mutex_t pid_mutex;
	pid_t pid;
};

int main(int argc, char *argv[]) {
	struct pid_data p;
	p.pid = getpid();
	int file_desc = shm_open("/sharedpid", O_RDWR|O_CREAT, S_IRWXU);
	ftruncate(file_desc, sizeof(struct pid_data));
	struct pid_data* dat = (struct pid_data*) mmap(0, sizeof(struct pid_data), PROT_READ|PROT_WRITE, MAP_SHARED, file_desc, 0);
	dat->pid = getpid();

	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(&dat->pid_mutex, &attr);
	printf("%i \n", dat->pid);
	printf("%i \n", p.pid);

	int* buff;
	int* send;

	int channelId = ChannelCreate(0);
	int recMsgId = MsgReceive(channelId, buff, sizeof(int), NULL);
	printf("%i \n", *buff);
	*send = *buff + 2;
	MsgReply(recMsgId, 0, send, sizeof(int));

	printf("hello");

	return 0;
}
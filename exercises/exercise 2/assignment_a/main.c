#include <unistd.h>
#include <stdio.h>
// #include <stdlib.h>
// #include <sys/times.h>
#include "header.h"
// #define N_FLOORS 4

// int global_variable = 0;


int main(int argc, char **argv)
{
	int local_variable = 0;
	printf("Local Variable: %i\n", local_variable );
	printf("Global Variable: %i\n", global_variable );
	fork();
	local_variable++;
	global_variable++;
	printf("Local Variable: %i\n", local_variable );
	printf("Global Variable: %i\n", global_variable );


	return 0;
}

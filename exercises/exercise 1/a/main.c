#include <stdlib.h>
#include <stdio.h>


void allocate(int value){
	int *ptr = NULL;
	ptr = malloc(1024*1024*sizeof(char));
	*ptr = value;
	//printf("test of allocated memory: %i\n", *ptr);
}

int main(){
	printf("Starting program\n");
	int stopPoint = 1000;
	int i;
	// while(i){
	// 	allocate(i);
	// }

	for(i = 0; i <= stopPoint; i = i + 1){
		allocate(i);
	}

	printf("Stopping program\n");
	return 0;
}
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char *argv[]){
	if(argc != 2){
		printf("Użycie %s \n", argv[0]);
		return 0;
	}

	int M = atoi(argv[1]);
	pid_t pid;
	
	for(int i = 0 ; i < M;i++){
		printf("Potomek %d\n",getuid());
	}

	return 0;
}
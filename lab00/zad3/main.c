#include "definitions.h"


int main(int argc,char* argv[]){

	if(argc != 3){
		return -1;
	}

	unlink(FILE_PATH);

	int N = atoi(argv[1]);
	int M = atoi(argv[2]);

	pid_t pid;

	for(int i = 0; i < N ; i++ ){

		pid = fork();
		if(pid == 0){
			execl("./child","./child",argv[2],NULL);
			exit(1);
		}
	}

	for(int i = 0 ; i<N ; i++){
		wait(NULL);
	}
	printf("Rodzic  PID: %d\n", getpid());
	return 0;
}
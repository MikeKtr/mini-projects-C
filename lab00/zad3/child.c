#include "definitions.h"


int main(int argc, char *argv[]){
	if(argc != 2){
		printf("Użycie %s \n", argv[0]);
		return 0;
	}

	FILE *f = fopen(FILE_PATH,"a");



	int M = atoi(argv[1]);
	pid_t pid;

	if (flock(fileno(f), LOCK_EX) == -1)
	{
		perror("flock");
		exit(1);
	}
	
	for(int i = 0 ; i < M;i++){
		fprintf(f,"Potomek (%d)\n",getpid());
		fflush(f);
		usleep(250000);
	}

	if (flock(fileno(f), LOCK_UN) == -1)
	{
		perror("flock");
		exit(1);
	}
	fclose(f);
	return 0;
}
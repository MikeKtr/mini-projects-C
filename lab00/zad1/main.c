#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>

#define M 5

int zmiennaGlobalna = 0;

int main(int argc, 	char * argv[]){

	if(argc != 2){
		return 0;
	}

	int N = atoi(argv[1]);
	pid_t pid;

	for (int i = 0; i < N; i++) {
        pid = vfork(); 
        
        if (pid == 0) {
            break;
        } else if (pid < 0) {
            perror("Blad forka");
            exit(1);
        }
    }
    if (pid == 0) {

        zmiennaGlobalna++; 
        
        for (int j = 0; j < M; j++) {
            printf("Potomek PID: %d\n", getpid());
            usleep(250000); 
        }
        exit(0); 
        
    } else {
        for (int i = 0; i < N; i++) {
            wait(NULL);
        }
    

        printf("Rodzic PID: %d zmienna Globalna = %d \n", getpid(), zmiennaGlobalna);
    }
	return 0;
}
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>
#include <stdlib.h>

void handle(int signr){
	printf("Wywolano funkcje nr %d\n", signr);
}

void sig_default(){
	printf("Wywolano funkcje sig_default\n");
	signal(SIGUSR2,SIG_DFL);
}

void sig_mask(){
	printf("Wywolano funkcje sig_mask\n");
	sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask,SIGUSR2);
	sigprocmask(SIG_BLOCK,&mask,NULL);
}

void sig_ignore(){
	printf("Wywolano funkcje sig_ignore\n");
	signal(SIGUSR2,SIG_IGN);
}

void sig_handle(){
	printf("Wywolano funkcje sig_handle\n");
	signal(SIGUSR2,handle);
}

int main(int argc, char* argv[]){
	if(argc != 2){
		return -1;
	}

	if(strcmp(argv[1],"default") == 0){
		sig_default();
	}
	else if(strcmp(argv[1],"mask") == 0){
		sig_mask();
	}
	else if(strcmp(argv[1],"ignore") == 0){
		sig_ignore();
	}
	else if(strcmp(argv[1],"handle") == 0){
		sig_handle();
	}

	pid_t pid = fork();
	if(pid == 0){
		execl("./child","./child",argv[1],NULL);
		kill(getpid(),SIGUSR2);
	}
	wait(NULL);
	return 0;
}
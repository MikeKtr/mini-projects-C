#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void sig_unblock(){
	sigset_t new_mask;
	sigemptyset(&new_mask);
	sigaddset(&new_mask,SIGUSR1);
	sigprocmask(SIG_UNBLOCK,&new_mask,NULL);
	printf("Odblokowano!\n");
}

void handle(int signr){
	printf("Wywolano funkcje nr %d\n", signr);
}

void sig_default(){
	printf("Wywolano funkcje sig_default\n");
	signal(SIGUSR1,SIG_DFL);
}

void sig_mask(){
	printf("Wywolano funkcje sig_mask\n");
	sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask,SIGUSR1);
	sigprocmask(SIG_BLOCK,&mask,NULL);
}

void sig_ignore(){
	printf("Wywolano funkcje sig_ignore\n");
	signal(SIGUSR1,SIG_IGN);
}

void sig_handle(){
	printf("Wywolano funkcje sig_handle\n");
	signal(SIGUSR1,handle);
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

	for(int i = 1; i <= 20 ; i++){
		printf("i = %d \n", i);
		sleep(1);

		if(i == 5 || i == 15){
			printf("Wysyłam sygnał USR1\n");
			raise(SIGUSR1);
		}
		else if(i == 10){
			printf("Odblokowywuje USR1\n");
			sig_unblock();
		}

	}
	printf("Pętla zostala wykonana w calosci\n");
}
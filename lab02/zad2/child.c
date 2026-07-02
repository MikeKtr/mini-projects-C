#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void sig_unblock(){
	sigset_t new_mask;
	sigemptyset(&new_mask);
	sigaddset(&new_mask,SIGUSR2);
	sigprocmask(SIG_UNBLOCK,&new_mask,NULL);
	printf("Odblokowano!\n");
}

int main(int argc, char* argv[]){

	for(int i = 1; i <= 20 ; i++){
		printf("i = %d \n", i);
		sleep(1);

		if(i == 5 || i == 15){
			printf("Wysyłam sygnał USR2\n");
			raise(SIGUSR2);
		}
		else if(i == 10){
			printf("Odblokowywuje USR2\n");
			sig_unblock();
		}

	}
	printf("Pętla zostala wykonana w calosci\n");
	return 0;
}
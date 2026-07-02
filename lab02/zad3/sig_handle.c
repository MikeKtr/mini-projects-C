#include <signal.h>
#include "sig_handle.h"

void handle(int signr){
	printf("Wywolano funkcje nr %d\n", signr);
}

void sig_handle(){
	printf("Wywolano funkcje sig_handle\n");
	signal(SIGUSR2,handle);
}
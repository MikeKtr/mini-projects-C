#include "sig_mask.h"
#include <signal.h>
#include <stdlib.h>

void sig_mask(){
	printf("Wywolano funkcje sig_mask\n");
	sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask,SIGUSR2);
	sigprocmask(SIG_BLOCK,&mask,NULL);
}
#include <signal.h>
#include "sig_default.h"

void sig_default(){
	printf("Wywolano funkcje sig_default\n");
	signal(SIGUSR2,SIG_DFL);
}

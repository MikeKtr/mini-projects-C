#include <signal.h>
#include "signal.h"

void sig_ignore(){
	printf("Wywolano funkcje sig_ignore\n");
	signal(SIGUSR2,SIG_IGN);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#ifdef DYNAMIC_LIB
#include <dlfcn.h>
#endif

#ifndef DYNAMIC_LIB
extern void sig_default();
extern void sig_ignore();
extern void sig_mask();
extern void sig_handle();
#endif

void sig_unblock() {
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    sigprocmask(SIG_UNBLOCK, &set, NULL);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        return 1;
    }

    char *mode = argv[1];

#ifdef DYNAMIC_LIB
    void *handle = dlopen("./libsig.so", RTLD_LAZY);

    void (*sig_func)() = NULL;
    char func_name[20];
    snprintf(func_name, 20, "sig_%s", mode);

    sig_func = dlsym(handle, func_name);
    
    printf("Wywołano funkcję '%s'\n", func_name);
    sig_func();

#else
    if (strcmp(mode, "default") == 0) sig_default();
    else if (strcmp(mode, "ignore") == 0) sig_ignore();
    else if (strcmp(mode, "mask") == 0) sig_mask();
    else if (strcmp(mode, "handle") == 0) sig_handle();
    printf("Wywołano funkcję 'sig_%s()'\n", mode);
#endif
    for (int i = 1; i <= 20; i++) {
        printf("%d\n", i);

        if (i == 5 || i == 15) {
            printf("Wysyłam sygnał USR1\n");
            raise(SIGUSR1);
        }

        if (i == 10) {
            sigset_t pending;
            sigpending(&pending);
            if (sigismember(&pending, SIGUSR1)) {
                printf("Odblokowuję USR1\n");
                sig_unblock();
            }
        }
        sleep(1);
    }

    printf("Pętla została wykonana w całości\n");

#ifdef DYNAMIC_LIB
    dlclose(handle);
#endif

    return 0;
}
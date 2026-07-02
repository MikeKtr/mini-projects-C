#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h> 
#include <fcntl.h>
#include <sys/mman.h> 
#include <sys/stat.h>
#include <signal.h>

#define MAX_K 100


int main(int argc, char *args[]){
	if(argc != 4){
		return 0;
	}
	int n = atoi(args[1]);
	int m = atoi(args[2]);
	int k = atoi(args[3]);

	struct SharedBuffer {
		char normal_data[MAX_K][11]; 
		int normal_head;
		int normal_tail;
		int normal_count; 
	
		char priority_data[MAX_K][11]; 
		int priority_head;
		int priority_tail;
		int priority_count; 
	};
	

	pid_t pid;


	sem_t *empty_normal = sem_open("emptyN",O_CREAT ,0666,k);
	sem_t *empty_priority = sem_open("emptyP",O_CREAT ,0666,k);
	sem_t *full = sem_open("full",O_CREAT ,0666,0);
	sem_t *mutex = sem_open("mutex",O_CREAT ,0666,1);

	int mem_desc = shm_open("pamiec", O_CREAT | O_RDWR, 0666);

	ftruncate(mem_desc,sizeof(struct SharedBuffer));

	struct SharedBuffer *shared = (struct SharedBuffer *)mmap(NULL,sizeof(struct SharedBuffer),PROT_READ | PROT_WRITE,MAP_SHARED,mem_desc,0);
	
	shared->normal_head = 0;
	shared->normal_tail = 0;
	shared->normal_count = 0; 
	shared->priority_head = 0;
	shared->priority_tail = 0;
	shared->priority_count = 0; 
	
	printf("wybrano %d nadawców i %d odbiorników\n",n,m);
	printf("asdfasdf");
	pid_t manager_pid = fork();

	if(manager_pid == 0){
		execl("./manager", "manager", args[3], NULL);
		perror("Błąd uruchomienia managera");
		exit(1);
	}

	for(int i = 0; i < n ; i++){
		pid = fork();

		if(pid == 0){
			break;
		}
		else if(pid < 0){
			perror("Błąd forka");
			exit(1);
		}
	}
	if(pid == 0){
		execl("./nadajnik","./nadajnik",args[3],NULL);
		perror("Błąd uruchomienia nadajnika");
    	exit(1);
	}
	

	for(int i = 0 ; i < m ; i++){
		pid = fork();

		if(pid == 0){

			char id_str[16];
			snprintf(id_str, sizeof(id_str), "%d", i);
			execl("./odbiornik","./odbiornik",args[3],id_str,NULL);
			perror("Błąd uruchomienia odbiornika");
    		exit(1);
		}
		else if(pid < 0){
			perror("Błąd forka");
			exit(1);
		}
	}




	for(int i = 0 ; i < n + m ; i++){
		wait(NULL);
	}
	kill(manager_pid, SIGTERM);
	waitpid(manager_pid, NULL, 0);

    munmap(shared, sizeof(struct SharedBuffer));
    sem_close(empty_normal);
    sem_close(empty_priority);
    sem_close(full);
    sem_close(mutex);

	sem_unlink("emptyN");
	sem_unlink("emptyP");
	sem_unlink("mutex");
	sem_unlink("full");
	shm_unlink("pamiec");

    printf("Kierownik: wszystkie zasoby usunięte, kończę pracę.\n");
    return 0;
}
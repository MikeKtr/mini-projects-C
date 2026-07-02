#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h> 
#include <fcntl.h>
#include <sys/mman.h> 
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <string.h>


#define MAX_K 100

int main(int argc,char *argv[]){
	if(argc != 3){
		return -1;
	}

	srand(time(NULL) ^ getpid());

	int k = atoi(argv[1]);
	

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

	sem_t *empty_priority = sem_open("emptyP",0);
	sem_t *empty_normal = sem_open("emptyN",0);
	sem_t *full = sem_open("full",0);
	sem_t *mutex = sem_open("mutex",0);

	int mem_desc = shm_open("pamiec",O_RDWR,0666);

	struct SharedBuffer *shared = (struct SharedBuffer *)mmap(NULL,sizeof(struct SharedBuffer),PROT_READ | PROT_WRITE,MAP_SHARED,mem_desc,0);


	sem_wait(full);
	sem_wait(mutex);
	char znaki[11];
	if(shared->priority_count > 0){
		strcpy(znaki, shared->priority_data[shared->priority_tail]);
		shared->priority_tail = (shared->priority_tail + 1) % k;
		shared->priority_count--;
		sem_post(mutex);
		sem_post(empty_priority);
		for(int i = 0; i < 10; i++) {
			printf("[odbiornik (Priorytetowa Kolejka) nr %s]: %c\n",argv[2], znaki[i]);
			fflush(stdout); 
			usleep(300000); 
		}
	}
	else{
		strcpy(znaki, shared->normal_data[shared->normal_tail]);
		shared->normal_tail = (shared->normal_tail + 1) % k;
		shared->normal_count--;
		sem_post(mutex);
		sem_post(empty_normal);
		for(int i = 0; i < 10; i++) {
			printf("[odbiornik (Zwykla kolejka) nr %s]: %c\n",argv[2], znaki[i]);
			fflush(stdout); 
			usleep(300000); 
		}
	
	}

	munmap(shared, sizeof(struct SharedBuffer));
	sem_close(empty_normal);
    sem_close(empty_priority);
    sem_close(full);
    sem_close(mutex);
	return 0;
}	
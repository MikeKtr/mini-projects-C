#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h> 
#include <fcntl.h>
#include <sys/mman.h> 
#include <sys/stat.h>
#include <time.h>
#include <string.h>


#define MAX_K 100
const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

int main(int argc,char *argv[]){
	if(argc != 2){
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

	char chars[11]; 
	for(int i = 0 ; i < 10; i++){
		
		chars[i] = 'a' + (rand() % 26);
	}
	chars[10] = '\0';


	if(rand() % 100 <= 30){
		sem_wait(empty_priority);
		sem_wait(mutex);
		shared->priority_count++;
		strcpy(shared->priority_data[shared->priority_head],chars);
		shared->priority_head = (shared->priority_head + 1) % k;
		sem_post(mutex);
		sem_post(full);
	}
	else{
		sem_wait(empty_normal);
		sem_wait(mutex);
		shared->normal_count++;

		strcpy(shared->normal_data[shared->normal_head],chars);
		shared->normal_head = (shared->normal_head + 1) % k;
		sem_post(mutex);
		sem_post(full);
	}

	munmap(shared, sizeof(struct SharedBuffer));
	sem_close(empty_normal);
    sem_close(empty_priority);
    sem_close(full);
    sem_close(mutex);
	return 0;
}	
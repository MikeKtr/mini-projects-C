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

int main(int argc, char *argv[]){

	if(argc != 2){
		return -1;
	}

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
	sem_t *mutex = sem_open("mutex",0);


	int mem_desc = shm_open("pamiec",O_RDWR,0666);

	struct SharedBuffer *shared = (struct SharedBuffer *)mmap(NULL,sizeof(struct SharedBuffer),PROT_READ | PROT_WRITE,MAP_SHARED,mem_desc,0);


	while(1){
		printf("[Manager]: Zwykla kolejka: %d, Kolejka Priorytetowa: %d\n",shared->normal_count,shared->priority_count);
		fflush(stdout); 
		if(sem_trywait(empty_priority) == 0){
			
			sem_wait(mutex);
			if(shared->normal_count > 0){
				strcpy(shared->priority_data[shared->priority_head],shared->normal_data[shared->normal_tail]);
				shared->normal_tail = (shared->normal_tail + 1) % k;
				shared->priority_head = (shared->priority_head + 1) % k;
				shared->normal_count--;
				shared->priority_count++;
				sem_post(empty_normal);
			}
			else{
				sem_post(empty_priority);
				
			}
			sem_post(mutex);
		}
		
		usleep(5000000);
	}
	
	munmap(shared, sizeof(struct SharedBuffer));
	sem_close(empty_normal);
    sem_close(empty_priority);
    sem_close(mutex);

	return 0;
}
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <semaphore.h> 
#include <fcntl.h>
#include <sys/mman.h> 
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

bool is_running = false;
struct timespec ts;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct robot_state{
    double x;
    double y;
    double orientation;
} robot_state;

typedef struct robot_bufor{
    robot_state buffer[20];
    int head;
    int tail; 
}robot_bufor;

struct frame {
    int id;
    long long time;
};

char left_id_str[50] = "";
char right_id_str[50] = "";

struct frame right_frame;
struct frame left_frame; 

struct robot_bufor robot_b = {.head = 0, .tail = 0};

void * camera(void *arg) { 
    int id = 0;
    
    void **args = (void **)arg;
    sem_t *buffer_empty = (sem_t *)args[0];
    sem_t *buffer_full  = (sem_t *)args[1];
    struct frame *my_frame = (struct frame *)args[2];
    
    while(is_running){
        clock_gettime(CLOCK_REALTIME, &ts);
        long long time = (long long)ts.tv_sec * 1000000000L + ts.tv_nsec;
        
        sem_wait(buffer_empty); 
        
        pthread_mutex_lock(&mutex);
        my_frame->id = id;
        my_frame->time = time;
        id++;
        pthread_mutex_unlock(&mutex); 
        
        sem_post(buffer_full); 
        
        usleep(40000);
    }
    return NULL;
}

void * synchronizer(void *arg){

    sem_t **semaphores = (sem_t **)arg;
    sem_t *left_buffer_full = semaphores[0];
    sem_t *right_buffer_full = semaphores[1];

    sem_t *right_buffer_empty = semaphores[2];
    sem_t *left_buffer_empty = semaphores[3];

    sem_t *save_full = semaphores[4];

    while(is_running){
        
        long long left_time;
        long long right_time;

        int left_id;
        int right_id;

        sem_wait(left_buffer_full);
        pthread_mutex_lock(&mutex);
        left_time = left_frame.time;
        left_id = left_frame.id;
        pthread_mutex_unlock(&mutex); 
        sem_post(left_buffer_empty);

        sem_wait(right_buffer_full);
        pthread_mutex_lock(&mutex);
        right_time = right_frame.time;
        right_id = right_frame.id;
        pthread_mutex_unlock(&mutex);
        sem_post(right_buffer_empty);


        if(llabs(right_time - left_time) <= 20000000){
            printf("Ramki synchronicznie\n");
            char left_temp[50];
            char right_temp[50]; 
            
            snprintf(left_temp, sizeof(left_temp), "left_%d.jpg", left_id);
            snprintf(right_temp, sizeof(right_temp), "right_%d.jpg", right_id);
            
            pthread_mutex_lock(&mutex);
            strcpy(right_id_str, right_temp);
            strcpy(left_id_str, left_temp);
            pthread_mutex_unlock(&mutex); 
            
            sem_post(save_full);
        }
        else{
            printf("ramki niesynchronicznie\n");
        }
    }
    return NULL;
}

void * saver(void * arg){
    struct timespec next_time;

    char local_left_id[50];
    char local_right_id[50]; 

    clock_gettime(CLOCK_MONOTONIC, &next_time);

    while(is_running){
        next_time.tv_nsec += 100000000;
        if (next_time.tv_nsec >= 1000000000) {
            next_time.tv_sec += 1;
            next_time.tv_nsec -= 1000000000;
        }
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next_time, NULL);
        pthread_mutex_lock(&mutex);

        strcpy(local_left_id, left_id_str);
        strcpy(local_right_id, right_id_str);
        
        left_id_str[0] = '\0';
        right_id_str[0] = '\0';
        pthread_mutex_unlock(&mutex);


        if (local_left_id[0] != '\0' && local_right_id[0] != '\0') {
            FILE *left_file = fopen(local_left_id, "w");
            FILE *right_file = fopen(local_right_id, "w");

            if (left_file != NULL && right_file != NULL) {
                
                fprintf(left_file, "%s\n", local_left_id);
                fprintf(right_file, "%s\n", local_right_id);
                fclose(left_file);
                fclose(right_file);
                
                printf("Wykonano zapis plików: %s oraz %s\n", local_left_id, local_right_id);
            } 
            else {
                if (left_file != NULL) fclose(left_file);
                if (right_file != NULL) fclose(right_file);
                printf("Błąd: Nie można utworzyć plików na dysku.\n");
            }
        }

    }
    return NULL;

    
}

void * robot_logger(void * arg){
    sem_t **semaphores = (sem_t **)arg;
    sem_t *robot_full = semaphores[0];
    struct robot_state curr_states[20];
    int local_head = 0;
    int local_tail = 0;
    while(is_running){
        FILE *fptr = fopen("logi.txt","a");
        sem_wait(robot_full);
        pthread_mutex_lock(&mutex);

        local_head = robot_b.head;

        for(int i = 0 ; i < local_head ; i++){
            curr_states[i] = robot_b.buffer[i];
        }
        robot_b.head = 0;
        pthread_mutex_unlock(&mutex);

        for(int i = 0 ; i < local_head ; i++){
            fprintf(fptr,"X: %f, Y: %f, Orient: %f \n",curr_states[i].x,curr_states[i].y,curr_states[i].orientation);
        }
        fclose(fptr);
        usleep(10000);
    }
    return NULL;
}

void * robot(void * arg){

    sem_t **semaphores = (sem_t **)arg;
    sem_t *robot_full = semaphores[0];

    robot_state curr_state;
    curr_state.x = 0.0;
    curr_state.y = 0.0;
    curr_state.orientation = 0.0;


    while(is_running){
        curr_state.x = curr_state.x + (rand() % 2) - 1.0;
        curr_state.y = curr_state.y + (rand() % 2) - 1.0;
        curr_state.orientation = curr_state.orientation + (rand() % 10) - 5; 
        pthread_mutex_lock(&mutex);
        robot_b.buffer[robot_b.head] = curr_state;
        robot_b.head += 1;
        sem_post(robot_full);
        pthread_mutex_unlock(&mutex);
        usleep(100000);
    }
    return NULL;
}

int main(int argc, char *args[]){    
    is_running = true;

    sem_t left_empty, left_full;
    sem_t right_empty, right_full;
    sem_t save_full;
    sem_t robot_full;

    sem_init(&left_empty, 0, 1);
    sem_init(&left_full, 0, 0);
    sem_init(&right_empty, 0, 1);
    sem_init(&right_full, 0, 0);
    sem_init(&save_full, 0, 0);
    sem_init(&robot_full, 0, 0);

    void *left_cam_args[3] = { &left_empty, &left_full, &left_frame };
    void *right_cam_args[3] = { &right_empty, &right_full, &right_frame };
    
    sem_t *sync_args[5] = { &left_full, &right_full, &right_empty, &left_empty, &save_full };
    sem_t *saver_args[5] = { &save_full }; 

    sem_t *robot_args[1] = {&robot_full};
    sem_t *logger_args[1] = {&robot_full};

    pthread_t thread_left_cam;
    pthread_t thread_right_cam;
    pthread_t thread_sync;
    pthread_t thread_saver;

    pthread_t thread_robot;
    pthread_t thread_logger;

    pthread_create(&thread_left_cam, NULL, camera, (void *)left_cam_args);
    pthread_create(&thread_right_cam, NULL, camera, (void *)right_cam_args);
    pthread_create(&thread_sync, NULL, synchronizer, (void *)sync_args);
    pthread_create(&thread_saver, NULL, saver, (void *)saver_args);
    pthread_create(&thread_robot,NULL,robot,(void *)robot_args);
    pthread_create(&thread_logger,NULL,robot_logger,(void *)logger_args);

    sleep(5);

    is_running = false;

    sem_post(&left_empty); sem_post(&left_full);
    sem_post(&right_empty); sem_post(&right_full);
    sem_post(&save_full);

    pthread_join(thread_left_cam, NULL);
    pthread_join(thread_right_cam, NULL);
    pthread_join(thread_sync, NULL);
    pthread_join(thread_saver, NULL);
    pthread_join(thread_robot,NULL);
    pthread_join(thread_logger,NULL);

    sem_destroy(&left_empty);
    sem_destroy(&left_full);
    sem_destroy(&right_empty);
    sem_destroy(&right_full);
    sem_destroy(&save_full);
    sem_destroy(&robot_full);

    return 0;
}
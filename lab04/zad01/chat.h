#ifndef CHAT_H
#define CHAT_H

#include <mqueue.h>

#define SERVER_Q "/server_queue"

#define MAX_CLIENTS 10


typedef enum {
    INIT,
    MESSAGE
} msg_type_t;

typedef struct {
    msg_type_t type;       
    int client_id;         
    char text[512];        
} chat_msg_t;

typedef struct {
    mqd_t queue_fd;      
    int is_active;       
} client_info_t;

#endif 
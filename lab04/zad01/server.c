#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>           
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>
#include "chat.h"

int main(){
	client_info_t clients[MAX_CLIENTS];
	int current_client = 0;

	//serwer
	const char *name = "/server_queue";

	
	struct mq_attr attr;
	attr.mq_flags = 0;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = sizeof(chat_msg_t);
	attr.mq_curmsgs = 0;


	printf("Uruchomiono serwer");	
	fflush(stdout);
	mqd_t server_q = mq_open(name,O_RDONLY | O_CREAT,0666,&attr);

	chat_msg_t message;
	while(1){
		ssize_t bytes_read = mq_receive(server_q,(char*)&message,sizeof(chat_msg_t),NULL);

		if (bytes_read == -1) {
            perror("Blad w mq_receive");
            continue; 
        }
		
		msg_type_t m_type = message.type;
		int m_id = message.client_id;

		chat_msg_t response;

		switch (m_type)
		{
		case INIT:
			if (current_client >= MAX_CLIENTS) {
				printf("Log: Brak wolnych miejsc dla klientow!\n");
				break;
			}
			mqd_t cl_q = mq_open(message.text,O_WRONLY,0666,&attr);
			
			clients[current_client].is_active = 1;
			clients[current_client].queue_fd = cl_q;
			
			printf("Dodano użytkownika %d",current_client);
			fflush(stdout);
			response.type = INIT;
			response.client_id = current_client;

			mq_send(cl_q,(const char *)&response,sizeof(chat_msg_t),0);
			
			
			current_client++;
			break;
		
		case MESSAGE:
	
			response.type = MESSAGE;
			strcpy(response.text, message.text);

			for(int i = 0 ; i < MAX_CLIENTS ; i++){
				if(clients[i].is_active == 1 && message.client_id != i){
					mq_send(clients[i].queue_fd,(const char *)&message,sizeof(chat_msg_t),0);
				}

			}


			break;
		}
	}


	mq_close(server_q);
    mq_unlink(name);

	return 0;
}
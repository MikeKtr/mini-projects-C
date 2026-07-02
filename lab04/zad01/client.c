#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>           
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>

#include "chat.h"

int generate_numeric_id() {
    return (rand() % (10000)) + 100;
}

int main(){

	struct mq_attr attr;
	attr.mq_flags = 0;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = sizeof(chat_msg_t);
	attr.mq_curmsgs = 0;

	char client_name[50];
	snprintf(client_name, sizeof(client_name), "/client_%d", getpid());

	const char *name = SERVER_Q;

	mqd_t server_q = mq_open(name,O_WRONLY,0666,&attr);
	mqd_t client_q = mq_open(client_name,O_CREAT | O_RDONLY,0666,&attr);

	chat_msg_t init_message;
	init_message.client_id =  -1;
	init_message.type = INIT;
	strcpy(init_message.text,client_name);	

	mq_send(server_q,(char *)&init_message,sizeof(chat_msg_t),0);
	
	
	
	chat_msg_t handshake_message;

	ssize_t bytes_read = mq_receive(client_q,(char*)&handshake_message,sizeof(chat_msg_t),NULL);
	if(bytes_read == -1) {
		perror("Blad w mq_receive");
	}


	int my_server_id = handshake_message.client_id;
	pid_t pid = fork();




	if(pid == 0){
		chat_msg_t incoming_msg;		
		while(1){
			ssize_t bytes_read = mq_receive(client_q, (char*)&incoming_msg, sizeof(chat_msg_t), NULL);
			if(bytes_read == -1) {
				perror("Blad w mq_receive");
				continue; 
			}
			
			msg_type_t m_type = incoming_msg.type;
			switch (m_type)
			{
			case MESSAGE:
			printf("\e[2K\r");
				printf("[Klient %d]: %s", incoming_msg.client_id, incoming_msg.text);
				printf("[Klient %d]: ",my_server_id);
				fflush(stdout);
				break;
			}

		}
	}
	else{
		while(1){
		char input_buffer[512];
		printf("[Klient %d]:",my_server_id);
		fflush(stdout);
		if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
			exit(0);
		}
		printf("\e[1A\e[2K\r");
		chat_msg_t response;
		response.type = MESSAGE;
		response.client_id = my_server_id;
		strcpy(response.text,input_buffer);

		mq_send(server_q,(char *)&response,sizeof(chat_msg_t),NULL);
		}
	}


	return 0;	
}
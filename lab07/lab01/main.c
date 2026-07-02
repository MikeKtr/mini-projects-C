#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

int LICZNIK_ZAPYTAN = 0;

int main(int argc, char* argv[]){
	int socket_desc = socket(AF_INET,SOCK_STREAM,0);
	if(socket_desc == -1){
		printf("Błąd otwierania socketa");
		return -1;
	}

	
	int opt = 1;
	if(setsockopt(socket_desc,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(int)) == -1){
		printf("Błąd otwierania socketa");
		return -1;
	}
	
	
	struct sockaddr_in socket_addr;

	socket_addr.sin_family = AF_INET;
	socket_addr.sin_port = htons(9000);
	socket_addr.sin_addr.s_addr = INADDR_ANY;


	if(bind(socket_desc,(struct sockaddr *)&socket_addr,sizeof(struct sockaddr_in)) == -1){
		printf("Błąd otwierania socketa");
		return -1;
	}
	listen(socket_desc,10);
	while(1){
		struct sockaddr_in client_addr;
		socklen_t client_len = sizeof(struct sockaddr_in);
		int client_desc = accept(socket_desc,(struct sockaddr *)&client_addr,&client_len);
		if(client_desc == -1){
			printf("Błąd połączenia z klientem");
			return -1;
		}
		char buffer[2048] = {0};
		size_t bytes = read(client_desc,buffer,sizeof(buffer));

		if(strncmp(buffer, "GET", 3) == 0){
			LICZNIK_ZAPYTAN++;
			char response_body[128];
			sprintf(response_body, "Liczba pobrań strony: %d", LICZNIK_ZAPYTAN);
			int body_length = strlen(response_body);
			char full_response[1024];
			sprintf(full_response,
					"HTTP/1.1 200 OK\r\n"
					"Server: Zajeciowy serwer SO\r\n"
					"Content-Type: text/plain; charset=utf-8\r\n"
					"Connection: close\r\n"
					"Cache-Control: no-store\r\n"
					"Content-Length: %d\r\n\r\n"
					"%s",                        
					body_length, response_body);
			write(client_desc,full_response,strlen(full_response));
		}
		else if (strncmp(buffer, "ZADANIE", 7) == 0)
		{
			int wartość;
			sscanf(buffer, "ZADANIE %d", &wartość);
			LICZNIK_ZAPYTAN += wartość;
			char response_body[128];
			sprintf(response_body, "Liczba pobrań strony: %d", LICZNIK_ZAPYTAN);
			write(client_desc,response_body,strlen(response_body));
		}
		close(client_desc);
		

	}
	return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

int main(int argc, char* argv[]) {
    int socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        perror("Błąd otwierania socketa");
        return -1;
    }
    
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9000);
    
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        printf("Niepoprawny adres IP\n");
        close(socket_desc);
        return -1;
    }

    if (connect(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Błąd połączenia z serwerem");
        close(socket_desc);
        return -1;
    }

    char input[2048];
    printf("Wpisz żądanie (np. GET lub ZADANIE 5): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        close(socket_desc);
        return -1;
    }

    input[strcspn(input, "\n")] = 0;

    
    if (strncmp(input, "GET", 3) == 0) {
        char http_request[1024];
        sprintf(http_request, "GET / HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n");
        write(socket_desc, http_request, strlen(http_request));
    } 
    else if (strncmp(input, "ZADANIE", 7) == 0) {
       
        write(socket_desc, input, strlen(input));
    } 
    else {
        printf("Nieznana komenda. Zamykanie połączenia.\n");
        close(socket_desc);
        return 0;
    }

    printf("\n--- ODPOWIEDŹ SERWERA ---\n");
    char buffer[2048];
    ssize_t bytes_read;
    
    while ((bytes_read = read(socket_desc, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0'; 
        printf("%s", buffer);
    }
    printf("\n-------------------------\n");

    if (bytes_read == -1) {
        perror("Błąd podczas odczytu z gniazda");
    }

    close(socket_desc);
    return 0;
}
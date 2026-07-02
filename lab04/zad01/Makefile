CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS = -lrt

SERVER = server
CLIENT = client

all: $(SERVER) $(CLIENT)

$(SERVER): server.c chat.h
	$(CC) $(CFLAGS) server.c -o $(SERVER) $(LDFLAGS)

$(CLIENT): client.c chat.h
	$(CC) $(CFLAGS) client.c -o $(CLIENT) $(LDFLAGS)

clean:
	rm -f $(SERVER) $(CLIENT)

.PHONY: all clean
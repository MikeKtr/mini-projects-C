CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -pthread -lrt

all: main nadajnik odbiornik manager

main: main.c
	$(CC) $(CFLAGS) -o main main.c $(LDFLAGS)

nadajnik: nadajnik.c
	$(CC) $(CFLAGS) -o nadajnik nadajnik.c $(LDFLAGS)

odbiornik: odbiornik.c
	$(CC) $(CFLAGS) -o odbiornik odbiornik.c $(LDFLAGS)

manager: manager.c
	$(CC) $(CFLAGS) -o manager manager.c $(LDFLAGS)

clean:
	rm -f main nadajnik odbiornik manager
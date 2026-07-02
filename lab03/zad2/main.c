#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define FIFO_REQ "/tmp/fifo_req"
#define FIFO_RES "/tmp/fifo_res"

int main(int argc, char* argv[]){
	double a, b, h;

	mkfifo(FIFO_REQ, 0666);
    mkfifo(FIFO_RES, 0666);

    printf("Podaj poczatek przedzialu: ");
    if (scanf("%lf", &a) != 1) return 1;
    printf("Podaj koniec przedzialu: ");
    if (scanf("%lf", &b) != 1) return 1;
    printf("Podaj dokladnosc: ");
    if (scanf("%lf", &h) != 1) return 1;

	int fd_req = open(FIFO_REQ, O_WRONLY);
    write(fd_req, &a, sizeof(double));
    write(fd_req, &b, sizeof(double));
    write(fd_req, &h, sizeof(double));
    close(fd_req);


    double wynik;
    int fd_res = open(FIFO_RES, O_RDONLY);
    read(fd_res, &wynik, sizeof(double));
    close(fd_res);

    printf("Otrzymany wynik calki to: %f\n", wynik);


    unlink(FIFO_REQ);
    unlink(FIFO_RES);

    return 0;
}
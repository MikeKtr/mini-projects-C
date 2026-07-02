#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define FIFO_REQ "/tmp/fifo_req"
#define FIFO_RES "/tmp/fifo_res"

double F(double x) {
    return 4.0 / (x * x + 1.0);
}

int main() {
    double a, b, h;

    // Otwarcie potoku do odczytu danych
    int fd_req = open(FIFO_REQ, O_RDONLY);
    read(fd_req, &a, sizeof(double));
    read(fd_req, &b, sizeof(double));
    read(fd_req, &h, sizeof(double));
    close(fd_req);

    printf("przedzial [%f, %f] z krokiem %f.\n", a, b, h);

    double sum = 0.0;
    double j;
    

    for (j = a; j + h <= b; j += h) {
        sum += F(j + (h / 2.0)) * h;
    }


    double resztka_szer = b - j;
    if (resztka_szer > 0.0) {
        sum += F(j + (resztka_szer / 2.0)) * resztka_szer;
    }


    int fd_res = open(FIFO_RES, O_WRONLY);
    write(fd_res, &sum, sizeof(double));
    close(fd_res);


    return 0;
}
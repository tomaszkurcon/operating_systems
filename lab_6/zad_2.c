#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>



int main(int argc, char *argv[]) {
    int fd;
    char * myfifo = "/tmp/myfifo";
    mkfifo(myfifo, 0666);

    double a, b;

    printf("Podaj przedział całkowania [a,b]:");
    scanf("%lf %lf", &a, &b);
    fd = open(myfifo, O_WRONLY);
    write(fd, &a, sizeof(double));
    write(fd, &b, sizeof(double));
    close(fd);

    fd = open(myfifo, O_RDONLY);
    double result;
    read(fd, &result, sizeof(double));
    close(fd);

    printf("Wynik całkowania: %f\n", result);

    remove(myfifo);

    return 0;
}
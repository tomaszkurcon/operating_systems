//#include <stdio.h>
//#include <stdlib.h>
//#include <unistd.h>
//#include <sys/wait.h>
//
//double func(double x) {
//    return 4/(x*x+1);
//}
//
//double calc_integral(double a,double b, double h) {
//    double sum = 0;
//    while(a+h <= b) {
//        double func_value = func(a+h/2);
//        sum+=func_value*h;
//        a+=h;
//    }
//    double rest = b-a;
//    sum+=func(a+(b-a)/2)*rest;
//    return sum;
//}
//
//int main(int argc, char *argv[]) {
//    int n = atoi(argv[2]);
//    double h = atof(argv[1]);
//    double range = 1.0/n;
//    int fd[2*n];
//    pid_t pid;
//
//    for(int i=0; i<n; i++) {
//        if(pipe(fd + 2*i) < 0) {
//            perror("pipe error");
//            exit(EXIT_FAILURE);
//        }
//
//        if((pid = fork()) < 0) {
//            perror("fork error");
//            exit(EXIT_FAILURE);
//        }
//
//        if(pid == 0) {
//            close(fd[2*i]);
//            double calc = calc_integral(i*range, i*range+range, h);
//            write(fd[2*i+1], &calc, sizeof(double));
//            close(fd[2*i+1]);
//            exit(EXIT_SUCCESS);
//        }
//    }
//
//    double total = 0.0;
//    for(int i=0; i<n; i++) {
//        close(fd[2*i + 1]);
//        double sum;
//        read(fd[2*i], &sum, sizeof(sum));
//        close(fd[2*i]);
//        total += sum;
//    }
//
//    printf("Całka: %f\n", total);
//
//    return 0;
//}


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

double func(double x) {
    return 4/(x*x+1);
}

double calc_integral(double a,double b, double h) {
    double sum = 0;
    while(a+h <= b) {
        double func_value = func(a+h/2);
        sum+=func_value*h;
        a+=h;
    }
    double rest = b-a;
    sum+=func(a+(b-a)/2)*rest;
    return sum;
}

int main(int argc, char *argv[]) {
    int pipe_fd;
    char * myfifo = "/tmp/myfifo";
    double a, b;

    pipe_fd = open(myfifo, O_RDONLY);
    read(pipe_fd, &a, sizeof(double));
    read(pipe_fd, &b, sizeof(double));

    close(pipe_fd);

    int n = atoi(argv[2]);
    double h = atof(argv[1]);
    double range = (b-a)/n;
    int fd[2*n];
    pid_t pid;

    for(int i=0; i<n; i++) {
        if(pipe(fd + 2*i) < 0) {
            perror("pipe error");
            exit(EXIT_FAILURE);
        }

        if((pid = fork()) < 0) {
            perror("fork error");
            exit(EXIT_FAILURE);
        }

        if(pid == 0) {
            close(fd[2*i]);
            double calc = calc_integral(a+i*range, a+i*range+range, h);
            write(fd[2*i+1], &calc, sizeof(double));
            close(fd[2*i+1]);
            exit(EXIT_SUCCESS);
        }
    }

    double total = 0.0;
    for(int i=0; i<n; i++) {
        close(fd[2*i + 1]);
        double sum;
        read(fd[2*i], &sum, sizeof(sum));
        close(fd[2*i]);
        total += sum;
    }


    pipe_fd = open(myfifo, O_WRONLY);
    write(pipe_fd, &total, sizeof(double));
    close(pipe_fd);
    return 0;
}







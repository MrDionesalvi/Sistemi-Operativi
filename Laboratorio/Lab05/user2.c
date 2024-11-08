#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

#define FIFOSIZE 128
int main(){
    int fd1;
    char *myfifo = "/tmp/myfifo";
    mkfifo(myfifo, 0666);
    char arr1[FIFOSIZE], arr2[FIFOSIZE];
    while (1)
    {
        fd1 = open(myfifo, O_RDONLY); // apertura FIFO
        read(fd1, arr1, FIFOSIZE);    // lettura da FIFO su arr1 e chiusura
        close(fd1);
        printf("User1: %s\n", arr1);
        fd1 = open(myfifo, O_WRONLY);       // apertura FIFO in scrittura
        fgets(arr2, FIFOSIZE, stdin);       // lettura da stdin su arr2
        write(fd1, arr2, strlen(arr2) + 1); // scrittura su FIFO
        close(fd1);
    }
    return 0;
}
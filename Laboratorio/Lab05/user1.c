#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

#define FIFOSIZE 128

int main(){
    int fd;
    char *myfifo = "/tmp/myfifo";
    mkfifo(myfifo, 0666);
    char arr1[FIFOSIZE], arr2[FIFOSIZE];
    while (1){
        fd = open(myfifo, O_WRONLY);       // apertura FIFO in scrittura
        fgets(arr2, FIFOSIZE, stdin);      // lettura da stdin su arr2
        write(fd, arr2, strlen(arr2) + 1); // scrittura e chiusura FIFO
        close(fd);
        fd = open(myfifo, O_RDONLY);  // apertura FIFO in lettura
        read(fd, arr1, sizeof(arr1)); // lettura da FIFO su arr1
        printf("User2: %s\n", arr1);
        close(fd); // chiusura FIFO
    }
    return 0;
}
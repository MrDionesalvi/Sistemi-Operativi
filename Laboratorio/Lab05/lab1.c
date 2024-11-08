#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>

#define MAXLINE 4096

void ddErrExit(const char *msg){
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv){
    int n;
    int fd[2];
    pid_t pid;
    char line[MAXLINE];
    if (pipe(fd) < 0)
        ddErrExit("pipe error");
    if ((pid = fork()) < 0){
        ddErrExit("fork error");
    }
    else if (pid > 0) { // --------- padre ---
        close(fd[0]); // chiudo il lato di lettura
        sleep(10);
        write(fd[1], "hello world\n", 12); // scrivo sul lato di scrittura
    }
    else { // ---------------------- figlio ---
        close(fd[1]); // chiudo il lato di scrittura
        n = read(fd[0], line, MAXLINE); // leggo dal lato di lettura
        write(STDOUT_FILENO, line, n);
    }
    exit(EXIT_SUCCESS);
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
    int pfd[2];
    char buf[1024];

    if (pipe(pfd) == -1){
        perror("pipe");
        exit(1);
    }

    if(argc <  4 || argc > 4 || strcmp(argv[1], "--help") == 0){
        printf("usage: %s <time_to_sleep> <time_to_sleep> <time_to_sleep>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    // Crea 3 figli
    for (int i = 0; i < 3; i++){
        pid_t pid = fork();
        if (pid == -1){
            exit(1);
        }
        if (pid == 0){
            printf("Child %d spawned\n", i);
            // Processo figlio
            if (close(pfd[0]) == -1){
                perror("close");
                exit(1);
            }
            // Wait 
            sleep(atoi(argv[i+1]));
            printf("Child %d closing after waited %d seconds\n", i, atoi(argv[i+1]));
            if (close(pfd[1]) == -1){
                perror("close");
                exit(1);
            }
            exit(0);
        }
    }

    // Processo padre
    if (close(pfd[1]) == -1){
        perror("close");
        exit(1);
    }
    ssize_t got = read(pfd[0], buf, sizeof(buf));
    printf("Parent read %ld bytes\n", got);
    if (got == -1){
        perror("read");
        exit(1);
    }
    if (got == 0){
        printf("Parent syncronized :D\n");
    } else {
        buf[got] = '\0';
        printf("Read: %s\n", buf);
    }
}
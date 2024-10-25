#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    pid_t pid;
    int status;
    switch (pid = fork()){
        case -1:
            perror("fork");
            exit(1);
        case 0:
            printf("Child process\n");
            sleep(5);
            break;
        default:
            printf("Parent process -  Mando SIGKILL a mio figlio\n");
            kill(pid, SIGKILL);
            if (waitpid(pid, &status, 0) == -1) {
                perror("Errore in waitpid");
            } else {
                if (WIFSIGNALED(status)) {
                    printf("Il processo figlio con PID %d è stato terminato dal segnale %d\n", pid, WTERMSIG(status));
                } else if (WIFEXITED(status)) {
                    printf("Il processo figlio con PID %d è terminato con stato %d\n", pid, WEXITSTATUS(status));
                } else {
                    printf("Il processo figlio con PID %d è terminato in modo sconosciuto\n", pid);
                }
            }
            break;
    }
}
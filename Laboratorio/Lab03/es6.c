#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

int main() {
    pid_t pid;
    int status;

    printf("Sono il processo padre, PID: %d\n", getpid());

    switch (pid = fork()) {
        case -1:
            perror("Errore nella fork");
            exit(1);
        case 0:
            printf("(Muoio subito) figlio, PID: %d\n", getpid());
            exit(0);
        default:
            sleep(10);
            kill(pid, SIGKILL);
            printf("Segnale SIGKILL inviato al processo figlio con PID %d\n", pid);

            // Verifica lo stato del figlio
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
    }

    return 0;
}
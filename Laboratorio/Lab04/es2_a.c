#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

// Gestore di segnali per il processo genitore
void sigchld_handler(int sig) {
    int status;
    pid_t pid;

    // Attende il cambiamento di stato del figlio
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        if (WIFEXITED(status)) {
            printf("Child %d exited with status %d\n", pid, WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Child %d killed by signal %d\n", pid, WTERMSIG(status));
        } else if (WIFSTOPPED(status)) {
            printf("Child %d stopped by signal %d\n", pid, WSTOPSIG(status));
        } else if (WIFCONTINUED(status)) {
            printf("Child %d continued\n", pid);
        }
    }
}

int main() {
    pid_t pid;

    // Imposta il gestore di segnali per SIGCHLD
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    // Crea un processo figlio
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Codice del processo figlio
        printf("Child process (PID: %d) started\n", getpid());
        while (1) {
            pause(); // Attende segnali
        }
    } else {
        // Codice del processo genitore
        printf("Parent process (PID: %d) created child (PID: %d)\n", getpid(), pid);
        while (1) {
            pause(); // Attende segnali
        }
    }

    return 0;
}
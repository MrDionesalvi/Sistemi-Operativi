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
            execlp("./saluta_persone", "saluta_persone", "paolo", "giuseppe", "mario", NULL);
            //execvp("./saluta_persone", (char *[]){"saluta_persone", "paolo", "giuseppe", "mario", NULL});
            exit(0);
        default:

    }

    return 0;
}
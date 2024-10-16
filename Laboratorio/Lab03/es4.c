#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>

int is_number(const char *str) {
    while (*str) {
        if (!isdigit(*str)) {
            return 0;
        }
        str++;
    }
    return 1;
}

int main(int argc, char const *argv[]) {
    pid_t pid;
    int i, num_children;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <number_of_children>\n", argv[0]);
        exit(1);
    }

    if (!is_number(argv[1])) {
        fprintf(stderr, "Error: Argument must be a number\n");
        exit(1);
    }

    // Creazione di 3 figli
    for (i = 0; i < atoi(argv[1]); i++) {
        pid = fork();

        if (pid < 0) {
            perror("Errore nella fork");
            exit(1);
        }

        if (pid == 0) {
            printf("Sono il figlio %d, PID: %d\n", i + 1, getpid());
            int valore = getpid();
            valore = valore % 10; // Ultima cifra
            sleep(valore);  // Il figlio dorme per un tempo variabile
            exit(0);  // Il figlio termina subito
        }
    }

    // Codice del padre
    for (i = 0; i < 3; i++) {
        pid_t terminated_pid = wait(NULL);  // Attende la terminazione di ciascun figlio
        if (terminated_pid > 0) {
            printf("Il processo figlio con PID %d è terminato\n", terminated_pid);
        }
    }

    printf("Tutti i processi figli sono terminati.\n");

    return 0;
}

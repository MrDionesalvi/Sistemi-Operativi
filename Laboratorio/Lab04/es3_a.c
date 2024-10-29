#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char const *argv[]){
    if(argc <  2){
        fprintf(stderr, "Usage: %s <pid>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    pid_t P = atoi(argv[1]);
    if (kill(P, 0) == 0) {
        printf("Il processo con PID %d esiste e abbiamo i permessi per inviargli segnali.\n", P);
    } else {
        // Controllo del tipo di errore con errno
        if (errno == ESRCH) {
            printf("Il processo con PID %d non esiste.\n", P);
        } else if (errno == EPERM) {
            printf("Il processo con PID %d esiste, ma non abbiamo i permessi per inviargli segnali.\n", P);
        } else {
            perror("Errore sconosciuto");  // Gestione di errori diversi
        }
    }
        
    return 0;
}

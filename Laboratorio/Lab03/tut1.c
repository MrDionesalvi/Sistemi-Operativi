#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int idata = 111; // allocata nel segmento dati
int main(int argc, char *argv[]){
    int istack = 222; // allocata nello stack
    pid_t procPid;
    switch (procPid = fork()){
    case -1:
        return 0; // gestione dell'errore
    case 0:              // - - - - codice del figlio
        idata *= 3;
        istack *= 3;
        break;
    default:      // - - - - codice del genitore
        sleep(3); // lasciamo che venga eseguito il figlio
        break;
    }
    // entrambi eseguono la printf
    printf("PID=%ld %s idata=%d istack=%d\n", (long)getpid(),
           (procPid == 0) ? "(child)" : "(parent)", idata,
           istack);
    exit(EXIT_SUCCESS);
}

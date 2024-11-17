#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define ddErrExit(msg) {perror(msg); exit(EXIT_FAILURE);}

#define KEY_VALUE 1234

int main(int argc, char *argv[]) {
    int semid;
    struct sembuf sops;

    if(argc != 2) {
        fprintf(stderr, "Usage: %s <op>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Ottenere l'ID del semaforo generato da sem_00_gen.c
    semid = semget(KEY_VALUE, 1, 0);
    if (semid == -1)
        ddErrExit("semget");

    // Eseguire l'operazione P sul semaforo
    sops.sem_num = 0;
    sops.sem_op = atoi(argv[1]);
    sops.sem_flg = 0;
    if (semop(semid, &sops, 1) == -1)
        ddErrExit("semop");

    // Get the value of the semaphore
    int semval = semctl(semid, 0, GETVAL);
    if (semval == -1)
        ddErrExit("semctl");

    printf("Semaphore value: %d\n", semval);

    printf("Operazione P eseguita\n");
    exit(EXIT_SUCCESS);
}

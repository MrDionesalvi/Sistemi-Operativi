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

    // Ottenere l'ID del semaforo generato da sem_00_gen.c
    semid = semget(KEY_VALUE, 1, 0);
    if (semid == -1)
        ddErrExit("semget");

    if (semctl(semid, 0, IPC_RMID) == -1)
        ddErrExit("semctl");

    printf("Operazione Remove eseguita\n");
    exit(EXIT_SUCCESS);
}

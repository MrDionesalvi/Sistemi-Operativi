#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/msg.h>
#include <string.h>

#include "server.h"

int initSemAvailable(int semId, int semNum) {
    union semun arg;
    arg.val = 1;
    return semctl(semId, semNum, SETVAL, arg);
}
int initSemInUse(int semId, int semNum) {
    union semun arg;
    arg.val = 0;
    return semctl(semId, semNum, SETVAL, arg);
}

int reserveSem(int semId, int semNum) {
    struct sembuf sops;
    sops.sem_num = semNum;
    sops.sem_op = -1;
    sops.sem_flg = 0;
    return semop(semId, &sops, 1);
}

int releaseSem(int semId, int semNum) {
    struct sembuf sops;
    sops.sem_num = semNum;
    sops.sem_op = 1;
    sops.sem_flg = 0;
    return semop(semId, &sops, 1);
}

int main(int argc, char *argv[]){
    int semid;
    union semun arg;
    semid = semget(WRITE_SEM, 1, 0);
    if (semid == -1)
        ddErrExit("semid");

    printf("Recuperato semaforo scrittore con ID = %d\n", semid);

    int semid_writer = semget(READ_SEM, 1, 0);
    if (semid_writer == -1)
        ddErrExit("semid_writer");
    printf("Recuperato semaforo lettore con ID = %d\n", semid_writer);

    int msqid = msgget(MSGQ_ID, 0);
    printf("Creato e inizializzato coda messaggi con ID = %d\n", msqid);

    char buffer[100];
    struct mymsgg msg;

    while (msgrcv(msqid, &msg, sizeof(msg) - sizeof(long), 1, 0) != -1) {
        reserveSem(semid_writer, 0);
        if (msg.mtype == 1 && strcmp(msg.mtext, "END") == 0) {
            break;
        }
        printf("%s ", msg.mtext);

        releaseSem(semid, 0);
    }
    printf("\n");
    printf("[Lettore] Reader finished reading\n");
    exit(EXIT_SUCCESS);
}

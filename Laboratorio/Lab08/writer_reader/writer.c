#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/shm.h>
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
    semid = semget(SPH, 2, IPC_CREAT | IPC_EXCL | 0666);
    if (semid == -1)
        ddErrExit("semid");

    printf("Creato e inizializzato semaforo scrittore con ID = %d\n", semid);
    initSemInUse(semid, 0);
    initSemAvailable(semid, 1); // 0 = scrittore, 1 = lettore

    int shm_id = shmget(SHM_ID, sizeof(struct mymsgg), IPC_CREAT | IPC_EXCL | 0666);
    if (shm_id == -1)
        ddErrExit("shm_id");
    printf("Creato e inizializzato memoria condivisa con ID = %d\n", shm_id);

    //char buffer[99];
    struct mymsgg *msg;
    msg = shmat(shm_id, NULL, 0);

    int still_running = 1;
    while(still_running){
        reserveSem(semid, 1);
        if(fscanf(stdin, "%99s", msg->mtext) != EOF){
            msg->mtype = 1;
            printf("[Scrittore] Inserisco messaggio: %s\n", msg->mtext);
        }
        else{
            msg->mtype = 0;
            printf("[Scrittore] Inserisco messaggio: EOF\n");
            still_running = 0;
        }
        releaseSem(semid, 0);
    }

    if(semctl(semid, 0, IPC_RMID, arg) == -1){
        ddErrExit("semctl");
    }
    if(shmctl(shm_id, IPC_RMID, NULL) == -1){
        ddErrExit("shmctl");
    }
    printf("[Scrittore] Programma terminato con successo\n");
    exit(EXIT_SUCCESS);
}

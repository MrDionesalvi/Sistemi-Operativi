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
    semid = semget(SPH, 2, 0);
    if (semid == -1)
        ddErrExit("semid");

    printf("Recuperato semafori con ID = %d\n", semid);

    int shm_id = shmget(SHM_ID, sizeof(struct mymsgg), 0);
    if (shm_id == -1)
        ddErrExit("shm_id");
    printf("Recuperata memoria condivisa con ID = %d\n", shm_id);

    char buffer[100];
    struct mymsgg *msg;

    msg = shmat(shm_id, NULL, 0);
    int bytesRead = 0;
    while (1) {
        reserveSem(semid, 0);
        if (msg->mtype == 0) {
            releaseSem(semid, 0);
            break;
        }
        bytesRead+=strlen(msg->mtext);
        // Clean the message
        msg->mtype = 0;
        memset(msg->mtext, 0, sizeof(msg->mtext));
        releaseSem(semid, 1);
    }
    printf("\n");
    printf("[Lettore] Bytes read: %d\n", bytesRead);
    printf("[Lettore] Reader finished reading\n");
    exit(EXIT_SUCCESS);
}

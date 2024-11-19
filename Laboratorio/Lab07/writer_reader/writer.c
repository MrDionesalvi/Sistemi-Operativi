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
    semid = semget(WRITE_SEM, 1, IPC_CREAT | IPC_EXCL | 0666);
    if (semid == -1)
        ddErrExit("semid");

    printf("Creato e inizializzato semaforo scrittore con ID = %d\n", semid);
    initSemAvailable(semid, 0);

    int semid_writer = semget(READ_SEM, 1, IPC_CREAT | IPC_EXCL | 0666);
    if (semid_writer == -1)
        ddErrExit("semid_writer");
    printf("Creato e inizializzato semaforo lettore con ID = %d\n", semid_writer);
    initSemInUse(semid_writer, 0);

    int msqid;
    msqid = msgget(MSGQ_ID, IPC_CREAT | 0666);
    if (msqid == -1)
        ddErrExit("msgget");
    printf("Creato e inizializzato coda messaggi con ID = %d\n", msqid);

    //char buffer[99];
    struct msqid_ds buf;
    // TODO: mymsg is [1] char text, i'm using invalid size
    struct mymsgg msg;
    while(fscanf(stdin, "%s", msg.mtext) != EOF){
        reserveSem(semid, 0);
        msg.mtype = 1;

        if(msgsnd(msqid, &msg, sizeof(msg) - sizeof(long), 0) == -1)
            ddErrExit("msgsnd");

        releaseSem(semid_writer, 0);
    }

    msg.mtype = 1;
    strncpy(msg.mtext, "END", sizeof(msg.mtext) - 1);
    if(msgsnd(msqid, &msg, sizeof(msg) - sizeof(long), 0) == -1)
        ddErrExit("msgsnd");    

    if(semctl(semid, 0, IPC_RMID, arg) == -1){
        ddErrExit("semctl");
    }
    if(semctl(semid_writer, 0, IPC_RMID, arg) == -1){
        ddErrExit("semctl");
    }
    if(msgctl(msqid, IPC_RMID, NULL) == -1){
        ddErrExit("msgctl");
    }
    printf("[Scrittore] Programma terminato con successo\n");
    exit(EXIT_SUCCESS);
}

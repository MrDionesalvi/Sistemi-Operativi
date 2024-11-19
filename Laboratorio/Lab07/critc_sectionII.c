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


#define ddErrExit(msg) {perror(msg); exit(EXIT_FAILURE);}
#define SYNC_SEM 3331
#define MSGQ_ID 4441

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

void critic_section(){
    printf("Inizio sezione critica\n");
    sleep(2);
    printf("Fine sezione critica\n");
}

int main(){
    int semid = semget(SYNC_SEM, 1, IPC_CREAT | IPC_EXCL | 0666);
    if(semid == -1) ddErrExit("semget");
    printf("Creato e inizializzato semaforo con ID = %d\n", semid);
    initSemInUse(semid, 0);

    int msqid = msgget(MSGQ_ID, IPC_CREAT | IPC_EXCL | 0666);
    if(msqid == -1) ddErrExit("msgget");
    printf("Creato e inizializzato coda messaggi con ID = %d\n", msqid);
    struct mymsgg {
        long mtype;
        char mtext[100];
    } msg;

    pid_t pid;

    switch(pid = fork()){
        case -1:
            ddErrExit("fork");
            break;
        case 0:
            reserveSem(semid, 0);
            msgrcv(msqid, &msg, sizeof(msg) - sizeof(long), 1, 0);
            if(msg.mtext[0] == '1') {
                printf("\nFiglio ha il semaforo\n");
                critic_section();
                printf("Figlio rilascia il semaforo\n\n");
            }
            releaseSem(semid, 0);
            exit(EXIT_SUCCESS);
        default:
            msg.mtype = 1;
            msg.mtext[0] = '1';
            msgsnd(msqid, &msg, sizeof(msg) - sizeof(long), 0);
            releaseSem(semid, 0);
            printf("Padre rilascia il semaforo\n");
            wait(NULL);
            reserveSem(semid, 0);
            printf("Padre ha il semaforo\n");
            break;
    }
    if(semctl(semid, 0, IPC_RMID) == -1) ddErrExit("semctl");
    if(msgctl(msqid, IPC_RMID, NULL) == -1) ddErrExit("msgctl");
    printf("Semaforo e coda messaggi rimossi\n");
}
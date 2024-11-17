#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>

#define ddErrExit(msg) {perror(msg); exit(EXIT_FAILURE);}
#define KEY_VALUE 12345

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
    semid = semget(KEY_VALUE, 1, IPC_CREAT | IPC_EXCL | 0666);
    if (semid == -1)
        ddErrExit("semid");
    printf("Creato e inizializzato semaforo con ID = %d\n", semid);
    initSemInUse(semid, 0);
    printf("Semaforo inizializzato\n");

    pid_t pid;
    switch (pid = fork()){
    case -1:
        ddErrExit("fork");
        break;
    case 0:
        // Figlio
        printf("Figlio in attesa del semaforo\n");
        sleep(5);
        printf("Figlio rilascia il semaforo\n");
        if (releaseSem(semid, 0) == -1){
            ddErrExit("releaseSem");
        }
        break;
    default:
        // Padre
        if (reserveSem(semid, 0) == -1){
            ddErrExit("reserveSem");
        }
        printf("Semaforo rilasciato\n");
        if(semctl(semid, 0, IPC_RMID) == -1)
            ddErrExit("semctl");
        printf("Semaforo rimosso\n");
        break;
    }
    exit(EXIT_SUCCESS);
}

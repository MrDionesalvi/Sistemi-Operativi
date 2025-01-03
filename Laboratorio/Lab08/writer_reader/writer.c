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
#include <errno.h>

#include "server.h"

int semid, shm_id;
struct mymsgg *msg;


void cleanup(){
    if(semctl(semid, 0, IPC_RMID, NULL) == -1){
        ddErrExit("semctl");
    }
    if(shmctl(shm_id, IPC_RMID, NULL) == -1){
        ddErrExit("shmctl");
    }
    shmdt(msg);
    printf("[Writer] Programma terminato con errore\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]){
    union semun arg;

    signal(SIGINT, cleanup);
    signal(SIGTERM, cleanup);

    semid = semget(SPH, 2, IPC_CREAT | IPC_EXCL | 0666);
    if (semid == -1){
        if(errno == EEXIST){
            semid = semget(SPH, 2, 0);
            if (semid == -1)
                ddErrExit("semid");
        }
        else
            ddErrExit("semid");
    }

    printf("Creato e inizializzato semaforo scrittore con ID = %d\n", semid);
    initSemInUse(semid, 0);
    initSemAvailable(semid, 1); // 0 = lettore, 1 = scrittore

    shm_id = shmget(SHM_ID, sizeof(struct mymsgg), IPC_CREAT | IPC_EXCL | 0666);
    if (shm_id == -1){
        if(errno == EEXIST){
            shm_id = shmget(SHM_ID, sizeof(struct mymsgg), 0);
            if (shm_id == -1)
                ddErrExit("shm_id");
        }
        else
            ddErrExit("shm_id");
    }
    printf("Creato e inizializzato memoria condivisa con ID = %d\n", shm_id);

    //char buffer[99];
    msg = shmat(shm_id, NULL, 0);

    int still_running = 1;
    int bytes_sent = 0;
    while(still_running){
        reserveSem(semid, 1); // Richiesta di scrittura
        if(fscanf(stdin, "%99s", msg->mtext) != EOF){
            msg->mtype = 1;
            bytes_sent += strlen(msg->mtext);
        }
        else{
            msg->mtype = 0;
            printf("[Writer] EOF\n");
            still_running = 0;
        }
        releaseSem(semid, 0); // Possibilità di lettura
    }

    if(semctl(semid, 0, IPC_RMID, arg) == -1){
        ddErrExit("semctl");
    }
    if(shmctl(shm_id, IPC_RMID, NULL) == -1){
        ddErrExit("shmctl");
    }
    shmdt(msg);
    printf("[Writer] Totale byte inviati: %d\n", bytes_sent);
    printf("[Writer] Programma terminato con successo\n");
    exit(EXIT_SUCCESS);
}

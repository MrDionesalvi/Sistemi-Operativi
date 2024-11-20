#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include "sem.h"

#define WAIT_TIME_PARENT 5
#define MAX_WAIT_TIME_CHILD 9
#define ddErrExit(msg) { perror(msg); exit(EXIT_FAILURE); }

int shmid, semid;

void cleanup() {
    if (shmctl(shmid, IPC_RMID, NULL) == -1) perror("shmctl");
    if (semctl(semid, 0, IPC_RMID) == -1) perror("semctl");
}

void sigHandler(int sig) {
    cleanup();
    exit(EXIT_SUCCESS);
}

void writePidShm(int shmid, pid_t pid) {
    long int *p = (long int *)shmat(shmid, NULL, 0);
    if (p == (void *)-1) ddErrExit("shmat");
    *p = pid;
    if (shmdt(p) == -1) ddErrExit("shmdt");
}

void printPidShm(int shmid) {
    long int *p = (long int *)shmat(shmid, NULL, 0);
    if (p == (void *)-1) ddErrExit("shmat");
    printf("pid: %ld\n", *p);
    if (shmdt(p) == -1) ddErrExit("shmdt");
}

int main(int argc, char const *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <num_child> <num_cycle_for_child>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, sigHandler);
    signal(SIGTERM, sigHandler);

    int num_child = atoi(argv[1]);
    int num_cycle_for_child = atoi(argv[2]);

    shmid = shmget(IPC_PRIVATE, sizeof(long int), IPC_CREAT | IPC_EXCL | 0666);
    if (shmid == -1) ddErrExit("shmget");

    semid = semget(IPC_PRIVATE, 2, IPC_CREAT | IPC_EXCL | 0666);
    if (semid == -1) ddErrExit("semget");

    if (initSemAvailable(semid, 0) == -1) ddErrExit("initSemAvailable");
    if (initSemInUse(semid, 1) == -1) ddErrExit("initSemInUse");

    for (size_t i = 0; i < num_child; i++) {
        pid_t pid = fork();
        if (pid == -1) ddErrExit("fork");
        if (pid == 0) {
            srand(time(NULL) ^ getpid());
            for (size_t j = 0; j < num_cycle_for_child; j++) {
                sleep(rand() % MAX_WAIT_TIME_CHILD);
                if (reserveSem(semid, 0) == -1) ddErrExit("reserveSem");
                writePidShm(shmid, getpid());
                if (releaseSem(semid, 0) == -1) ddErrExit("releaseSem");
            }
            exit(EXIT_SUCCESS);
        }
    }

    for (size_t i = 0; i < num_cycle_for_child; i++) {
        sleep(WAIT_TIME_PARENT);
        if (reserveSem(semid, 0) == -1) ddErrExit("reserveSem");
        printPidShm(shmid);
        if (releaseSem(semid, 0) == -1) ddErrExit("releaseSem");
    }

    for (size_t i = 0; i < num_child; i++) {
        if (wait(NULL) == -1) ddErrExit("wait");
    }

    cleanup();
    return 0;
}

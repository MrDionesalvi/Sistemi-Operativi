#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <semid>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int semid = atoi(argv[1]);
    struct semid_ds buf;

    if (semctl(semid, 0, IPC_STAT, &buf) == -1) {
        perror("semctl");
        exit(EXIT_FAILURE);
    }

    printf("Semaphore %d information:\n", semid);
    printf("Owner's UID: %d\n", buf.sem_perm.cuid);
    printf("Owner's GID: %d\n", buf.sem_perm.cgid);
    printf("Creator's UID: %d\n", buf.sem_perm.uid);
    printf("Creator's GID: %d\n", buf.sem_perm.gid);
    printf("Mode: %o\n", buf.sem_perm.mode);
    printf("Number of semaphores in set: %hu\n", buf.sem_nsems);
    printf("Last semop operation time: %s", ctime(&buf.sem_otime));
    printf("Last semop change time: %s", ctime(&buf.sem_ctime));

    printf("\n%-10s %-10s %-10s %-10s %-10s\n", "Sem #", "Value", "SEMPID", "SEMNCNT", "SEMZCNT");

    for (unsigned short i = 0; i < buf.sem_nsems; i++) {
        int semval = semctl(semid, i, GETVAL);
        int sempid = semctl(semid, i, GETPID);
        int semncnt = semctl(semid, i, GETNCNT);
        int semzcnt = semctl(semid, i, GETZCNT);

        if (semval == -1 || sempid == -1 || semncnt == -1 || semzcnt == -1) {
            perror("semctl");
            exit(EXIT_FAILURE);
        }

        printf("%-10d %-10d %-10d %-10d %-10d\n", i, semval, sempid, semncnt, semzcnt);
    }

    return 0;
}
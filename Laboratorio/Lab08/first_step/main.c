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

#define SHM_ID 1234
#define SMP_ID 1235
#define DATASZ 1024
#define ddErrExit(msg) {perror(msg); exit(EXIT_FAILURE);}

typedef struct my_data { 
  int counter; 
  char stringa[DATASZ]; 
} data;

int main(int argc, char const *argv[]){
    int shmid;
    shmid = shmget(SHM_ID, sizeof(data), IPC_CREAT | IPC_EXCL | 0666);
    if(shmid == -1){
        if(errno == EEXIST){
            shmid = shmget(SHM_ID, sizeof(data), 0);
            if(shmid == -1) ddErrExit("shmget");
        } else {
            ddErrExit("shmget");
        }
    }

    data *d = (data *)shmat(shmid, NULL, 0);
    if(d == (void *)-1) ddErrExit("shmat");

    d->counter = 0;
    printf("Inserisci una stringa: ");
    fscanf(stdin, "%1023s", d->stringa);

    if(shmdt(d) == -1) ddErrExit("shmdt");

    pid_t pid;
    switch (pid = fork())
    {
    case -1:
        ddErrExit("fork");
        break;
    case 0:
        // Figlio
        d = (data *)shmat(shmid, NULL, 0);
        if(d == (void *)-1) ddErrExit("shmat");
        printf("Stringa inserita dal padre: %s\n", d->stringa);
        printf("Counter: %d\n", d->counter);
        if(shmdt(d) == -1) ddErrExit("shmdt");
        exit(EXIT_SUCCESS);
        break;
    }

    wait(NULL);
    if(shmctl(shmid, IPC_RMID, NULL) == -1) ddErrExit("shmctl");
    printf("Risorsa deallocata\n");
    return 0;
}

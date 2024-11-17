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
#define KEY_VALUE 1234

union semunn {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

int main(int argc, char *argv[]){
    int semid;
    union semun arg;
    semid = semget(KEY_VALUE, 1, 0);
    if (semid == -1)
        ddErrExit("semid");
    arg.val = 0;
    if (semctl(semid, /* semnum= */ 0, SETVAL, arg) == -1)
        ddErrExit("semctl");
    printf("Operation V executed\n");
    exit(EXIT_SUCCESS);
}

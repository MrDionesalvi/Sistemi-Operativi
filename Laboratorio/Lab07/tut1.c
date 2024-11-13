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

union semunn {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

char* get_curr_time() {
    time_t t;
    time(&t);
    return ctime(&t);
}

int main(int argc, char *argv[]){
    int semid;
    if (argc == 2){ // ----------- creo e inizializzo un sem --------
        union semun arg;
        semid = semget(IPC_PRIVATE, 1, S_IRUSR | S_IWUSR);
        if (semid == -1)
            ddErrExit("semid");
        arg.val = atoi(argv[1]);
        if (semctl(semid, /* semnum= */ 0, SETVAL, arg) == -1)
            ddErrExit("semctl");
        printf("creato e inizializzato semaforo con ID = %d\n", semid);
    }
    else {                      // -------- eseguo un'operazione sul primo semaforo -----
        struct sembuf sop; // la struttura che definisce l'operazione
        semid = atoi(argv[1]);
        sop.sem_num = 0; // specifica il primo semaforo nel set
        sop.sem_op = atoi(argv[2]);
        // aggiungo, sottraggo o attendo 0

        sop.sem_flg = 0; // per ora NON settiamo flag per effettuare
        // operazioni speciali
        printf("%ld: info: %d %d\n",
               (long)getpid(), semid, sop.sem_op);
        printf("%ld: about to semop at  %s\n",
               (long)getpid(), get_curr_time());
        if (semop(semid, &sop, 1) == -1)
            ddErrExit("semop");
        printf("%ld: semop completed at %s\n",
               (long)getpid(), get_curr_time());
    }
    exit(EXIT_SUCCESS);
}

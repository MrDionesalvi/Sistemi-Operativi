#define ddErrExit(msg) {perror(msg); exit(EXIT_FAILURE);}
#define SPH 333
#define SHM_ID 555
struct mymsgg {
    long mtype;
    char mtext[100];
};
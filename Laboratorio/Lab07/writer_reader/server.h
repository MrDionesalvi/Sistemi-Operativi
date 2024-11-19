#define ddErrExit(msg) {perror(msg); exit(EXIT_FAILURE);}
#define WRITE_SEM 333
#define READ_SEM 444
#define MSGQ_ID 555
struct mymsgg {
    long mtype;
    char mtext[100];
};
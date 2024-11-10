#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> /* For portability */
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>


struct mymsg { 
    long mtype;   /* Message type */ 
    char mtext[100]; /* Message body */
} msg;

int main(int argc, const char* argv[]){

    if(argc != 2){
        fprintf(stderr, "Usage: %s <number of processes>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen("msgq_id.txt", "r");
    if(f == NULL){
        perror("fopen");
        return 1;
    }
    int msqid;
    fscanf(f, "%d", &msqid);

    struct msqid_ds buf;
    if (msgctl(msqid, IPC_STAT, &buf) == -1) {
        perror("msgctl");
        return 1;
    }

    printf("Message queue identifier: %d\n", msqid);

    for(int i = 0; i < atoi(argv[1]); i++){
        pid_t pid;
        switch(pid = fork()){
            case -1:
                perror("fork");
                return 1;
            case 0:
                pid_t pid = getpid();
                msg.mtype = 1; 
                
                snprintf(msg.mtext, sizeof(msg.mtext), "%d %d", i, pid);
                printf("Message (%d) text: %s\n", i, msg.mtext);
                printf("Message (%d) type: %ld\n", i, msg.mtype);
                
                if (msgsnd(msqid, &msg, sizeof(msg) - sizeof(long), 0) < 0) {
                    perror("msgsnd");
                    return 1;
                }
                
                printf("Message (%d) sent to message queue. Waiting for response...\n", i);
                
                if (msgrcv(msqid, &msg, sizeof(msg) - sizeof(long), pid, 0) != -1) {
                    printf("Message response for (%d) received from message queue.\n", i);
                    printf("Message text: %s\n", msg.mtext);
                } else {
                    perror("msgrcv");
                    return 1;
                }
                
                return 1;

            default:
                // Parent process
                wait(NULL);
        }
    }

    for(int i = 0; i < argc-1; i++){
        wait(NULL);
    }

    
    msg.mtype = 1;
    strncpy(msg.mtext, "END", sizeof(msg.mtext) - 1);
    if(msgsnd(msqid, &msg, sizeof(msg)-sizeof(long), IPC_NOWAIT) < 0){
        perror("msgsnd");
        return 1;
    }
    

    printf("All messages sent. Closing connection...\n");
    return 0;
}
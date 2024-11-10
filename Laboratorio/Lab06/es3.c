#include <stdio.h>
#include <sys/types.h> /* For portability */
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>


#define RESET "\033[0m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define GREEN "\033[32m"

struct mymsg { 
    long mtype;   /* Message type */ 
    char mtext[100]; /* Message body */
} msg;

int main(){

    int msqid;
    msqid = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
    if(msqid == -1){
        perror("msgget");
        return 1;
    }
    printf(YELLOW "Message queue identifier: %d\n" RESET, msqid);

    pid_t pid;
    switch(pid = fork()){
        case -1:
            perror("fork");
            return 1;
        case 0:
            // Child process
            msg.mtype = 1;
            snprintf(msg.mtext, sizeof(msg.mtext), "Saluti da: %d", getppid());
            msgsnd(msqid, &msg, sizeof(msg)-sizeof(long), IPC_NOWAIT);
            return 1;
        default:
            // Parent process
            if (msgrcv(msqid, &msg, sizeof(msg)-sizeof(long), 1, 0) != -1){
                printf(GREEN "Message received from message queue.\n" RESET);
                printf("Message text: %s\n", msg.mtext);
            }
            else{
                perror("msgrcv");
                return 1;
            }
            wait(NULL);
            msgctl(msqid, IPC_RMID, NULL);
            return 1;
    }
    return 0;
}
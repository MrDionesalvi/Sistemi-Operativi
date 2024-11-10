#include <stdio.h>
#include <sys/types.h> /* For portability */
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#include <time.h>

#define RESET "\033[0m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define GREEN "\033[32m"

struct mymsg { 
    long mtype;   /* Message type */ 
    char mtext[100]; /* Message body */
} msg;


int main(){
    FILE *f = fopen("msgq_id.txt", "r");
    if(f == NULL){
        perror("fopen");
        return 1;
    }
    int msqid;
    fscanf(f, "%d", &msqid);
    fclose(f);
    printf(YELLOW "Message queue identifier: %d\n" RESET, msqid);

    // check if message queue exists
    if(msgget(msqid, 0) == -1){
        perror("msgget");
        return 1;
    }

    struct msqid_ds msgctl_ds;
    msgctl(msqid, IPC_STAT, &msgctl_ds);
    if(msqid == -1){
        perror("msgctl");
        return 1;
    }

    printf("Size of msg_qbytes: %ld\n", msgctl_ds.msg_qbytes);
    printf("Last timestamp sended message: %s\n", ctime(&msgctl_ds.msg_stime));

    int type;
    printf("\nInsert message type | Format: id\n");
    scanf("%d", &type);

    while(msgrcv(msqid, &msg, sizeof(msg)-sizeof(long), type, IPC_NOWAIT) != -1){
        printf(GREEN "Message received from message queue.\n" RESET);
        printf("Message text: %s\n", msg.mtext);
    }

    return 0;
}
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
    int msqid = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
    if(msqid == -1){
        perror("msgget");
        return 1;
    }
    printf(YELLOW "Message queue identifier: %d\n" RESET, msqid);

    FILE *f = fopen("msgq_id.txt", "w");
    if(f == NULL){
        perror("fopen");
        return 1;
    }
    fprintf(f, "%d", msqid);
    fclose(f);
    printf(GREEN "Message queue identifier written to msgq_id.txt.\n" RESET);


    printf("\nInsert message type and message text | Format: id-message\n");
    while(fscanf(stdin, "%ld-%s", &msg.mtype, msg.mtext) != EOF){ // TODO: check if message type is valid
        if(msgsnd(msqid, &msg, sizeof(msg)-sizeof(long), IPC_NOWAIT) < 0){
            perror("msgsnd");
            return 1;
        }
        printf(GREEN "Message sent to message queue.\n" RESET);
    }

    struct msqid_ds msgctl_ds;
    msgctl(msqid, IPC_STAT, &msgctl_ds);
    if(msqid == -1){
        perror("msgctl");
        return 1;
    }
    printf("Size of msg_qbytes: %ld\n", msgctl_ds.msg_qbytes);
    printf("Last timestamp sended message: %s\n", ctime(&msgctl_ds.msg_stime));

    msgctl(msqid, IPC_RMID, NULL);
    printf(RED "Message queue removed.\n" RESET);

    return 0;
}
#include <stdio.h>
#include <sys/types.h> /* For portability */
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>


struct mymsg { 
    long mtype;   /* Message type */ 
    char mtext[100]; /* Message body */
} msg;


int main(){
    int msqid;
    msqid = msgget(1234, IPC_CREAT | 0666);
    if(msqid == -1){
        perror("msgget");
        return 1;
    }
    printf("Message queue identifier: %d\n", msqid);

    FILE *f = fopen("msgq_id.txt", "w");
    if(f == NULL){
        perror("fopen");
        return 1;
    }
    fprintf(f, "%d", msqid);
    fclose(f);
    printf("Message queue identifier written to msgq_id.txt.\n\n");
    srand(time(NULL));

    struct mymsg response;
    while (msgrcv(msqid, &msg, sizeof(msg) - sizeof(long), 1, 0) != -1) {
        printf("Message received from message queue.\n");
        printf("Message text: %s\n", msg.mtext);
        printf("Message type: %ld\n", msg.mtype);

        int upper_limit;
        pid_t client_pid;
        
        sscanf(msg.mtext, "%d %d", &upper_limit, &client_pid);

        int random_number = rand() % (upper_limit + 1);
        snprintf(response.mtext, sizeof(response.mtext), "Random number between 0 and %d: %d\n", upper_limit, random_number);
        
        response.mtype = client_pid;
        
        if (msgsnd(msqid, &response, sizeof(response) - sizeof(long), 0) < 0) {
            perror("msgsnd");
            return 1;
        }
        if (msg.mtype == 1 && strcmp(msg.mtext, "END") == 0) {
            break;
        }
    }




    msgctl(msqid, IPC_RMID, NULL);
    printf("Message queue removed.\n");
    return 0;
}
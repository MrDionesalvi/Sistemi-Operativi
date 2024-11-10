#include <stdio.h>
#include <sys/types.h> /* For portability */
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#include <time.h>

struct mymsg { 
    long mtype;   /* Message type */ 
    char mtext[100]; /* Message body */
} msg;

int main(){
    /**
     * This program demonstrates the use of System V message queues in C.
     * It performs the following operations:
     * 1. Creates a new message queue with read and write permissions for all users.
     * 2. Retrieves and prints the message queue identifier.
     * 3. Retrieves and prints the size of the message queue and the last message sent timestamp.
     * 4. Sends a message to the message queue.
     * 5. Retrieves and prints the updated size of the message queue and the last message sent timestamp.
     * 6. Removes the message queue.
     *
     * Functions used:
     * - msgget: Creates a new message queue or accesses an existing one.
     * - msgctl: Performs various control operations on the message queue.
     * - msgsnd: Sends a message to the message queue.
     * - perror: Prints a descriptive error message to stderr.
     * - printf: Prints formatted output to stdout.
     * - strncpy: Copies a specified number of characters from one string to another.
     * - ctime: Converts a time value to a string representation.
     *
     * Error handling:
     * - If msgget fails, an error message is printed and the program exits with a status of 1.
     * - If msgctl fails, an error message is printed and the program exits with a status of 1.
     * - If msgsnd fails, an error message is printed and the program exits with a status of 1.
     */
    int msqid = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
    if(msqid == -1){
        perror("msgget");
        return 1;
    }
    printf("Message queue identifier: %d\n", msqid);

    struct msqid_ds msgctl_ds;

    msgctl(msqid, IPC_STAT, &msgctl_ds);
    if(msqid == -1){
        perror("msgctl");
        return 1;
    }
    printf("Size of msg_qbytes: %ld\n", msgctl_ds.msg_qbytes);
    printf("Last timestamp sended message: %ld\n", msgctl_ds.msg_stime);

    msg.mtype = 1;
    strncpy(msg.mtext, "Hello, world!", sizeof(msg.mtext) - 1);
    msg.mtext[sizeof(msg.mtext) - 1] = '\0'; // Ensure null-termination

    if(msgsnd(msqid, &msg, sizeof(msg)-sizeof(long), IPC_NOWAIT) < 0){
        perror("msgsnd");
        return 1;
    }

    printf("Message sended: %s\n", msg.mtext);
    msgctl(msqid, IPC_STAT, &msgctl_ds);
    printf("Size of msg_qbytes: %ld\n", msgctl_ds.msg_qbytes);
    char *times = ctime(&msgctl_ds.msg_stime);
    printf("Last timestamp sended message: %s\n", times);

    msgctl(msqid, IPC_RMID, NULL);
    return 0;


}